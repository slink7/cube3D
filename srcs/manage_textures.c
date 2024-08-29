/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 18:49:55 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/17 18:49:55 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "funct.h"
#include "incl.h"

void	load_texture(t_data *data, int index, char *path)
{
	t_image	img;
	int		x;
	int		y;
	int		i;
	int		j;

	img.addr = mlx_xpm_file_to_image(data->mlx_ptr, path, &x, &y);
	if (!img.addr || x != TEXTURE_SIZE || y != TEXTURE_SIZE)
		return ;
	img.data = (int *)mlx_get_data_addr(img.addr,
			&img.bits_per_pixel, &img.line_length, &img.endian);
	data->texture_buffer[index] = malloc(sizeof(int)
			* TEXTURE_SIZE * TEXTURE_SIZE);
	i = -1;
	while (++i < TEXTURE_SIZE)
	{
		j = -1;
		while (++j < TEXTURE_SIZE)
			data->texture_buffer[index][i
				* TEXTURE_SIZE + j] = img.data[i * TEXTURE_SIZE + j];
	}
	mlx_destroy_image(data->mlx_ptr, img.addr);
}

void	destroy_textures(t_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		free(data->texture_buffer[i]);
		i++;
	}
}

void	init_textures(t_data *data)
{
	load_texture(data, NORTH, data->map.wall_textures[NORTH].path);
	load_texture(data, SOUTH, data->map.wall_textures[SOUTH].path);
	load_texture(data, WEST, data->map.wall_textures[WEST].path);
	load_texture(data, EAST, data->map.wall_textures[EAST].path);
}

void	select_texture(t_rays *rays)
{
	if (rays->side == 0)
	{
		if (rays->ray_dir_x > 0)
			rays->tex_num = SOUTH;
		else
			rays->tex_num = NORTH;
	}
	else
	{
		if (rays->ray_dir_y > 0)
			rays->tex_num = EAST;
		else
			rays->tex_num = WEST;
	}
}

void	handle_textures(t_rays *vars)
{
	vars->wall_x -= floor(vars->wall_x);
	vars->tex_x = (int)(vars->wall_x * (double)TEXTURE_SIZE);
	if ((vars->side == 0 && vars->ray_dir_x > 0)
		|| (vars->side == 1 && vars->ray_dir_y < 0))
		vars->tex_x = TEXTURE_SIZE - vars->tex_x - 1;
	vars->step = 1.0 * TEXTURE_SIZE / vars->line_height;
	vars->tex_pos = (vars->draw_start - WIN_HEIGHT
			/ 2 + vars->line_height / 2) * vars->step;
}
