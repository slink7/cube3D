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

void	print_map(t_map *map)
{
	int	k;

	ft_printf("FLOOR   COLOR : %X\n", map->floor_color);
	ft_printf("CEILING COLOR : %X\n", map->ceiling_color);
	ft_printf("MAP DIMENSION : %d * %d\n", map->width, map->height);
	ft_printf("MAP LAYOUT :\n");
	k = -1;
	while (map->content[++k])
	{
		ft_printf("%s.\n", map->content[k]);
	}
	ft_printf("%d lines\n", k);
}

void	init_game(t_data *data, t_mouse *mouse)
{
	init_player(data);
	data->mlx_ptr = mlx_init();
	data->win_ptr = mlx_new_window(
			data->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "Cub3D");
	mouse->last_x = WIN_WIDTH / 2;
	mouse->last_y = WIN_HEIGHT / 2;
	data->mouse = mouse;
	init_textures(data);
	init_imgs(data, &data->backbuffer, WIN_WIDTH, WIN_HEIGHT);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_mouse	mouse;

	if (argc < 2)
		return (0);
	ft_memset(&data, 0, sizeof(t_data));
	if (load_map(argv[1], &data.map))
	{
		if (!check_walls(&data.map))
			ft_printf("Invalid walls\n");
	}
	init_game(&data, &mouse);
	mlx_hook(data.win_ptr, 2, 1L << 0, on_keydown, &data);
	mlx_hook(data.win_ptr, 3, 1L << 1, on_keyup, &data);
	mlx_hook(data.win_ptr, 6, 1L << 6, mouse_move, &data);
	mlx_hook(data.win_ptr, DestroyNotify, \
		ButtonPressMask, close_game, &data);
	mlx_loop_hook(data.mlx_ptr, &render_frame, &data);
	/*mlx_mouse_move(data.mlx_ptr, data.win_ptr, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	mlx_mouse_hide(data.mlx_ptr, data.win_ptr);*/
	mlx_loop(data.mlx_ptr);

	
	return (0);
}
