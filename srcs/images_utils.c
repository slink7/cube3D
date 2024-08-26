/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 18:44:41 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/17 18:44:41 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "funct.h"
#include "incl.h"

void	put_pixel(t_image *img, int x, int y, int color)
{
	int	index;

	index = img->line_length * y + x * (img->bits_per_pixel / 8);
	if (img->endian == 1)
	{
		img->datachar[index + 0] = (color >> 24);
		img->datachar[index + 1] = (color >> 16) & 0xFF;
		img->datachar[index + 2] = (color >> 8) & 0xFF;
		img->datachar[index + 3] = (color) & 0xFF;
	}
	else if (img->endian == 0)
	{
		img->datachar[index + 0] = (color) & 0xFF;
		img->datachar[index + 1] = (color >> 8) & 0xFF;
		img->datachar[index + 2] = (color >> 16) & 0xFF;
		img->datachar[index + 3] = (color >> 24);
	}
}

void	init_imgs(t_data *data, t_image *img, int width, int heigth)
{
	img->addr = mlx_new_image(data->mlx_ptr, width, heigth);
	img->datachar = mlx_get_data_addr(img->addr, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	img->width = width;
	img->height = heigth;
}
