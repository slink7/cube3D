/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:56 by ymostows          #+#    #+#             */
/*   Updated: 2024/05/29 14:50:44 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "incl.h"
#include <math.h>
#include <mlx.h>

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080
#define MOVE_SPEED 0.2
#define MOUSE_SENS 0.005
#define TEXTURE_SIZE 64

typedef struct s_rays {
    double	camera_x;
    double	ray_dir_x;
    double	ray_dir_y;
    double	delta_dist_x;
    double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
    double  step;
    double  tex_pos;
    double  wall_x;
    int		map_x;
    int		map_y;
    int		step_x;
    int		step_y;
	int		side;
	int 	line_height;
	int		draw_start;
	int		draw_end;
    int     tex_num;
    int     tex_x;
    int     tex_y;
} t_rays;

#define NUM_TEXTURES 4

typedef struct s_imgs {
    void    *img;
    int     *addr;
    int     bpp;
    int     line_length;
    int     endian;
} t_imgs;

typedef struct s_mouse {
    int last_x;
    int last_y;
} t_mouse;

typedef struct s_data {
    void    *mlx_ptr;
    void    *win_ptr;
    int     world_map[24][24];
    double  pos_x;
    double  pos_y;
    double  dir_x;
    double  dir_y;
    double  plane_x;
    double  plane_y;
    t_imgs   texture[NUM_TEXTURES];
    int     *texture_buffer[NUM_TEXTURES];
    t_imgs   img;
	t_image	backbuffer;
    t_mouse *mouse;
} t_data;

void rotate_view(t_data *data, double angle)
{
    double old_dir_x;
    double old_plane_x;

    old_dir_x = data->dir_x;
    old_plane_x = data->plane_x;

    data->dir_x = data->dir_x * cos(angle) - data->dir_y * sin(angle);
    data->dir_y = old_dir_x * sin(angle) + data->dir_y * cos(angle);
    data->plane_x = data->plane_x * cos(angle) - data->plane_y * sin(angle);
    data->plane_y = old_plane_x * sin(angle) + data->plane_y * cos(angle);
}

int mouse_move(int x, int y, t_data *data) {
    double  delta_x;

    if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT) {
        data->mouse->last_x = x;
        data->mouse->last_y = y;
        return 0;
    }
    delta_x = x - data->mouse->last_x;
    rotate_view(data, -delta_x * MOUSE_SENS);
    data->mouse->last_x = x;
    data->mouse->last_y = y;
    //mlx_mouse_move(data->mlx_ptr, data->win_ptr, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    return 0;
}

void load_texture(t_data *data, int index, char *path)
{
    t_imgs img;
    int x;
	int	y;
	
    img.img = mlx_xpm_file_to_image(data->mlx_ptr, path, &x, &y);
    if (!img.img || x != TEXTURE_SIZE || y != TEXTURE_SIZE)
        return;
    img.addr = (int *)mlx_get_data_addr(img.img, &img.bpp, &img.line_length, &img.endian);
    data->texture_buffer[index] = malloc(sizeof(int) * TEXTURE_SIZE * TEXTURE_SIZE);
    for (int i = 0; i < TEXTURE_SIZE; i++) {
        for (int j = 0; j < TEXTURE_SIZE; j++) {
            data->texture_buffer[index][i * TEXTURE_SIZE + j] = img.addr[i * TEXTURE_SIZE + j];
        }
    }
    mlx_destroy_image(data->mlx_ptr, img.img);
}

void init_textures(t_data *data)
{
    load_texture(data, NORTH, "textures/wall2.xpm");
    load_texture(data, SOUTH, "textures/wall2.xpm");
    load_texture(data, WEST, "textures/wall2.xpm");
    load_texture(data, EAST, "textures/wall2.xpm");
}


void    put_pixel(t_image *img, int x, int y, int color)
{
    int index;
    
    index = img->line_length * y + x * (img->bits_per_pixel / 8);
    if (img->endian == 1)        // Most significant (Alpha) byte first
    {
        img->data[index + 0] = (color >> 24);
        img->data[index + 1] = (color >> 16) & 0xFF;
        img->data[index + 2] = (color >> 8) & 0xFF;
        img->data[index + 3] = (color) & 0xFF;
    }
    else if (img->endian == 0)   // Least significant (Blue) byte first
    {
        img->data[index + 0] = (color) & 0xFF;
        img->data[index + 1] = (color >> 8) & 0xFF;
        img->data[index + 2] = (color >> 16) & 0xFF;
        img->data[index + 3] = (color >> 24);
    }
}

void init_imgs(t_data *data,t_image *img, int width, int heigth)
{
    img->addr = mlx_new_image(data->mlx_ptr, width, heigth);
    img->data = mlx_get_data_addr(img->addr, &img->bits_per_pixel, &img->line_length, &img->endian);
    img->width = width;
    img->height = heigth;
}

void move_forward(t_data *data)
{
    if (data->world_map[(int)(data->pos_x + data->dir_x * MOVE_SPEED)][(int)(data->pos_y)] != 1)
        data->pos_x += data->dir_x * MOVE_SPEED;
    if (data->world_map[(int)(data->pos_x)][(int)(data->pos_y + data->dir_y * MOVE_SPEED)] != 1)
        data->pos_y += data->dir_y * MOVE_SPEED;
}

void move_backward(t_data *data)
{
    if (data->world_map[(int)(data->pos_x - data->dir_x * MOVE_SPEED)][(int)(data->pos_y)] != 1)
        data->pos_x -= data->dir_x * MOVE_SPEED;
    if (data->world_map[(int)(data->pos_x)][(int)(data->pos_y - data->dir_y * MOVE_SPEED)] != 1)
        data->pos_y -= data->dir_y * MOVE_SPEED;
}

void strafe_right(t_data *data)
{
    double strafe_x;
    double strafe_y;
    
    strafe_x = data->dir_y * MOVE_SPEED;
    strafe_y = -data->dir_x * MOVE_SPEED;
    if (data->world_map[(int)(data->pos_x + strafe_x)][(int)(data->pos_y)] != 1)
        data->pos_x += strafe_x;
    if (data->world_map[(int)(data->pos_x)][(int)(data->pos_y + strafe_y)] != 1)
        data->pos_y += strafe_y;
}

void strafe_left(t_data *data)
{
    double strafe_x;
    double strafe_y;
    
    strafe_x = -data->dir_y * MOVE_SPEED;
    strafe_y = data->dir_x * MOVE_SPEED;
    if (data->world_map[(int)(data->pos_x + strafe_x)][(int)(data->pos_y)] != 1)
        data->pos_x += strafe_x;
    if (data->world_map[(int)(data->pos_x)][(int)(data->pos_y + strafe_y)] != 1)
        data->pos_y += strafe_y;
}
int key_press(int keycode, t_data *data)
{
    if (keycode == 'z')
        move_forward(data);
    if (keycode == 's')
        move_backward(data);
    if (keycode == 'q')
        strafe_left(data);
    if (keycode == 'd')
        strafe_right(data);
    return 0;
}

void    render_floor_ceiling(t_data *data)
{
    int     x;
    int     y;
    
    y = -1;
    while (++y < WIN_HEIGHT / 2)
    {
        x= -1;
        while (++x < WIN_WIDTH)
            put_pixel(&data->backbuffer, x, y, 0x009AED);
    }
    y = WIN_HEIGHT / 2 -1;
    while (++y < WIN_HEIGHT)
    {
        x = -1;
        while (++x < WIN_WIDTH)
            put_pixel(&data->backbuffer, x, y, 0x2D610C);
    }
}

void    calcul_delta_dist(t_data *data, t_rays *vars, int x)
{   
    vars->camera_x = 2 * x / (double)WIN_WIDTH - 1;
    vars->ray_dir_x = data->dir_x + data->plane_x * vars->camera_x;
    vars->ray_dir_y = data->dir_y + data->plane_y * vars->camera_x;

    vars->map_x = (int)data->pos_x;
    vars->map_y = (int)data->pos_y;

    vars->delta_dist_x = fabs(1 / vars->ray_dir_x);
    vars->delta_dist_y = fabs(1 / vars->ray_dir_y);
}

void    calcul_side_dist(t_data *data, t_rays *vars)
{   
        if (vars->ray_dir_x < 0)
        {
            vars->step_x = -1;
            vars->side_dist_x = (data->pos_x - vars->map_x) * vars->delta_dist_x;
        }
        else
        {
            vars->step_x = 1;
            vars->side_dist_x = (vars->map_x + 1.0 - data->pos_x) * vars->delta_dist_x;
        }

        if (vars->ray_dir_y < 0)
        {
            vars->step_y = -1;
            vars->side_dist_y = (data->pos_y - vars->map_y) * vars->delta_dist_y;
        }
        else
        {
            vars->step_y = 1;
            vars->side_dist_y = (vars->map_y + 1.0 - data->pos_y) * vars->delta_dist_y;
        }
}

void    calcul_wall_pos(t_data *data, t_rays *vars)
{   
    while (1)
    {
        if (vars->side_dist_x < vars->side_dist_y)
        {
            vars->side_dist_x += vars->delta_dist_x;
            vars->map_x += vars->step_x;
            vars->side = 0;
        }
        else
        {
            vars->side_dist_y += vars->delta_dist_y;
            vars->map_y += vars->step_y;
            vars->side = 1;
        }
        if (data->world_map[vars->map_x][vars->map_y] > 0 && data->world_map[vars->map_x][vars->map_y] != 2) 
            break;
    }
}

void    calcul_wall_size(t_data *data, t_rays *vars)
{   
    double perp_wall_dist;
    
    if (vars->side == 0)
        perp_wall_dist = (vars->map_x - data->pos_x + (1 - vars->step_x) / 2) / vars->ray_dir_x;
    else
        perp_wall_dist = (vars->map_y - data->pos_y + (1 - vars->step_y) / 2) / vars->ray_dir_y;
    vars->line_height = (int)(WIN_HEIGHT / perp_wall_dist);
    vars->draw_start = -vars->line_height / 2 + WIN_HEIGHT / 2;
    if (vars->draw_start < 0)
        vars->draw_start = 0;
    vars->draw_end = vars->line_height / 2 + WIN_HEIGHT / 2;
    if (vars->draw_end >= WIN_HEIGHT)
        vars->draw_end = WIN_HEIGHT - 1;
    if (vars->side == 0)
        vars->wall_x = data->pos_y + perp_wall_dist * vars->ray_dir_y;
    else
        vars->wall_x = data->pos_x + perp_wall_dist * vars->ray_dir_x;
}


void select_texture(t_rays *rays)
{
    if (rays->side == 0)
    {
        if (rays->ray_dir_x > 0)
            rays->tex_num = EAST;
        else
            rays->tex_num = WEST;
    }
    else
    {
        if (rays->ray_dir_y > 0)
            rays->tex_num = SOUTH;
        else
            rays->tex_num = NORTH;
    }
}

void    handle_textures(t_rays *vars)
{
    vars->wall_x -= floor(vars->wall_x);
    vars->tex_x = (int)(vars->wall_x * (double)TEXTURE_SIZE);
    if ((vars->side == 0 && vars->ray_dir_x > 0) || (vars->side == 1 && vars->ray_dir_y < 0))
        vars->tex_x = TEXTURE_SIZE - vars->tex_x - 1;
    vars->step = 1.0 * TEXTURE_SIZE / vars->line_height;
    vars->tex_pos = (vars->draw_start - WIN_HEIGHT / 2 + vars->line_height / 2) * vars->step;
}

void    draw_walls(t_data *data, t_rays *vars, int x)
{
    int y;
    int tex_y;
    int color;
    
    y = vars->draw_start - 1;
    while (++y < vars->draw_end)
    {
        tex_y = (int)vars->tex_pos & (TEXTURE_SIZE - 1);
        vars->tex_pos += vars->step;
        color = data->texture_buffer[vars->tex_num][TEXTURE_SIZE * tex_y + vars->tex_x];
        if (vars->side == 1)
            color = (color >> 1) & 0x7F7F7F;
        put_pixel(&data->backbuffer, x, y, color);
    }
}

int render_frame(t_data *data) {
    t_rays  vars;
    int     x;
    
    render_floor_ceiling(data);
    x = -1;
    while (++x < WIN_WIDTH)
    {
        calcul_delta_dist(data, &vars, x);
        calcul_side_dist(data, &vars);
        calcul_wall_pos(data, &vars);
        calcul_wall_size(data, &vars);
        select_texture(&vars);
        handle_textures(&vars);
        draw_walls(data, &vars, x);
    }
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->backbuffer.addr, 0, 0);
    return 0;
}


int main() 
{
    t_data  data;
    t_mouse mouse;

    data.mlx_ptr = mlx_init();
    data.win_ptr = mlx_new_window(data.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "Cub3D");
    
    mouse.last_x = WIN_WIDTH / 2;
    mouse.last_y = WIN_HEIGHT / 2;
    data.mouse = &mouse;

    int world_map[24][24] = {
        { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 1, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1 },
        { 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    };

    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            data.world_map[i][j] = world_map[i][j];
        }
    }

    data.pos_x = 10.0;
    data.pos_y = 12.0;
    data.dir_x = -1.0;
    data.dir_y = 0.0;
    data.plane_x = 0.0;
    data.plane_y = 0.66;

    init_textures(&data);
	init_imgs(&data, &data.backbuffer, WIN_WIDTH, WIN_HEIGHT);
    mlx_hook(data.win_ptr, 2, 1L << 0, key_press, &data);
    mlx_hook(data.win_ptr, 6, 1L << 6, mouse_move, &data);
    mlx_loop_hook(data.mlx_ptr, &render_frame, &data);
    /*mlx_mouse_move(data.mlx_ptr, data.win_ptr, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    mlx_mouse_hide(data.mlx_ptr, data.win_ptr);*/
    mlx_loop(data.mlx_ptr);
    return 0;
}