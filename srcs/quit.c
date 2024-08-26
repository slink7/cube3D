/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:05:09 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/21 15:05:09 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "funct.h"
#include "incl.h"

int	free_all(t_data *data)
{
	free(data->map.wall_textures[0].path);
	free(data->map.wall_textures[1].path);
	free(data->map.wall_textures[2].path);
	free(data->map.wall_textures[3].path);
	for (int k = 0; k < data->map.height; k++)
		free(data->map.content[k]);
	free(data->map.content);
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	mlx_destroy_image(data->mlx_ptr, data->backbuffer.addr);
	mlx_destroy_display(data->mlx_ptr);
	free(data->mlx_ptr);
	return (0);
}

int	close_game(t_data *data)
{
	destroy_textures(data);
	free_all(data);
	exit (EXIT_FAILURE);
}

int on_keyup(int keycode, t_data *data)
{
	if (keycode == 'z')
		data->inputs ^= data->inputs & IN_Z;
	else if (keycode == 'q')
		data->inputs ^= data->inputs & IN_Q;
	else if (keycode == 's')
		data->inputs ^= data->inputs & IN_S;
	else if (keycode == 'd')
		data->inputs ^= data->inputs & IN_D;
	return (0);
}

int	on_keydown(int keycode, t_data *data)
{
	if (keycode == 'z')
		data->inputs |= IN_Z;
	else if (keycode == 'q')
		data->inputs |= IN_Q;
	else if (keycode == 's')
		data->inputs |= IN_S;
	else if (keycode == 'd')
		data->inputs |= IN_D;
	else if (keycode == 'a')
		close_game(data);
	return (0);
}
