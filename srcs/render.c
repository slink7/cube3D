/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:56 by ymostows          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/06/17 13:24:33 by ymostows         ###   ########.fr       */
=======
/*   Updated: 2024/05/30 16:28:27 by ymostows         ###   ########.fr       */
>>>>>>> refs/remotes/origin/testrender
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "incl.h"
#include <time.h>
#include <math.h>
#include <mlx.h>

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080
#define MOVE_SPEED 0.2
#define MOUSE_SENS 0.005
#define TEXTURE_SIZE 64

typedef struct s_cf_rays
{
    double	ray_dir_x0;
    double	ray_dir_y0;
    double	ray_dir_x1;
    double	ray_dir_y1;
    double	row_distance;
    double	floor_step_x;
    double	floor_step_y;
    double  inv_win;
    double	floor_x;
    double	floor_y;
    int		cell_x;
    int		cell_y;
    int		tx;
    int		ty;
    int		color;
    int     texture_size_mask;
} t_cf_rays;

typedef struct s_rays {
    double	    camera_x;
    double	    ray_dir_x;
    double	    ray_dir_y;
    double	    delta_dist_x;
    double	    delta_dist_y;
	double	    side_dist_x;
	double	    side_dist_y;
    double      step;
    double      tex_pos;
    double      wall_x;
    double      perp_wall_dist;
    int		    map_x;
    int		    map_y;
    int		    step_x;
    int		    step_y;
	int		    side;
	int 	    line_height;
	int		    draw_start;
	int		    draw_end;
    int         tex_num;
    int         tex_x;
    int         tex_y;
    t_cf_rays   cf_vars;
} t_rays;

#define NUM_TEXTURES 6
#define NUM_ENEMIES 5

typedef struct s_enemy {
    double  x;
    double  y;
    int     texture;
    double  distance;
    double  transform_y;
    int     visible;
} t_enemy;

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
    t_enemy enemies[NUM_ENEMIES];
    double z_buffer[WIN_WIDTH];
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

int mouse_move(int x, int y, t_data *data)
{
    double  delta_x;

    if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
	{
        data->mouse->last_x = x;
        data->mouse->last_y = y;
        return 0;
    }
    delta_x = x - data->mouse->last_x;
    rotate_view(data, -delta_x * MOUSE_SENS);
    mlx_mouse_move(data->mlx_ptr, data->win_ptr, WIN_WIDTH / 2, WIN_HEIGHT / 2);
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
    load_texture(data, NORTH, "textures/grey_wall.xpm");
    load_texture(data, SOUTH, "textures/grey_wall.xpm");
    load_texture(data, WEST, "textures/grey_wall.xpm");
    load_texture(data, EAST, "textures/grey_wall.xpm");
    load_texture(data, 4, "textures/grey_floor.xpm");
    load_texture(data, 5, "textures/Minotaurfront.xpm");
}

void init_enemies(t_data *data)
{
    data->enemies[0] = (t_enemy){6.5, 6.5, 5, 0, 0, 0};
    data->enemies[1] = (t_enemy){10.5, 8.5, 5, 0, 0, 0};
    data->enemies[2] = (t_enemy){15.5, 10.5, 5, 0, 0, 0};
    data->enemies[3] = (t_enemy){20.5, 15.5, 5, 0, 0, 0};
    data->enemies[4] = (t_enemy){12.5, 12.5, 5, 0, 0, 0};
}


void    put_pixel(t_image *img, int x, int y, int color)
{
    int index;
    
    index = img->line_length * y + x * (img->bits_per_pixel / 8);
    if (img->endian == 1)
    {
        img->data[index + 0] = (color >> 24);
        img->data[index + 1] = (color >> 16) & 0xFF;
        img->data[index + 2] = (color >> 8) & 0xFF;
        img->data[index + 3] = (color) & 0xFF;
    }
    else if (img->endian == 0)
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
    if (keycode == 'w')
        move_forward(data);
    if (keycode == 's')
        move_backward(data);
    if (keycode == 'a')
        strafe_left(data);
    if (keycode == 'd')
        strafe_right(data);
    return 0;
}

int adjust_color_brightness(int color, double factor) {
    int red = ((color >> 16) & 0xFF) * factor;
    int green = ((color >> 8) & 0xFF) * factor;
    int blue = (color & 0xFF) * factor;
    return (red << 16) | (green << 8) | blue;
}

void	init_cf_vars(t_data *data, t_cf_rays *cf_vars)
{
	cf_vars->ray_dir_x0 = data->dir_x - data->plane_x;
    cf_vars->ray_dir_y0 = data->dir_y - data->plane_y;
    cf_vars->ray_dir_x1 = data->dir_x + data->plane_x;
    cf_vars->ray_dir_y1 = data->dir_y + data->plane_y;
    cf_vars->inv_win = 1.0 / WIN_WIDTH;
    cf_vars->texture_size_mask = TEXTURE_SIZE - 1;
}
void    render_ceiling(t_data *data, t_cf_rays *cf_vars)
{
    int x;
    int y;
    double distance_shade_factor;
    
    y = -1;
    while (++y < WIN_HEIGHT / 2)
	{
        cf_vars->row_distance = WIN_HEIGHT / (WIN_HEIGHT - 2.0 * y);
        cf_vars->floor_step_x = cf_vars->row_distance * (cf_vars->ray_dir_x1 - cf_vars->ray_dir_x0) * cf_vars->inv_win;
        cf_vars->floor_step_y = cf_vars->row_distance * (cf_vars->ray_dir_y1 - cf_vars->ray_dir_y0) * cf_vars->inv_win;
        cf_vars->floor_x = data->pos_x + cf_vars->row_distance * cf_vars->ray_dir_x0;
        cf_vars->floor_y = data->pos_y + cf_vars->row_distance * cf_vars->ray_dir_y0;
		x = -1;
        while (++x < WIN_WIDTH)
		{
            cf_vars->cell_x = (int)(cf_vars->floor_x);
            cf_vars->cell_y = (int)(cf_vars->floor_y);
            cf_vars->tx = (int)(TEXTURE_SIZE * (cf_vars->floor_x - cf_vars->cell_x)) & cf_vars->texture_size_mask;
            cf_vars->ty = (int)(TEXTURE_SIZE * (cf_vars->floor_y - cf_vars->cell_y)) & cf_vars->texture_size_mask;
            cf_vars->floor_x += cf_vars->floor_step_x;
            cf_vars->floor_y += cf_vars->floor_step_y;
            cf_vars->color = data->texture_buffer[4][TEXTURE_SIZE * cf_vars->ty + cf_vars->tx];
            distance_shade_factor = 1.0 / (1.0 + cf_vars->row_distance * 0.5);
            cf_vars->color = adjust_color_brightness(cf_vars->color, distance_shade_factor);
            put_pixel(&data->backbuffer, x, y, cf_vars->color);
        }
    }
}

void	render_floor(t_data *data, t_cf_rays *cf_vars)
{
	int	x;
	int	y;
    double distance_shade_factor;

	y = WIN_HEIGHT / 2 - 1;
    while (++y < WIN_HEIGHT)
	{
        cf_vars->row_distance = WIN_HEIGHT / (2.0 * y - WIN_HEIGHT);
        cf_vars->floor_step_x = cf_vars->row_distance * (cf_vars->ray_dir_x1 - cf_vars->ray_dir_x0) * cf_vars->inv_win;
        cf_vars->floor_step_y = cf_vars->row_distance * (cf_vars->ray_dir_y1 - cf_vars->ray_dir_y0) * cf_vars->inv_win;
        cf_vars->floor_x = data->pos_x + cf_vars->row_distance * cf_vars->ray_dir_x0;
        cf_vars->floor_y = data->pos_y + cf_vars->row_distance * cf_vars->ray_dir_y0;
		x = -1;
        while (++x < WIN_WIDTH)
		{
            cf_vars->cell_x = (int)(cf_vars->floor_x);
            cf_vars->cell_y = (int)(cf_vars->floor_y);
            cf_vars->tx = (int)(TEXTURE_SIZE * (cf_vars->floor_x - cf_vars->cell_x)) & cf_vars->texture_size_mask;
            cf_vars->ty = (int)(TEXTURE_SIZE * (cf_vars->floor_y - cf_vars->cell_y)) & cf_vars->texture_size_mask;
            cf_vars->floor_x += cf_vars->floor_step_x;
            cf_vars->floor_y += cf_vars->floor_step_y;
            cf_vars->color = data->texture_buffer[4][TEXTURE_SIZE * cf_vars->ty + cf_vars->tx];
            distance_shade_factor = 1.0 / (1.0 + cf_vars->row_distance * 0.5);
            cf_vars->color = adjust_color_brightness(cf_vars->color, distance_shade_factor);
            put_pixel(&data->backbuffer, x, y, cf_vars->color);
        }
    }
}

void render_cf(t_data *data, t_cf_rays *cf_vars)
{
    init_cf_vars(data, cf_vars);
	render_floor(data, cf_vars);
    render_ceiling(data, cf_vars);
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
    if (vars->side == 0)
        vars->perp_wall_dist = (vars->map_x - data->pos_x + (1 - vars->step_x) / 2) / vars->ray_dir_x;
    else
        vars->perp_wall_dist = (vars->map_y - data->pos_y + (1 - vars->step_y) / 2) / vars->ray_dir_y;
    vars->line_height = (int)(WIN_HEIGHT / vars->perp_wall_dist);
    vars->draw_start = -vars->line_height / 2 + WIN_HEIGHT / 2;
    if (vars->draw_start < 0)
        vars->draw_start = 0;
    vars->draw_end = vars->line_height / 2 + WIN_HEIGHT / 2;
    if (vars->draw_end >= WIN_HEIGHT)
        vars->draw_end = WIN_HEIGHT - 1;
    if (vars->side == 0)
        vars->wall_x = data->pos_y + vars->perp_wall_dist * vars->ray_dir_y;
    else
        vars->wall_x = data->pos_x + vars->perp_wall_dist * vars->ray_dir_x;
}

void select_texture(t_rays *rays, int cell_value)
{
    if (cell_value == 3)
        rays->tex_num = 5;
    else
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

void draw_walls(t_data *data, t_rays *vars, int x) {
    int y;
    int tex_y;
    int color;
    double distance_shade_factor;

    y = vars->draw_start - 1;
    while (++y < vars->draw_end)
	{
        tex_y = (int)vars->tex_pos & (TEXTURE_SIZE - 1);
        vars->tex_pos += vars->step;
        color = data->texture_buffer[vars->tex_num][TEXTURE_SIZE * tex_y + vars->tex_x];
        distance_shade_factor = 1.0 / (1.0 + vars->perp_wall_dist * 0.5);
        color = adjust_color_brightness(color, distance_shade_factor);
        if (vars->side == 1 && color != -16777216)
            color = (color >> 1) & 0x7F7F7F;
        put_pixel(&data->backbuffer, x, y, color);
    }
    data->z_buffer[x] = vars->perp_wall_dist;
}

// Les deux prochaines fonctions ont été faite par chatgpt donc c'est dégeulasse (mais ça marche xD)

int has_line_of_sight(t_data *data, double enemy_x, double enemy_y) {
    double ray_dir_x = enemy_x - data->pos_x;
    double ray_dir_y = enemy_y - data->pos_y;
    double delta_dist_x = fabs(1 / ray_dir_x);
    double delta_dist_y = fabs(1 / ray_dir_y);

    int map_x = (int)data->pos_x;
    int map_y = (int)data->pos_y;

    double side_dist_x;
    double side_dist_y;

    int step_x;
    int step_y;
    int hit = 0;

    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (data->pos_x - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - data->pos_x) * delta_dist_x;
    }

    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (data->pos_y - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - data->pos_y) * delta_dist_y;
    }

    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
        }
        // Vérifiez si le rayon a frappé un mur
        if (data->world_map[map_x][map_y] == 1)
            return 0; // Ligne de vue bloquée
        // Vérifiez si le rayon a atteint l'ennemi
        if (map_x == (int)enemy_x && map_y == (int)enemy_y)
            return 1; // Ligne de vue dégagée
    }
    return 0; // Ligne de vue bloquée (au cas où)
}

void draw_enemies(t_data *data) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        t_enemy *enemy = &data->enemies[i];
        double sprite_x = enemy->x - data->pos_x;
        double sprite_y = enemy->y - data->pos_y;

        if (!has_line_of_sight(data, enemy->x, enemy->y))
			continue ;
        double inv_det = 1.0 / (data->plane_x * data->dir_y - data->dir_x * data->plane_y);
        double transform_x = inv_det * (data->dir_y * sprite_x - data->dir_x * sprite_y);
        double transform_y = inv_det * (-data->plane_y * sprite_x + data->plane_x * sprite_y);

        enemy->transform_y = transform_y;
        int sprite_screen_x = (int)((WIN_WIDTH / 2) * (1 + transform_x / transform_y));

        int sprite_height = abs((int)(WIN_HEIGHT / transform_y));
        int draw_start_y = -sprite_height / 2 + WIN_HEIGHT / 2;
        if (draw_start_y < 0) draw_start_y = 0;
        int draw_end_y = sprite_height / 2 + WIN_HEIGHT / 2;
        if (draw_end_y >= WIN_HEIGHT) draw_end_y = WIN_HEIGHT - 1;

        int sprite_width = abs((int)(WIN_HEIGHT / transform_y));
        int draw_start_x = -sprite_width / 2 + sprite_screen_x;
        if (draw_start_x < 0) draw_start_x = 0;
        int draw_end_x = sprite_width / 2 + sprite_screen_x;
        if (draw_end_x >= WIN_WIDTH) draw_end_x = WIN_WIDTH - 1;

        // Calcul du facteur d'ombrage basé sur la distance de l'ennemi
        double distance_shade_factor = 1.0 / (1.0 + enemy->transform_y * 0.5);

        // Dessiner l'ennemi
        for (int stripe = draw_start_x; stripe < draw_end_x; stripe++) {
            int tex_x = (int)((256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) * TEXTURE_SIZE / sprite_width) / 256);
            if (transform_y > 0 && stripe > 0 && stripe < WIN_WIDTH && transform_y < data->z_buffer[stripe]) {
                for (int y = draw_start_y; y < draw_end_y; y++) {
                    int d = y * 256 - WIN_HEIGHT * 128 + sprite_height * 128;
                    int tex_y = ((d * TEXTURE_SIZE) / sprite_height) / 256;
                    int color = data->texture_buffer[enemy->texture][TEXTURE_SIZE * tex_y + tex_x];
                    // Appliquer l'effet d'ombrage
                    color = adjust_color_brightness(color, distance_shade_factor);
                    if ((color & 0x00FFFFFF) != 0) { // Vérifier que le pixel n'est pas transparent
                        put_pixel(&data->backbuffer, stripe, y, color);
                    }
                }
            }
        }
    }
}

double enemies_dir_x(t_data *data, int i)
{
    if (data->enemies[i].x - data->pos_x > 0)
        return (-0.1);
    else if (data->enemies[i].x - data->pos_x < 0)
        return (0.1);
    return (0.0);
}

double enemies_dir_y(t_data *data, int i)
{
    if (data->enemies[i].y - data->pos_y > 0)
        return (-0.1);
    else if (data->enemies[i].y - data->pos_y < 0)
        return (0.1);
    return (0.0);
}

int    enemies_mouvs(t_data *data)
{
    int i;

    i = -1;
    while (++i < NUM_ENEMIES)
    {
        data->enemies[i].x += enemies_dir_x(data, i);
        data->enemies[i].y += enemies_dir_y(data, i);
    }
    return (0);
}

int render_frame(t_data *data) {
    t_rays  vars;
    int     x;
    
    render_cf(data, &vars.cf_vars);
    x = -1;
    while (++x < WIN_WIDTH)
    {
        calcul_delta_dist(data, &vars, x);
        calcul_side_dist(data, &vars);
        calcul_wall_pos(data, &vars);
        calcul_wall_size(data, &vars);
        select_texture(&vars, data->world_map[vars.map_x][vars.map_y]);
        handle_textures(&vars);
        draw_walls(data, &vars, x);
    }
    //update_enemy_visibility(data);
    draw_enemies(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->backbuffer.addr, 0, 0);
    return 0;
}


// Pour le moment ça sert pas à grand chose j'ai rajouter ça pour le déplacement des ennemies pas utile mais ça pourra servir pour plus tard
int combined_hook(t_data *data)
{
    static clock_t last_enemy_update = 0;
    clock_t current_time = clock();

    // Render frame
    render_frame(data);

    // Update enemies every 100ms
    if ((current_time - last_enemy_update) * 1000 / CLOCKS_PER_SEC >= 100)
    {
        enemies_mouvs(data);
        last_enemy_update = current_time;
    }

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
    init_enemies(&data);
	init_imgs(&data, &data.backbuffer, WIN_WIDTH, WIN_HEIGHT);
    mlx_hook(data.win_ptr, 2, 1L << 0, key_press, &data);
    mlx_hook(data.win_ptr, 6, 1L << 6, mouse_move, &data);
<<<<<<< HEAD
    //mlx_loop_hook(data.mlx_ptr, &enemies_mouvs, &data);
    mlx_loop_hook(data.mlx_ptr, combined_hook, &data);
    /*mlx_mouse_move(data.mlx_ptr, data.win_ptr, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    mlx_mouse_hide(data.mlx_ptr, data.win_ptr);*/
=======
    mlx_loop_hook(data.mlx_ptr, &render_frame, &data);
    mlx_mouse_hide(data.mlx_ptr, data.win_ptr);
>>>>>>> refs/remotes/origin/testrender
    mlx_loop(data.mlx_ptr);
    return 0;
}