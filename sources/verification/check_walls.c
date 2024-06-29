/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_walls.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:45:42 by scambier          #+#    #+#             */
/*   Updated: 2024/06/29 20:05:26 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"

static int	check_tile(t_map *map, int k, int l)
{
	if (k == map->width - 1 || l == map->height - 1)
		return (map->content[l][k] != '0');
	if ((k == 0 || l == 0) && map->content[l][k] == '0')
		return (0);
	if (ft_abs(map->content[l][k] - map->content[l][k + 1]) == '0' - ' ')
		return (0);
	if (ft_abs(map->content[l][k] - map->content[l + 1][k]) == '0' - ' ')
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
				return (ft_fprintf(2, "Error: Invalid map at (%d;%d)\n", k, l) & 0);
	}
	return (1);
}