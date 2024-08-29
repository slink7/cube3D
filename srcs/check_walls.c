/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_walls.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:45:42 by scambier          #+#    #+#             */
/*   Updated: 2024/08/29 14:58:23 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"

static int	check_tile(t_map *map, int k, int l)
{
	if (k == map->width - 1 || l == map->height - 1)
		return (!!ft_strchr(" 1", map->content[l][k]));
	if (k == 0 || l == 0)
		return (!!ft_strchr(" 1", map->content[l][k]));
	if (map->content[l][k] == ' ' && !ft_strchr(" 1", map->content[l][k + 1]))
		return (0);
	if (map->content[l][k] == ' ' && !ft_strchr(" 1", map->content[l + 1][k]))
		return (0);
	if (map->content[l][k + 1] == ' ' && !ft_strchr(" 1", map->content[l][k]))
		return (0);
	if (map->content[l + 1][k] == ' ' && !ft_strchr(" 1", map->content[l][k]))
		return (0);
	return (1);
}

int	check_walls(t_map *map)
{
	int	k;
	int	l;

	l = -1;
	while (++l < map->height - 1)
	{
		k = -1;
		while (++k < map->width)
			if (!check_tile(map, k, l))
				return (ft_fprintf(2, "Error: Invalid map at (%d;%d)\n"
						, k, l) & 0);
	}
	return (1);
}
