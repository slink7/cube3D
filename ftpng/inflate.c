/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inflate.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 04:50:42 by scambier          #+#    #+#             */
/*   Updated: 2024/05/11 10:31:12 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"

typedef struct s_bit_stream {
	t_uint8		*data;
	t_uint32	bit_buffer;
	t_uint32	bits_left;

}	t_bit_stream;

void	get_bits(t_bit_stream *stream, t_uint32 bits_req)
{
	t_uint32	extra_bits_needed;
	t_uint32	bytes_to_read;
	int			k;

	extra_bits_needed = (stream->bits_left > bits_req) ? (stream->bits_left - bits_req) : (bits_req - stream->bits_left);
	bytes_to_read = extra_bits_needed / 8;

	if (extra_bits_needed % 8)
		bytes_to_read++;
	
	k = -1;
	while (++k < bytes_to_read)
		stream->bit_buffer |= (*stream->data++) << (k * 8 + stream->bits_left);
	
	stream->bits_left += bytes_to_read * 8;
}

t_uint32	read_bits(t_bit_stream *stream, t_uint32 bits_to_read)
{
	t_uint32	out;
	int			k;

	out = 0;
	if (bits_to_read > stream->bits_left)
		get_bits(stream, bits_to_read);
	k = -1;
	while (++k < bits_to_read)
		out |= stream->bit_buffer & (1 << k);
	
	stream->bit_buffer >>= bits_to_read;
	stream->bits_left -= bits_to_read;
	return (out);
}

int	inflate(t_uint8 *dst, t_uint32 dst_len, t_uint8 *src, t_uint32 src_len)
{
	t_bit_stream	stream;

	stream.data = src;
	stream.bit_buffer = 0;
	stream.bits_left = 0;
	ft_pmem(src, 1);
	ft_printf("Is last: %x\n", read_bits(&stream, 1));
	ft_printf("Block type: %x\n", read_bits(&stream, 2));
	return (0);
}