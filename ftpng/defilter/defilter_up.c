/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defilter_up.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:45:45 by scambier          #+#    #+#             */
/*   Updated: 2024/06/04 14:49:40 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

void	defilter_up(t_uint8 *dst, t_uint8 *src, int width, int bpr)
{
	int	k;

	k = -1;
	while (++k < width)
		dst[k] = src[k] + (dst - bpr)[k];
}
