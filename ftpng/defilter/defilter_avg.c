/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defilter_avg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:46:56 by scambier          #+#    #+#             */
/*   Updated: 2024/06/04 14:49:35 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

void defilter_avg(t_uint8 *dst, t_uint8 *src, int width, int bpr)
{
	int	k;

	k = -1;
	while (++k < width)
	{
		t_uint8 a = 0;
		t_uint8 b = (dst - bpr)[k];
		if(k) {
			a = dst[k - 1];
		}
		dst[k] = src[k] + ( (a + b) >> 1 );
	}
}
