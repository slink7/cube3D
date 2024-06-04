/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defilter_sub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:46:18 by scambier          #+#    #+#             */
/*   Updated: 2024/06/04 14:51:58 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

void	defilter_sub(t_uint8 *dst, t_uint8 *src, int width, int bpr)
{
	int		k;
	t_uint8	a;

	(void) bpr;
	k = -1;
	while (++k < width)
	{
		a = 0;
		if (k)
			a = dst[k - 1];
		dst[k] = src[k] + a;
	}
}
