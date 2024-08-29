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
#include "parsing.h"

int	free_all(t_data *data)
{
	unload_map(&data->map);
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	mlx_destroy_image(data->mlx_ptr, data->backbuffer.addr);
	mlx_destroy_display(data->mlx_ptr);
	free(data->mlx_ptr);
	return (0);
}

int	close_game(t_data *data)
{
	destroy_textures(data, 4);
	free_all(data);
	exit (EXIT_FAILURE);
}

// if (data->inputs & IN_ESC)
// 	mlx_mouse_show(data->mlx_ptr, data->win_ptr);
// else
// 	mlx_mouse_hide(data->mlx_ptr, data->win_ptr);

int	on_keyup(int keycode, t_data *data)
{
	if (keycode == XK_Escape)
	{
		data->inputs ^= IN_ESC;
	}
	else if (keycode == 'w')
		data->inputs ^= data->inputs & IN_Z;
	else if (keycode == 'a')
		data->inputs ^= data->inputs & IN_Q;
	else if (keycode == 's')
		data->inputs ^= data->inputs & IN_S;
	else if (keycode == 'd')
		data->inputs ^= data->inputs & IN_D;
	return (0);
}

int	on_keydown(int keycode, t_data *data)
{
	if (keycode == 'w')
		data->inputs |= IN_Z;
	else if (keycode == 'a')
		data->inputs |= IN_Q;
	else if (keycode == 's')
		data->inputs |= IN_S;
	else if (keycode == 'd')
		data->inputs |= IN_D;
	else if (keycode == 'q')
		close_game(data);
	return (0);
}
