/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:34:12 by scambier          #+#    #+#             */
/*   Updated: 2024/06/08 19:39:49 by scambier         ###   ########.fr       */
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



void	load_texture(void *mlx, t_image *img)
{
	if (!ft_strncmp(img->path + ft_strlen(img->path) - 4, ".xpm", 5))
	{
		img->content = mlx_xpm_file_to_image(mlx, img->path, &img->width, &img->height);
		img->addr = mlx_get_data_addr(img->content, &img->bits_per_pixel, &img->line_length, &img->endian);
	}
	else if (!ft_strncmp(img->path + ft_strlen(img->path) - 4, ".png", 5))
		ft_fprintf(2, "Error: PNG not yet supported");
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
	t_map	map;

	ft_memset(&map, 0, sizeof(t_map));
	load_map("map.cub", &map);
	print_map(&map);
	
	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, 256, 256, "Cube3D de zinzin furieux moijdi");
	load_texture(mlx, map.wall_textures + 0);
	load_texture(mlx, map.wall_textures + 1);
	load_texture(mlx, map.wall_textures + 2);
	load_texture(mlx, map.wall_textures + 3);

	for(int k = 0; k < 4; k++)
		mlx_put_image_to_window(mlx, win, map.wall_textures[k].content, k * map.wall_textures[k].width, 0);
	mlx_hook(win, DestroyNotify, ButtonPressMask, close_hook, mlx);
	mlx_loop(mlx);
	//mlx_destroy_image(mlx, mlx_img.img);
	mlx_destroy_window(mlx, win);
	mlx_destroy_display(mlx);
	free(mlx);

	return (0);
}
