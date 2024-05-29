/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_stream.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:51:28 by scambier          #+#    #+#             */
/*   Updated: 2024/05/24 12:49:27 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpng.h"

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

t_uint32 peek_bits_reversed(t_bit_stream *stream, t_uint32 bits_to_peek) {
    t_uint32	out;
	int			k;

	out = 0;
	if(bits_to_peek > stream->bits_left) {
        get_bits(stream, bits_to_peek);
    }
    k = -1;
	while (++k < bits_to_peek)
		out = (out << 1) | (stream->bit_buffer & (1 << k)) > 0;
    return (out);
}