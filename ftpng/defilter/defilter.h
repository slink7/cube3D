/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defilter.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:48:08 by scambier          #+#    #+#             */
/*   Updated: 2024/06/04 15:04:02 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFILTER_H
# define DEFILTER_H

#include "structs.h"

void	defilter_none(t_uint8 *dst, t_uint8 *src, int width, int bpr);
void	defilter_sub(t_uint8 *dst, t_uint8 *src, int width, int bpr);
void	defilter_up(t_uint8 *dst, t_uint8 *src, int width, int bpr);
void	defilter_avg(t_uint8 *dst, t_uint8 *src, int width, int bpr);
void	defilter_paeth(t_uint8 *dst, t_uint8 *src, int width, int bpr);

void	png_defilter(t_png *png);

#endif