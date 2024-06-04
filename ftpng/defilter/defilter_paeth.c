/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defilter_paeth.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:50:59 by scambier          #+#    #+#             */
/*   Updated: 2024/06/04 14:55:43 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

static int sh_png_paeth_predict(int a, int b, int c) { 
    int p = a + b - c;
    int pa = ft_abs(p - a);
    int pb = ft_abs(p - b);
    int pc = ft_abs(p - c);

    if(pa <= pb && pa <= pc) return a;
    if(pb <= pc) return b;
    return c;
}

void	defilter_paeth(t_uint8 *dst, t_uint8 *src, int width, int bpr)
{
	int	k;

	k = -1;
	while (++k < width)
	{
		t_uint8 a = 0;
		t_uint8 b = (dst - bpr)[k];
		t_uint8 c = 0;
		if(k) { 
			a = dst[k - 1];
			c = (dst - bpr)[k - 1];
		}
		dst[k] = src[k] + sh_png_paeth_predict(a, b, c);
	}
}
