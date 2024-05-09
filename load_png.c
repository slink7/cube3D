/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_png.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:19:20 by scambier          #+#    #+#             */
/*   Updated: 2024/05/09 17:48:45 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

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
	read(fd, out, length);
	return (out);
}

void	read_chunk(int fd)
{
	int		length;
	char	*data;
	unsigned int	crc;

	length = read_int(fd);

	data = read_malloc(fd, 4 + length);

	crc = read_int(fd);
	if (crc != crc32(data, length + 4))
	{
		ft_printf("Error: CRC token is invalid\n");
	}
}

t_image *load_png(char *path)
{
	t_image	*out;
	char	buffer[25];
	int		fd;

	out = ft_calloc(1, sizeof(t_image));
	fd = open(path, O_RDONLY);
	if (fd < 3)
		return (0);
	read(fd, buffer, 8);
	ft_pmem(buffer, 8);
	if (ft_strncmp("\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", buffer, 8) != 0)
	{
		ft_printf("Error: Invalid PNG signature");
		return (0);
	}
	read_chunk(fd);
	return (out);
}