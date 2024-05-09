/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_png.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:19:20 by scambier          #+#    #+#             */
/*   Updated: 2024/05/09 15:43:30 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

#include "libft.h"
#include "structs.h"

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

int	calcul_crc(unsigned int crc, unsigned char *bloc, unsigned int size)
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
	return (c ^ 0xFFFFFFFF);
}

int	bytes_to_int(char *bytes)
{
	return ((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + (bytes[3] << 0));
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
	char	*length;
	int		length_value;
	char	*type;
	char	*data;
	int		crc_value;
	char	*concat;

	int		crc1;

	length = read_malloc(fd, 4);
	length_value = bytes_to_int(length);
	
	type = read_malloc(fd, 4);

	data = read_malloc(fd, length_value);
	ft_pmem(data, length_value);

	crc_value = read_int(fd);

	char *a[4] = {(char *)type, (char *)data, (char *)length, 0};
	concat = ft_strsjoin((char **)a, "");
	crc1 = calcul_crc(0xFFFFFFFF, (unsigned char *)concat, ft_strlen(concat));
	ft_printf("Read crc: 0x%x\n", crc_value);
	ft_printf("Computed crc: 0x%x\n", crc1);
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
	// int crc1 = validation_crc(maj_crc(0xFFFFFFFF, (unsigned short *)buffer, 21));
	// ft_printf("calc : %x\n", crc1);
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