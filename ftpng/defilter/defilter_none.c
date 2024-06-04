/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defilter_none.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:44:48 by scambier          #+#    #+#             */
/*   Updated: 2024/06/04 14:49:36 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "structs.h"

void defilter_none(t_uint8 *dst, t_uint8 *src, int width, int bpr)
{
	(void) width;
	ft_memcpy(dst, src, bpr);
}
