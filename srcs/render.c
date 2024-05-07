/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:56 by ymostows          #+#    #+#             */
/*   Updated: 2024/05/07 19:14:33 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl.h"
#include "structs.h"
#include <math.h>


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

static void	put_line_v(t_image *img, t_vec2i *v0, t_vec2i *v1, int color)
{
	float	a;
	float	b;
	int		k;

	if (v0->x < 0 || v0->x > img->width || v0->y < 0 || v0->y > img->height)
		if (v1->x < 0 || v1->x > img->width || v1->y < 0 || v1->y > img->height)
			return ;
	a = (float)(v1->x - v0->x) / (float)(v1->y - v0->y);
	b = -a * (float)v0->y + (float)v0->x;
	k = ft_min(v0->y, v1->y) - 1;
	while (++k <= ft_max(v0->y, v1->y))
		put_pixel(img, a * (float)k + b, k, color);
}

static void	put_line_h(t_image *img, t_vec2i *v0, t_vec2i *v1, int color)
{
	float	a;
	float	b;
	int		k;

	if (v0->x < 0 || v0->x > img->width || v0->y < 0 || v0->y > img->height)
		if (v1->x < 0 || v1->x > img->width || v1->y < 0 || v1->y > img->height)
			return ;
	a = (float)(v1->y - v0->y) / (float)(v1->x - v0->x);
	b = -a * (float)v0->x + (float) v0->y;
	k = ft_min(v0->x, v1->x) - 1;
	while (++k <= ft_max(v0->x, v1->x))
		put_pixel(img, k, a * (float)k + b, color);
}

void	put_line(t_image *img, t_vec2i v0, t_vec2i v1, int color)
{
	if (ft_abs(v1.x - v0.x) > ft_abs(v1.y - v0.y))
		put_line_h(img, &v0, &v1, color);
	else
		put_line_v(img, &v0, &v1, color);
}

void draw_3d_walls(t_world *world, int px, int py)
{
    float ray_angle;
    float rx, ry;
    float xo, yo;
    float xstep, ystep;
    int mx, my;
    int hit_wall;
    float distance;
    int r;

    r = 512 - 1;
    while (++r < 1024)
    {
        ray_angle = world->player.z - (PI / 12) + ((r - 512) * 0.0015);
        
        xo = px;
        yo = py;
        xstep = cos(ray_angle) * 0.1f;
        ystep = sin(ray_angle) * 0.1f;
        hit_wall = 0;

        while (!hit_wall)
        {
            rx = xo + xstep;
            ry = yo + ystep;
            mx = (int)rx / TILE_SIZE;
            my = (int)ry / TILE_SIZE;
            if (mx >= 0 && mx < world->map.width && my >= 0 && my < world->map.height && world->map.content[my][mx] == MAP_WALL)
            {
                hit_wall = 1;
                distance = sqrt((rx - px) * (rx - px) + (ry - py) * (ry - py));
                int wall_height = (int)(512.0f / distance * 70);
                wall_height = (int)((float)wall_height / cos(ray_angle - world->player.z));
                int wall_top = 256 - wall_height / 2;
                int wall_bottom = 256 + wall_height / 2;
                if (r  == 512)
                    printf("wt:%d wb:%d\n", wall_top, wall_bottom);
                if (wall_top < 0)
                    wall_top = 1;
                if (wall_bottom > 512)
                    wall_bottom = 512;
                if ((int)rx % 73 == 0)
                    put_line(&world->backbuffer, (t_vec2i){r, wall_top}, (t_vec2i){r, wall_bottom}, 0x00AA00);
                else
                    put_line(&world->backbuffer, (t_vec2i){r, wall_top}, (t_vec2i){r, wall_bottom}, 0x00A000); // Dessiner le mur décalé
                put_line(&world->backbuffer, (t_vec2i){r, wall_top}, (t_vec2i){r, 0}, 0x03A9FC);
                put_line(&world->backbuffer, (t_vec2i){r, wall_bottom}, (t_vec2i){r, 512}, 0xAAFFAA);

            }
            else
            {
                xo = rx;
                yo = ry;
            }
        }
    }
}






void draw_rays(t_world *world, int px, int py)
{
    float ray_angle;
    float rx, ry;
    float xo, yo;
    float xstep, ystep;
    int mx, my;
    int hit_wall;
    int r;

    ray_angle = world->player.z - (PI / 12);

    r = 0 - 1;
    while (++r < 512)
    {
        xo = px;
        yo = py;
        xstep = cos(ray_angle) * 0.1f;
        ystep = sin(ray_angle) * 0.1f;
        hit_wall = 0;
        while (!hit_wall)
        {
            rx = xo + xstep;
            ry = yo + ystep;
            mx = (int)rx / TILE_SIZE;
            my = (int)ry / TILE_SIZE;

            if (mx >= 0 && mx < world->map.width && my >= 0 && my < world->map.height && world->map.content[my][mx] == MAP_WALL)
            {
                hit_wall = 1;
                //printf("rx:%f ry:%f mx:%d my:%d\n", rx, ry, mx * TILE_SIZE, my * TILE_SIZE);
                put_line(&world->backbuffer, (t_vec2i){px, py}, (t_vec2i){(int)rx, (int)ry}, 0xFF0000);
            }
            else
            {
                xo = rx;
                yo = ry;
            }
        }
        ray_angle += 0.0015;
    }
}

int move_player2(int keysym, t_world *world)
{
    if (keysym == KEY_UP)
    {
        world->player.x += cos(world->player.z) * 0.1f;
        world->player.y += sin(world->player.z) * 0.1f;
        return (1);
    }
    if (keysym == KEY_LEFT)
    {
        world->player.x += sin(world->player.z) * 0.1f;
        world->player.y -= cos(world->player.z) * 0.1f;
        return (1);
    }
    if (keysym == XK_Left)
    {
        world->player.z -=0.1f;
        return (1);
    }
    if (keysym == XK_Right)
    {
        world->player.z +=0.1f;
        return (1);
    }
    return (0);
}

int move_player(int keysym, t_world *world)
{
    if (keysym == KEY_DOWN)
    {
        world->player.x -= cos(world->player.z) * 0.1f;
        world->player.y -= sin(world->player.z) * 0.1f;
        return (1);
    }
    if (keysym == KEY_RIGHT)
    {
        world->player.x -= sin(world->player.z) * 0.1f;
        world->player.y += cos(world->player.z) * 0.1f;
        return (1);
    }
    return move_player2(keysym, world);
}

int	close_game(t_world *world)
{
    (void)world;
	exit (EXIT_FAILURE);
}

void draw_square(t_image *img, int x, int y, int size, int color)
{
    int i;
    int j;

    j = y - 1;
    while (++j < y + size)
    {
        i = x - 1;
        while (++i < x + size)
            put_pixel(img, i, j, color);
    }
}

void init_img(t_world *world,t_image *img, int width, int heigth)
{
    img->addr = mlx_new_image(world->mlx_ptr, width, heigth);
    img->data = mlx_get_data_addr(img->addr, &img->bits_per_pixel, &img->line_length, &img->endian);
    img->width = width;
    img->height = heigth;
}


int init_minimap(t_world *world)
{
    int i;
    int j;
    int height;
    int width;
    int color;

    height = 512 / world->map.height;
    width = 512 / world->map.width;
    init_img(world, &world->minimap, 1024, 512);
    j = -1;
    while (++j < world->map.height)
    {
        i = -1;
        while (++i < world->map.width)
        {
            if (world->map.content[j][i] == MAP_WALL)
                color = WHITE;
            else if (world->map.content[j][i] == MAP_FLOOR)
                color = GREY;
            else
                color = 0;
            draw_square(&world->minimap, i * width + 1, j * height + 1, width - 2, color);
        }
    }
    return (0);
}

void    cpy_pixel(t_image *dest, t_image *src, int dest_i, int src_i)
{   
    if (dest->endian == src->endian)
    {
        dest->data[dest_i + 0] = src->data[src_i + 0];
        dest->data[dest_i + 1] = src->data[src_i + 1];
        dest->data[dest_i + 2] = src->data[src_i + 2];
        dest->data[dest_i + 3] = src->data[src_i + 3];
    }
    else
    {
        dest->data[dest_i + 0] = src->data[src_i + 3];
        dest->data[dest_i + 1] = src->data[src_i + 2];
        dest->data[dest_i + 2] = src->data[src_i + 1];
        dest->data[dest_i + 3] = src->data[src_i + 0];
    }
}

void    cpy_img(t_image *dest, t_image *src, int x, int y)
{
    int i;
    int j;
    int dest_i;
    int src_i;

    j = y - 1;
    while (++j < src->height)
    {
        i = x - 1;
        while (++i < src->line_length)
        {
            dest_i = dest->line_length * (y + j) + (x + i) * (dest->bits_per_pixel / 8);
            src_i = src->line_length * j + i * (src->bits_per_pixel / 8);
            cpy_pixel(dest, src, dest_i, src_i);
        }
    }
}

int render(t_world *world)
{
    int px;
    int py;

    px = world->player.x * (512 / world->map.width);
    py = world->player.y * (512 / world->map.height);
    cpy_img(&world->backbuffer, &world->minimap, 0, 0);
    draw_square(&world->backbuffer, px - 4, py - 4, 8, 0x00FF00);
    put_line(&world->backbuffer, (t_vec2i){px , py}, (t_vec2i){px + cos(world->player.z) * 16.0f, py + sin(world->player.z) * 16.0f}, 0x00FF00);
    draw_rays(world, px, py);
    draw_3d_walls(world, px, py);
    mlx_put_image_to_window(world->mlx_ptr, world->win_ptr, world->backbuffer.addr, 0, 0);
    return (0);
}

int main()
{
    t_world world = {
        mlx_init(),
        mlx_new_window(world.mlx_ptr, 1024, 512, "cube3d"),
        {
            {
                { 0, 0, 0, 0, 1, 1, 1 },
                { 0, 1, 1, 1, 1, 2, 1 },
                { 1, 1, 2, 2, 2, 2, 1 },
                { 1, 2, 2, 1, 2, 2, 1 },
                { 1, 2, 2, 2, 2, 1, 1 },
                { 1, 2, 2, 2, 1, 1, 0 },
                { 1, 1, 1, 1, 1, 0, 0 },
            },
            7,
            7,
            {0,0,0,0,0,0,0},
            0xFF000000,
            0x00FFFF00
        },
        {
            2.0f,
            2.0f,
            0.0f
        },
        {0},
        {0}
    };
    init_img(&world, &world.backbuffer, 1024, 512);
    /*mlx_hook(world.win_ptr, KeyPress, KeyPressMask,
			&ft_move_player, s_game);*/
	mlx_hook(world.win_ptr, DestroyNotify, \
		ButtonPressMask, close_game, &world);
    mlx_hook(world.win_ptr, KeyPress, KeyPressMask,
			&move_player, &world);
	mlx_loop_hook(world.mlx_ptr, &render, &world);
    //draw_player(world.player, line_bytes, endian, color, buffer);
    init_minimap(&world);
    mlx_loop(world.mlx_ptr);
}