/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:06:46 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/17 19:06:46 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "funct.h"
#include "incl.h"

void	calcul_delta_dist(t_data *data, t_rays *vars, int x)
{
	vars->camera_x = 2 * x / (double)WIN_WIDTH - 1;
	vars->ray_dir_x = data->dir_x + data->plane_x * vars->camera_x;
	vars->ray_dir_y = data->dir_y + data->plane_y * vars->camera_x;
	vars->map_x = (int)data->pos_x;
	vars->map_y = (int)data->pos_y;
	vars->delta_dist_x = fabs(1 / vars->ray_dir_x);
	vars->delta_dist_y = fabs(1 / vars->ray_dir_y);
}

void	calcul_side_dist(t_data *data, t_rays *vars)
{
	if (vars->ray_dir_x < 0)
	{
		vars->step_x = -1;
		vars->side_dist_x = (data->pos_x - vars->map_x) * vars->delta_dist_x;
	}
	else
	{
		vars->step_x = 1;
		vars->side_dist_x = (vars->map_x + 1.0 - data->pos_x)
			* vars->delta_dist_x;
	}
	if (vars->ray_dir_y < 0)
	{
		vars->step_y = -1;
		vars->side_dist_y = (data->pos_y - vars->map_y) * vars->delta_dist_y;
	}
	else
	{
		vars->step_y = 1;
		vars->side_dist_y = (vars->map_y + 1.0 - data->pos_y)
			* vars->delta_dist_y;
	}
}
