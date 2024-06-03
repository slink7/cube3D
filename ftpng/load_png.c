/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_png.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:19:20 by scambier          #+#    #+#             */
/*   Updated: 2024/06/03 16:45:46 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include "libft.h"
#include "ftpng.h"
#include "structs.h"
#include "parsing.h"



t_uint32	crc32(t_uint8 *bloc, t_uint32 size)
{
	int			k;
	int			l;
	t_uint32	crc;
	t_uint8		ch;
	t_uint32	b;

	crc = 0xFFFFFFFF;
	k = -1;
	while ((t_uint32)++k < size)
	{
		ch = bloc[k];
		l = -1;
		while ((t_uint32)++l < 8)
		{
			b = (ch ^ crc) & 1;
			crc >>= 1;
			if (b)
				crc = crc ^ 0xEDB88320;
			ch >>= 1;
		}
	}
	return (~crc);
}

t_uint32	bytes_to_int(t_uint8 *bytes)
{
	return ((0xFF000000 & bytes[0] << 24)
		+ (0x00FF0000 & bytes[1] << 16)
		+ (0x0000FF00 & bytes[2] << 8)
		+ (0x000000FF & bytes[3] << 0));
}

typedef struct s_png_chunk
{
	char	*data;
	int		length;
}	t_png_chunk;

int	read_chunk(t_png_chunk *dst, t_uint8 *data, t_uint32 len)
{
	if (len < 4)
		return (ft_fprintf(2, "Error: truncated chunk\n") & 0);
	dst->length = bytes_to_int(data);
	if (len < dst->length + 4)
		return (ft_fprintf(2, "Error: truncated chunk\n") & 0);
	dst->data = data + 4;
	if (bytes_to_int(data + dst->length + 8) != crc32(dst->data, dst->length + 4))
		return (ft_fprintf(2, "Error: CRC token is invalid\n") & 0);
	return (1);
}

int	load_png(t_png *dst, char *path)
{
	t_uint8		*file;
	t_uint32	file_length;
	t_png_chunk	ihdr_chunk;
	t_png_chunk	chunk;
	int			file_index;

	file_length = ft_get_file((char **)&file, path, 128);
	if (!file)
		return (ft_fprintf(2, "Error: Can't open file \'%s\'\n", path) & 0);
	if (file_length < 57)
	{
		free(file);
		return (ft_fprintf(2, "Error: Incomplete file \'%s\'\n", path) & 0);
	}
	if (ft_memcmp(file, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8) != 0)
	{
		free(file);
		return (ft_fprintf(2, "Error: Invalid PNG header\n") & 0);
	}
	ft_pmem(file, file_length);
	read_chunk(&ihdr_chunk, file + 8, file_length - 8);
	if (ft_memcmp(ihdr_chunk.data, "IHDR", 4) != 0)
	{
		free(file);
		return (ft_fprintf(2, "Error: Missing IHDR chunk\n") & 0);
	}
	dst->width = bytes_to_int(ihdr_chunk.data + 4);
	dst->height = bytes_to_int(ihdr_chunk.data + 8);
	dst->bit_depth = ihdr_chunk.data[12];
	dst->color_type = ihdr_chunk.data[13];
	dst->compression_method = ihdr_chunk.data[14];
	if (dst->compression_method != 0)
	{
		free(file);
		return ((ft_fprintf(2, "Error: Unsupported compression method\n") & 0));
	}
	dst->filter_method = ihdr_chunk.data[15];
	if (dst->filter_method != 0)
	{
		free(file);
		return ((ft_fprintf(2, "Error: Unsupported filter method\n") & 0));
	}
	dst->interlaced = ihdr_chunk.data[16];

	file_index = 33;
	read_chunk(&chunk, file + file_index, file_length - file_index);
	while (ft_memcmp(chunk.data, "IEND", 4) != 0)
	{
		if (ft_memcmp(chunk.data, "IDAT", 4) == 0)
		{
			dst->data_len = inflate(&dst->data, chunk.data + 4, chunk.length);
		}
		//ft_pmem(chunk.data, chunk.length + 4);
		file_index += chunk.length + 12;
		read_chunk(&chunk, file + file_index, file_length - file_index);
	}
	free(file);
	return (1);
}
