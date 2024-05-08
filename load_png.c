/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_png.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:19:20 by scambier          #+#    #+#             */
/*   Updated: 2024/05/09 01:22:26 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

#include "libft.h"
#include "structs.h"

int	validation_crc(unsigned int crc)
{
	return (crc ^ 0xFFFFFFFF);
}

unsigned int	table_crc(unsigned int i)
{
	unsigned int	c, j;

	c = i;
	j = 0;
	while (j < 8)
	{
		if (c & 1)
			c = 0xEDB88320 ^ (c / 2);
		else
			c = c / 2;
		j++;
	}
	return (c);
}

int	maj_crc(unsigned int crc, unsigned short *bloc, unsigned int size)
{
	unsigned int c, n, v;
	c = crc;

	n = 0;
	while (n < size)
	{
		v = c ^ bloc[n];
		v = v & 0xff;
		v = table_crc(v);
		c = v ^ (c / 256);
		n++;
	}
	return (c);
}

int	bytes_to_int(char *bytes)
{
	return ((bytes[0] << 3) + (bytes[1] << 2) + (bytes[2] << 1) + (bytes[3] << 0));
}

int	read_int(int fd)
{
	char	buffer[4];

	read(fd, buffer, 4);
	return ((buffer[0] << 3) + (buffer[1] << 2) + (buffer[2] << 1) + (buffer[3] << 0));
}

void	read_chunk(int fd)
{
	int		length;
	char	name[4];
	char	*data;
	int		crc0;
	int		crc1;

	length = read_int(fd);
	read(fd, name, 4);
	data = ft_calloc(length + 1, sizeof(char));
	read(fd, data, length);
	ft_pmem(data, length);
	ft_printf("%d bytes\n", length);
	crc0 = read_int(fd);
	crc1 = validation_crc(maj_crc(0xFFFFFFFF, (unsigned short *)data, length));
	ft_printf("Read crc: %x\n", crc0);
	ft_printf("Computed crc: %u\n", crc1);
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
	// read(fd, buffer, 25);
	// ft_pmem(buffer, 25);
	read_chunk(fd);
	
	// read(fd, buffer, 4);
	// ft_pmem(buffer, 4);
	// read(fd, buffer, 4); //4 IHDR
	// ft_pmem(buffer, 4); 
	// if (ft_strncmp("IHDR", buffer, 4) != 0)
	// {
	// 	ft_printf("Error: Missing IHDR chunk");
	// 	return (0);
	// }
	// out->width = read_int(fd); //4 WIDTH
	// out->height = read_int(fd); //4 HEIGHT
	// ft_printf("DIM : %d * %d\n", out->width, out->height);
	return (0);
}