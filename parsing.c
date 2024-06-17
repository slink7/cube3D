/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:43:09 by scambier          #+#    #+#             */
/*   Updated: 2024/06/12 18:48:08 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>

#include "libft.h"

#include "structs.h"
#include "mlx.h"

int	space_predicat(int c)
{
	return (c == ' ' || c == '\t');
}

int	map_predicat(int c)
{
	return (ft_strchr(" 01ENWS", c) != 0);
}

int	read_color(char *in)
{
	int	out;
	char **temp;

	if (!ft_strncmp(in, "0x", 2))
		return (ft_atoi_base(in + 2, "0123456789ABCDEF"));
	out = 0;
	temp = ft_split(in, ',');
	if (!temp)
		return (0);
	if (ft_strarrlen(temp) == 3)
	{
		out = out << 8 | (ft_atoi(temp[0]) & 0xFF);
		out = out << 8 | (ft_atoi(temp[1]) & 0xFF);
		out = out << 8 | (ft_atoi(temp[2]) & 0xFF);
	}
	else
		ft_fprintf(2, "Error: wrong color format \'%s\'\n", in);
	ft_strarrfree(temp);
	return (out);
}

int	parse_infos(char **lines, int *k, t_map *map)
{
	while (lines[++*k])
	{
		if (ft_strlen(lines[*k]) < 1 || !ft_strchrf(lines[*k], space_predicat, 1))
			continue ;
		else if (!ft_strchrf(lines[*k], map_predicat, 1))
			return (1);
		else if (lines[*k][0] == 'F')
			map->floor_color = read_color(lines[*k] + 2);
		else if (lines[*k][0] == 'C')
			map->ceiling_color = read_color(lines[*k] + 2);
		else if (!ft_strncmp(lines[*k], "EA ", 3))
			map->wall_textures[0].path = ft_strdup(lines[*k] + 3);
		else if (!ft_strncmp(lines[*k], "NO ", 3))
			map->wall_textures[1].path = ft_strdup(lines[*k] + 3);
		else if (!ft_strncmp(lines[*k], "WE ", 3))
			map->wall_textures[2].path = ft_strdup(lines[*k] + 3);
		else if (!ft_strncmp(lines[*k], "SO ", 3))
			map->wall_textures[3].path = ft_strdup(lines[*k] + 3);
	}
	ft_fprintf(2, "Error: Missing map layout");
	return (0);
}

int	get_longest(char **lines)
{
	int	k;
	int	max;
	int	temp;

	max = -0x80000000;
	k = -1;
	while (lines[++k])
	{
		temp = ft_strlen(lines[k]);
		if (temp > max)
			max = temp;
	}
	return (max);
}

int	parse_map(char **lines, int *k, t_map *map)
{
	char	*pred;
	int		l;
	int		line_len;

	map->height = ft_strarrlen(lines + *k);
	map->content = ft_calloc(map->height + 1, sizeof(char *));
	if (!map->content)
		return (0);
	map->width = get_longest(lines + *k);
	l = -1;
	while (lines[*k + ++l])
	{
		pred = ft_strchrf(lines[*k + l], map_predicat, 1);
		if (pred)
			return (ft_printf("Error: invalid map char: '%c'\n", *pred) & 0);
		map->content[l] = ft_calloc(map->width + 1, sizeof(t_tile));
		line_len = ft_strlen(lines[*k + l]);
		ft_memcpy(map->content[l], lines[*k + l], line_len);
		ft_memset(map->content[l] + line_len, ' ', map->width - line_len);
	}
	return (1);
}

int	load_map(char *path, t_map *map)
{
	char	**lines;
	int		k;
	int		mode;
	char	*temp;

	ft_get_file(&temp, path, 128);
	lines = ft_split(temp, '\n');
	free(temp);
	mode = 0;
	k = -1;
	if (!parse_infos(lines, &k, map))
		return (ft_strarrfree(lines) & 0);
	if (!parse_map(lines, &k, map))
		return (ft_strarrfree(lines) & 0);
	ft_strarrfree(lines);
	return (1);
}
