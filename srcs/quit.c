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

int	key_press(int keycode, t_data *data)
{
	if (keycode == 'z')
		move_forward(data);
	if (keycode == 's')
		move_backward(data);
	if (keycode == 'q')
		strafe_left(data);
	if (keycode == 'd')
		strafe_right(data);
	if (keycode == 'a')
		close_game(data);
	return (0);
}
