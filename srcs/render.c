/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:56 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/26 19:23:44 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "incl.h"
#include "funct.h"

void	render_floor_ceiling(t_data *data)
{
	int		x;
	int		y;

	y = -1;
	while (++y < WIN_HEIGHT / 2)
	{
		x = -1;
		while (++x < WIN_WIDTH)
			put_pixel(&data->backbuffer, x, y, data->map.floor_color);
	}
	y = WIN_HEIGHT / 2 -1;
	while (++y < WIN_HEIGHT)
	{
		x = -1;
		while (++x < WIN_WIDTH)
			put_pixel(&data->backbuffer, x, y, data->map.ceiling_color);
	}
}

int	render_frame(t_data *data)
{
	t_rays	vars;
	int		x;

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
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr,
		data->backbuffer.addr, 0, 0);
	return (0);
}
