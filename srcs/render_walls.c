/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 19:12:05 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/17 19:12:05 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "funct.h"
#include "incl.h"

void	calcul_wall_pos(t_data *data, t_rays *vars)
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
		if (data->map.content[vars->map_x][vars->map_y] == '1')
			break ;
	}
}

void	calcul_wall_size(t_data *data, t_rays *vars)
{
	double	perp_wall_dist;

	if (vars->side == 0)
		perp_wall_dist = (vars->map_x - data->pos_x
				+ (1 - vars->step_x) / 2) / vars->ray_dir_x;
	else
		perp_wall_dist = (vars->map_y - data->pos_y
				+ (1 - vars->step_y) / 2) / vars->ray_dir_y;
	vars->line_height = (int)(WIN_HEIGHT / perp_wall_dist);
	vars->draw_start = -vars->line_height / 2 + WIN_HEIGHT / 2;
	if (vars->draw_start < 0)
		vars->draw_start = 0;
	vars->draw_end = vars->line_height / 2 + WIN_HEIGHT / 2;
	if (vars->draw_end >= WIN_HEIGHT)
		vars->draw_end = WIN_HEIGHT - 1;
	if (vars->side == 0)
		vars->wall_x = data->pos_y + perp_wall_dist
			* vars->ray_dir_y;
	else
		vars->wall_x = data->pos_x + perp_wall_dist
			* vars->ray_dir_x;
}

void	draw_walls(t_data *data, t_rays *vars, int x)
{
	int	y;
	int	tex_y;
	int	color;

	y = vars->draw_start - 1;
	while (++y < vars->draw_end)
	{
		tex_y = (int)vars->tex_pos & (TEXTURE_SIZE - 1);
		vars->tex_pos += vars->step;
		color = data->texture_buffer[vars->tex_num][TEXTURE_SIZE
			* tex_y + vars->tex_x];
		if (vars->side == 1)
			color = (color >> 1) & 0x7F7F7F;
		put_pixel(&data->backbuffer, x, y, color);
	}
}
