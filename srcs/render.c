/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:56 by ymostows          #+#    #+#             */
/*   Updated: 2024/05/03 16:49:30 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl.h"

int check_wall(t_map map, t_vec2f pos)
{
    if (map.content[round(pos.y)][round(pos.x)])
}

t_vec2f cast_ray(t_world world)
{
    int		x;
    float	dirX;
    float	dirY;
    float	camX;

    dirX = cos(world.player.rotation);
    dirY = sin(world.player.rotation);
    camX = 



}

int main()
{
    t_world world = {
        {
            {
                { 0, 0, 0, 0, 1, 1, 1 },
                { 0, 1, 1, 1, 1, 2, 1 },
                { 1, 1, 2, 2, 2, 2, 1 },
                { 1, 0, 0, 0, 0, 0, 1 },
                { 1, 0, 0, 0, 1, 1, 1 },
                { 1, 0, 0, 0, 1, 0, 0 },
                { 1, 1, 1, 1, 1, 0, 0 },
            },
            7,
            7,
            {0,0,0,0},
            0xFF000000,
            0x00FFFF00
        },
        {
            {2.0f, 2.0f},
            0.0f
        }
    };

    t_mlx ptrs;

    ptrs.mlx_ptr = mlx_init();
    ptrs.win_ptr = mlx_new_window(ptrs.mlx_ptr, 1024, 1024, "cube3d");
    
}