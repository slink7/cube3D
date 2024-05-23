/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpng.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:52:17 by scambier          #+#    #+#             */
/*   Updated: 2024/05/23 16:51:56 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTPNG_H
# define FTPNG_H

#include "structs.h"

typedef struct s_bit_stream {
	t_uint8		*data;
	t_uint32	bit_buffer;
	t_uint32	bits_left;
}	t_bit_stream;

int		load_png(t_image *dst, char *path);
int		inflate(t_uint8 *dst, t_uint32 dst_len, t_uint8 *src, t_uint32 src_len);

#endif