/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:02:38 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/26 16:02:38 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "incl.h"
#include "funct.h"
#include "parsing.h"

int	init_game(t_data *data, t_mouse *mouse)
{
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (0);
	data->win_ptr = mlx_new_window(
			data->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "Cub3D");
	if (!data->win_ptr)
		return (0);
	mouse->last_x = WIN_WIDTH / 2;
	mouse->last_y = WIN_HEIGHT / 2;
	data->mouse = mouse;
	init_imgs(data, &data->backbuffer, WIN_WIDTH, WIN_HEIGHT);
	if (!init_textures(data))
		return (ft_fprintf(2, "XPM file error.\n") & 0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_mouse	mouse;

	if (argc < 2)
		return (0);
	ft_memset(&data, 0, sizeof(t_data));
	if (load_map(argv[1], &data.map) && !check_walls(&data.map))
		return (0 & unload_map(&data.map) & ft_printf("Invalid walls\n"));
	if (!init_player(&data))
		return (0 & unload_map(&data.map));
	if (!init_game(&data, &mouse))
		return (0 & free_all(&data));
	mlx_hook(data.win_ptr, 2, 1L << 0, on_keydown, &data);
	mlx_hook(data.win_ptr, 3, 1L << 1, on_keyup, &data);
	mlx_hook(data.win_ptr, 6, 1L << 6, mouse_move, &data);
	mlx_hook(data.win_ptr, DestroyNotify, \
		ButtonPressMask, close_game, &data);
	mlx_loop_hook(data.mlx_ptr, &render_frame, &data);
	mlx_loop(data.mlx_ptr);
	return (0);
}
