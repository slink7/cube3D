/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:43:09 by scambier          #+#    #+#             */
/*   Updated: 2024/05/08 21:31:48 by scambier         ###   ########.fr       */
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

void	parse_map(char **lines, int *k, t_map *map)
{
	char	*pred;
	int		l;

	map->content = ft_calloc(ft_strarrlen(lines), sizeof(char *));
	ft_printf("%d\n", ft_strarrlen(lines + *k));
	l = 0;
	while (lines[++*k])
	{
		pred = ft_strchrf(lines[*k], map_predicat, 1);
		if (pred)
			ft_printf("Error: '%c' is an invalid map char\n", *pred);
		map->content[l++] = (t_tile *)ft_strdup(lines[*k]);
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
