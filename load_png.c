/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_png.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:19:20 by scambier          #+#    #+#             */
/*   Updated: 2024/05/10 04:40:03 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "libft.h"
#include "structs.h"

unsigned int	crc32(unsigned char *bloc, unsigned int size)
{
	int				k;
	int				l;
	unsigned int	crc;
	char			ch;
	unsigned int	b;

	crc = 0xFFFFFFFF;
	k = -1;
	while ((unsigned int)++k < size)
	{
		ch = bloc[k];
		l = -1;
		while ((unsigned int)++l < 8)
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

unsigned int	bytes_to_int(char *bytes)
{
	return ((0xFF000000 & bytes[0] << 24)
		+ (0x00FF0000 & bytes[1] << 16)
		+ (0x0000FF00 & bytes[2] << 8)
		+ (0x000000FF & bytes[3] << 0));
}

int	read_int(int fd)
{
	char	buffer[4];

	read(fd, buffer, 4);
	return (bytes_to_int(buffer));
}

char	*read_malloc(int fd, int length)
{
	char	*out;

	out = ft_calloc(length + 1, sizeof(char));
	if (!out)
		return (0);
	read(fd, out, length);
	return (out);
}

typedef struct s_png_chunk
{
	char	*data;
	int		length;
}	t_png_chunk;

void	free_chunk(t_png_chunk **chunk)
{
	free((*chunk)->data);
	free(*chunk);
	*chunk = 0;
}

t_png_chunk	*read_chunk(int fd)
{
	t_png_chunk		*chunk;

	chunk = ft_calloc(1, sizeof(t_png_chunk));
	if (!chunk)
		return ((void *)(ft_fprintf(2, "Error: malloc failed\n") & 0l));
	chunk->length = read_int(fd);
	chunk->data = read_malloc(fd, 4 + chunk->length);
	if (!chunk->data)
	{
		free(chunk);
		return ((void *)(ft_fprintf(2, "Error: malloc failed\n") & 0l));
	}
	if (read_int(fd) != crc32(chunk->data, chunk->length + 4))
	{
		free_chunk(&chunk);
		return ((void *)(ft_fprintf(2, "Error: CRC token is invalid\n") & 0l));
	}
	return (chunk);
}

t_image *load_png(char *path)
{
	t_image		*out;
	t_png_chunk	*ihdr;
	t_png_chunk	*chunk;
	char		buffer[8];
	int			fd;

	out = ft_calloc(1, sizeof(t_image));
	fd = open(path, O_RDONLY);
	if (fd < 3)
		return ((void *)(ft_fprintf(2, "Error: Invalid file \"%s\"\n", path) & 0l));
	read(fd, buffer, 8);
	if (ft_strncmp("\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", buffer, 8) != 0)
	{
		free(out);
		return ((void *)(ft_fprintf(2, "Error: Invalid PNG header\n") & 0l));
	}
	ihdr = read_chunk(fd);
	if (ft_strncmp(ihdr->data, "IHDR", 4) != 0)
	{
		free_chunk(&ihdr);
		free(out);
		return ((void *)(ft_fprintf(2, "Error: Missing IHDR chunk\n") & 0l));
	}
	out->width = bytes_to_int(ihdr->data + 4);
	ft_printf("Width: %d\n", out->width);
	out->height = bytes_to_int(ihdr->data + 8);
	ft_printf("Height: %d\n", out->height);
	ft_printf("Bit depth: %d\n", ihdr->data[12]);
	ft_printf("Color type: %d\n", ihdr->data[13]);
	ft_printf("Compression method: %d\n", ihdr->data[14]);
	if (ihdr->data[14] != 0)
	{
		free_chunk(&ihdr);
		free(out);
		return ((void *)(ft_fprintf(2, "Error: Unsupported compression method\n") & 0l));
	}
	ft_printf("Filter method: %d\n", ihdr->data[15]);
	if (ihdr->data[15] != 0)
	{
		free_chunk(&ihdr);
		free(out);
		return ((void *)(ft_fprintf(2, "Error: Unsupported filter method\n") & 0l));
	}
	ft_printf("Interlaced: %d\n", ihdr->data[16]);
	chunk = read_chunk(fd);
	while (ft_strncmp(chunk->data, "IEND", 4) != 0)
	{
		ft_pmem(chunk->data, chunk->length + 4);
		free_chunk(&chunk);
		chunk = read_chunk(fd);
	}
	free_chunk(&chunk);
	free_chunk(&ihdr);
	return (out);
}
