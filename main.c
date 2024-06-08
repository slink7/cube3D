/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:34:12 by scambier          #+#    #+#             */
/*   Updated: 2024/06/08 16:24:14 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
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

#include "mlx.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}




int	close_hook(void *mlx)
{
	mlx_loop_end(mlx);
	return (0);
}

#include <X11/keysym.h>
#include <X11/X.h>

int	main(int argc, char **argv)
{
	// t_map	map;

	// ft_memset(&map, 0, sizeof(t_map));
	// load_map("map.cub", &map);
	// print_map(&map);
	if (argc < 2)
		return (ft_fprintf(2, "Missing arg\n") & 0);
	
	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, 256, 256, "Cube3D de zinzin furieux moijdi");
	t_data mlx_img;
	mlx_img.img = mlx_new_image(mlx, 256, 256);
	mlx_img.addr = mlx_get_data_addr(mlx_img.img, &mlx_img.bits_per_pixel, &mlx_img.line_length, &mlx_img.endian);

	mlx_put_image_to_window(mlx, win, mlx_img.img, 0, 0);
	mlx_hook(win, DestroyNotify, ButtonPressMask, close_hook, mlx);
	mlx_loop(mlx);
	mlx_destroy_image(mlx, mlx_img.img);
	mlx_destroy_window(mlx, win);
	mlx_destroy_display(mlx);
	free(mlx);

	return (0);
}
