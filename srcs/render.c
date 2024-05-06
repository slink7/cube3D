/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:56 by ymostows          #+#    #+#             */
/*   Updated: 2024/05/06 17:39:56 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl.h"
#include "structs.h"
#include <math.h>


int move_player2(int keysym, t_world *world)
{
    if (keysym == KEY_UP)
    {
        int y = roundf(world->player.y) - 1;
        int x = roundf(world->player.x);
        if (world->map.content[y][x] != MAP_WALL)
            world->player.y--;
        return 1;
    }
    if (keysym == KEY_LEFT)
    {
        int y = roundf(world->player.y);
        int x = roundf(world->player.x) - 1;
        if (world->map.content[y][x] != MAP_WALL)
            world->player.x--;
        return 1;
    }
    return (0);
}

int move_player(int keysym, t_world *world)
{
    if (keysym == KEY_DOWN)
    {
        int y = roundf(world->player.y) + 1;
        int x = roundf(world->player.x);
        if (world->map.content[y][x] != MAP_WALL)
            world->player.y++;
        return 1;
    }
    if (keysym == KEY_RIGHT)
    {
        int y = roundf(world->player.y);
        int x = roundf(world->player.x) + 1;
        if (world->map.content[y][x] != MAP_WALL)
            world->player.x++;
        return 1;
    }
    return move_player2(keysym, world);
}

int	close_game(t_world *world)
{
    (void)world;
	exit (EXIT_FAILURE);
}

int draw_square(t_image *img, int x, int y, int size, int color)
{
    int i;
    int j;
    int index;

    //printf("bpp:%d\n", img->bits_per_pixel);
    j = y;
    while (j < y + size)
    {
        i = x;
        while (i < x + size)
        {
            index = img->line_length * j + i * (img->bits_per_pixel / 8);
            img->data[index] = color;
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
            i++;
        }
        j++;
    }
    return (0);
}

void init_img(t_world *world,t_image *img, int width, int heigth)
{
    img->addr = mlx_new_image(world->mlx_ptr, width, heigth);
    img->data = mlx_get_data_addr(img->addr, &img->bits_per_pixel, &img->line_length, &img->endian);
}


int init_backbuffer(t_world *world)
{
    int i;
    int j;
    int height;
    int width;
    int color;

    height = 512 / world->map.height;
    width = 512 / world->map.width;
    init_img(world, &world->backbuffer, 1024, 512);
    j = 0;
    while (j < world->map.height)
    {
        i = 0;
        while (i < world->map.width)
        {
            if (world->map.content[j][i] == MAP_WALL)
                color = WHITE;
            else if (world->map.content[j][i] == MAP_FLOOR)
                color = GREY;
            else
                color = 0;
            printf("i:%d j:%d\n", i, width);
            draw_square(&world->backbuffer, i * width + 1, j * height + 1, width - 2, color);
            i++;
        }
        j++;
    }
    return (0);
}

void draw_backbuffer(t_world *world)
{
    mlx_put_image_to_window(world->mlx_ptr, world->win_ptr, world->backbuffer.addr, 0, 0);
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
                { 1, 2, 2, 2, 2, 2, 1 },
                { 1, 2, 2, 2, 1, 1, 1 },
                { 1, 2, 2, 2, 1, 0, 0 },
                { 1, 1, 1, 1, 1, 0, 0 },
            },
            7,
            7,
            {0,0,0,0,0},
            0xFF000000,
            0x00FFFF00
        },
        {
            2.0f,
            2.0f,
            0.0f
        },
        {0}
    };
    /*mlx_hook(world.win_ptr, KeyPress, KeyPressMask,
			&ft_move_player, s_game);*/
	mlx_hook(world.win_ptr, DestroyNotify, \
		ButtonPressMask, close_game, &world);
    /*mlx_hook(world.win_ptr, KeyPress, KeyPressMask,
			&move_player, &world);*/
	//mlx_loop_hook(world.mlx_ptr, &draw_player, &world);
    //draw_player(world.player, line_bytes, endian, color, buffer);
    init_backbuffer(&world);
    draw_backbuffer(&world);
    mlx_loop(world.mlx_ptr);
}