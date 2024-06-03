/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:34:12 by scambier          #+#    #+#             */
/*   Updated: 2024/06/03 17:10:01 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "parsing.h"
#include "ftpng.h"

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
	ft_printf("(%d, %d) : %X\n", x, y, color);
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

t_uint32	bytes_to_int2(t_uint8 *bytes)
{
	return ((0xFF000000 & bytes[0] << 24)
		+ (0x00FF0000 & bytes[1] << 16)
		+ (0x0000FF00 & bytes[2] << 8)
		+ (0x000000FF & bytes[3] << 0));
}

void	png_to_data(t_data *out, t_png *in)
{
	for (int k = 0; k < in->data_len / 4; k++)
	{
		my_mlx_pixel_put(out, k % in->width, k / in->width, bytes_to_int2(in->data + 4 * k));
	}
}

int	main(int argc, char **argv)
{
	// t_map	map;

	// ft_memset(&map, 0, sizeof(t_map));
	// load_map("map.cub", &map);
	// print_map(&map);
	if (argc < 2)
		return (ft_fprintf(2, "Missing arg\n") & 0);
	t_png img;
	load_png(&img, argv[1]);
	// ft_pmem(img.data, img.data_len);

	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, img.width, img.height, "Test");

	t_data mlx_img;
	mlx_img.img = mlx_new_image(mlx, img.width, img.height);
	mlx_img.addr = mlx_get_data_addr(mlx_img.img, &mlx_img.bits_per_pixel, &mlx_img.line_length, &mlx_img.endian);
	png_to_data(&mlx_img, &img);
	mlx_put_image_to_window(mlx, win, mlx_img.img, 0, 0);
	mlx_loop(mlx);
	return (0);
}
