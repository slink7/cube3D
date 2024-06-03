/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:34:12 by scambier          #+#    #+#             */
/*   Updated: 2024/06/03 18:11:53 by scambier         ###   ########.fr       */
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
	for (int k = 0; k < in->data_len; k++)
	{
		my_mlx_pixel_put(out, k % in->width, k / in->width, bytes_to_int2(in->data + k));
	}
}

// uint8* sh_png_defilter(uint8 *decompressed_image, uint32 size, sh_png_chunk *ihdr) {
//     uint32 x = sh_get_uint32be(ihdr->data);
//     uint32 y = sh_get_uint32be(ihdr->data+4);
//     uint8 bit_depth = *( ihdr->data + 4 + 4 );//count the bytes out
//     uint8 byte_per_pixel = 1;//this is usually determined by checking color type, the picture I'm using is only greyscale, its only one byte per pixel

//     uint8 *row = decompressed_image;
//     uint32 stride = x*byte_per_pixel;

//     uint8 *image = malloc(x*y*byte_per_pixel); //this is even smaller than the filter but just being safe
//     uint8 *working = image;
//     for(uint32 i = 0; i < y; ++i) {
//         working = image + i*stride;
//         uint8 filter = *row++;

//         switch(filter) {
//             case sh_no_filter: {
//                 for(uint32 j = 0; j < x; ++j) {
//                     working[j] = row[j];
//                 }
//             } break;

//             case sh_sub_filter: {
//                 for(uint32 j = 0; j < x; ++j) {
//                     uint8 a = 0;
//                     if(j != 0) {
//                         a = working[j-1];
//                     }
//                     uint8 value = row[j] + a;
//                     working[j] = value;
//                 }
//             } break;

//             case sh_up_filter: {
//                 uint8 *prev_row = working - stride;
//                 for(uint32 j = 0; j < x; ++j) {
//                     uint8 b = prev_row[j];
//                     uint8 value = row[j] + b;
//                     working[j] = value;
//                 }
//            } break;

//             case sh_avg_filter: {
//                 uint8 *prev_row = working - stride;
//                 for(uint32 j = 0; j < x; ++j) {
//                     uint8 a = 0;
//                     uint8 b = prev_row[j];
//                     if(j) {
//                         a = working[j - 1];
//                     }

//                     uint8 value = row[j] + ( (a + b) >> 1 );
//                     working[j] = value;
//                 }
//             } break;

//             case sh_paeth_filter: {
//                 uint8 *prev_row = working - stride;
//                 for(uint32 j = 0; j < x; ++j) {
//                     uint8 a = 0;
//                     uint8 b = prev_row[j];
//                     uint8 c = 0;
//                     if(j) { 
//                         a = working[j - 1];
//                         c = prev_row[j - 1];
//                     }

//                     uint8 value = row[j] + sh_png_paeth_predict((int32) a, (int32) b, (int32) c);
//                     working[j] = value;
//                 }
//             } break;

//         }

//         row += stride;
//     }

//     return image;
// }

enum sh_png_filters {
    sh_no_filter,
    sh_sub_filter,
    sh_up_filter,
    sh_avg_filter,
    sh_paeth_filter
};

int sh_png_paeth_predict(int a, int b, int c) { 
    int p = a + b - c;
    int pa = ft_abs(p - a);
    int pb = ft_abs(p - b);
    int pc = ft_abs(p - c);

    if(pa <= pb && pa <= pc) return a;
    if(pb <= pc) return b;
    return c;
}

void	defilter(t_png *png)
{
	t_uint8	*row;
	t_uint8 byte_per_pixel = 1;
	t_uint32 byte_per_row = png->width * byte_per_pixel;
	t_uint8	*out;
	t_uint32 out_len;

	row = png->data;
	out_len = png->width * png->height * byte_per_pixel;
	out = ft_calloc(out_len, sizeof(t_uint8));
	for (int l = 0; l < png->height; l++)
	{
		t_uint8 *working = out + l * byte_per_row;
		t_uint8 filter = *row++;
		switch (filter)
		{
		case sh_no_filter:
			ft_memcpy(working, row, byte_per_row);
			break;
		
		case sh_sub_filter:
			for(t_uint32 j = 0; j < png->width; ++j) {
				t_uint8 a = 0;
				if(j != 0) {
					a = working[j-1];
				}
				t_uint8 value = row[j] + a;
				working[j] = value;
			}
		break;
		case sh_up_filter: 
			for(t_uint32 j = 0; j < png->width; ++j) {
				t_uint8 b = (working - byte_per_row)[j];
				t_uint8 value = row[j] + b;
				working[j] = value;
			}
		break;
		case sh_avg_filter:
			for(t_uint32 j = 0; j < png->width; ++j) {
				t_uint8 a = 0;
				t_uint8 b = (working - byte_per_row)[j];
				if(j) {
					a = working[j - 1];
				}

				t_uint8 value = row[j] + ( (a + b) >> 1 );
				working[j] = value;
			}
			break;
		case sh_paeth_filter:
                for(t_uint32 j = 0; j < png->width; ++j) {
                    t_uint8 a = 0;
                    t_uint8 b = (working - byte_per_row)[j];
                    t_uint8 c = 0;
                    if(j) { 
                        a = working[j - 1];
                        c = (working - byte_per_row)[j - 1];
                    }

                    t_uint8 value = row[j] + sh_png_paeth_predict(a, b, c);
                    working[j] = value;
                }
            break;
		default:
			break;
		}
		row += byte_per_row;
	}
	free(png->data);
	png->data = out;
	png->data_len = out_len;
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
	defilter(&img);
	// ft_pmem(img.data, img.data_len);

	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, img.width, img.height, "Cube3D de zinzin furieux moijdi");

	t_data mlx_img;
	mlx_img.img = mlx_new_image(mlx, img.width, img.height);
	mlx_img.addr = mlx_get_data_addr(mlx_img.img, &mlx_img.bits_per_pixel, &mlx_img.line_length, &mlx_img.endian);
	png_to_data(&mlx_img, &img);
	mlx_put_image_to_window(mlx, win, mlx_img.img, 0, 0);
	ft_pmem(img.data, img.data_len / 2);
	mlx_loop(mlx);
	return (0);
}
