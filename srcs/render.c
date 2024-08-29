/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:56 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/29 13:49:50 by scambier         ###   ########.fr       */
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

void	handle_movement(t_data *data)
{
	if (data->inputs & IN_Z)
		move_forward(data);
	if (data->inputs & IN_Q)
		strafe_left(data);
	if (data->inputs & IN_S)
		move_backward(data);
	if (data->inputs & IN_D)
		strafe_right(data);
	// if (data->inputs & IN_ESC)
	// 	mlx_mouse_move(data->mlx_ptr, data->win_ptr,
	// 		WIN_WIDTH / 2, WIN_HEIGHT / 2);	
}

int	render_frame(t_data *data)
{
	t_rays	vars;
	int		x;

	handle_movement(data);
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
