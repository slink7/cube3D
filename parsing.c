/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:43:09 by scambier          #+#    #+#             */
/*   Updated: 2024/05/08 21:47:45 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>

#include "libft.h"

#include "structs.h"

int	space_predicat(int c)
{
	return (c == ' ' || c == '\t');
}

int	map_predicat(int c)
{
	return (ft_strchr(" 01ENWS", c) != 0);
}

void	parse_infos(char **lines, int *k, t_map *map)
{
	while (lines[++*k])
	{
		if (ft_strlen(lines[*k]) < 1 || !ft_strchrf(lines[*k], space_predicat, 1))
			continue ;
		if (!ft_strchrf(lines[*k], map_predicat, 1))
			return ;
	}
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

void	parse_map(char **lines, int *k, t_map *map)
{
	char	*pred;
	int		l;

	map->height = ft_strarrlen(lines + *k);
	map->content = ft_calloc(map->height, sizeof(char *));
	if (!map->content)
		return ;
	map->width = get_longest(lines + *k);
	l = 0;
	while (lines[++*k])
	{
		pred = ft_strchrf(lines[*k], map_predicat, 1);
		if (pred)
			ft_printf("Error: '%c' is an invalid map char\n", *pred);
		map->content[l] = ft_calloc(map->width + 1, sizeof(t_tile));
		ft_memcpy(map->content[l++], lines[*k], map->width);
	}
}

int	load_map(char *path, t_map *map)
{
	char	**lines;
	int		k;
	int		mode;
	char	*pred;

	lines = ft_get_file(path);
	mode = 0;
	k = -1;
	parse_infos(lines, &k, map);
	k -= 1;
	parse_map(lines, &k, map);
	ft_strarrfree(lines);
	return (1);
}
