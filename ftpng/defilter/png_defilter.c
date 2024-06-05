/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_defilter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:41:19 by scambier          #+#    #+#             */
/*   Updated: 2024/06/04 15:03:01 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "structs.h"
#include "ftpng.h"

#include "defilter.h"

void	png_defilter(t_png *png)
{
	t_uint8	*row;
	t_uint8 byte_per_pixel = 1;
	t_uint32 byte_per_row = png->width * byte_per_pixel;
	t_uint8	*out;
	t_uint32 out_len;
	static void (*defilters[])(t_uint8*, t_uint8*, int, int) = {
		defilter_none,
		defilter_sub,
		defilter_up,
		defilter_avg,
		defilter_paeth
	};

	row = png->data;
	out_len = png->width * png->height * byte_per_pixel;
	out = ft_calloc(out_len, sizeof(t_uint8));
	for (int l = 0; l < png->height; l++)
	{
		t_uint8 *working = out + l * byte_per_row;
		defilters[*row++](working, row, png->width, byte_per_row);
		row += byte_per_row;
	}
	free(png->data);
	png->data = out;
	png->data_len = out_len;
}
