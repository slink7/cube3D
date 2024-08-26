/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:36:44 by scambier          #+#    #+#             */
/*   Updated: 2024/08/26 19:24:34 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "structs.h"
#include "parsing_utils.h"

static int	parse_infos(char **lines, int *k, t_map *map)
{
	while (lines[++*k])
	{
		if (ft_strlen(lines[*k]) < 1
			|| !ft_strchrf(lines[*k], space_predicat, 1))
			continue ;
		else if (!ft_strchrf(lines[*k], map_predicat, 1))
			return (1);
		else if (lines[*k][0] == 'F')
			map->floor_color = read_color(lines[*k] + 2);
		else if (lines[*k][0] == 'C')
			map->ceiling_color = read_color(lines[*k] + 2);
		else if (!ft_strncmp(lines[*k], "EA ", 3))
			map->wall_textures[EAST].path = ft_strdup(lines[*k] + 3);
		else if (!ft_strncmp(lines[*k], "NO ", 3))
			map->wall_textures[NORTH].path = ft_strdup(lines[*k] + 3);
		else if (!ft_strncmp(lines[*k], "WE ", 3))
			map->wall_textures[WEST].path = ft_strdup(lines[*k] + 3);
		else if (!ft_strncmp(lines[*k], "SO ", 3))
			map->wall_textures[SOUTH].path = ft_strdup(lines[*k] + 3);
	}
	ft_fprintf(2, "Error: Missing map layout");
	return (0);
}

static int	parse_map(char **lines, int *k, t_map *map)
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
	char	*temp;

	ft_get_file(&temp, path, 128);
	lines = ft_split(temp, '\n');
	free(temp);
	k = -1;
	if (!parse_infos(lines, &k, map))
		return (ft_strarrfree(lines) & 0);
	if (!parse_map(lines, &k, map))
		return (ft_strarrfree(lines) & 0);
	ft_strarrfree(lines);
	return (1);
}
