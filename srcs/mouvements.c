/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouvements.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 18:15:11 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/17 18:15:11 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "funct.h"
#include "incl.h"

void	move_forward(t_data *data)
{
	if (data->map.content[(int)(data->pos_x + data->dir_x
			* MOVE_SPEED)][(int)(data->pos_y)] != '1')
		data->pos_x += data->dir_x * MOVE_SPEED;
	if (data->map.content[(int)(data->pos_x)][(int)(data->pos_y + data->dir_y
			* MOVE_SPEED)] != '1')
		data->pos_y += data->dir_y * MOVE_SPEED;
}

void	move_backward(t_data *data)
{
	if (data->map.content[(int)(data->pos_x - data->dir_x
			* MOVE_SPEED)][(int)(data->pos_y)] != '1')
		data->pos_x -= data->dir_x * MOVE_SPEED;
	if (data->map.content[(int)(data->pos_x)][(int)(data->pos_y - data->dir_y
			* MOVE_SPEED)] != '1')
		data->pos_y -= data->dir_y * MOVE_SPEED;
}

void	strafe_right(t_data *data)
{
	double	strafe_x;
	double	strafe_y;

	strafe_x = data->dir_y * MOVE_SPEED;
	strafe_y = -data->dir_x * MOVE_SPEED;
	if (data->map.content[(int)(data->pos_x
			+ strafe_x)][(int)(data->pos_y)] != '1')
		data->pos_x += strafe_x;
	if (data->map.content[(int)(data->pos_x)][(int)(data->pos_y
			+ strafe_y)] != '1')
		data->pos_y += strafe_y;
}

void	strafe_left(t_data *data)
{
	double	strafe_x;
	double	strafe_y;

	strafe_x = -data->dir_y * MOVE_SPEED;
	strafe_y = data->dir_x * MOVE_SPEED;
	if (data->map.content[(int)(data->pos_x
			+ strafe_x)][(int)(data->pos_y)] != '1')
		data->pos_x += strafe_x;
	if (data->map.content[(int)(data->pos_x)][(int)(data->pos_y
			+ strafe_y)] != '1')
		data->pos_y += strafe_y;
}
