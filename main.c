/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:34:12 by scambier          #+#    #+#             */
/*   Updated: 2024/05/08 18:52:43 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>

#include "libft.h"

#include "parsing.h"

void	print_map(t_map *map)
{
	ft_printf("FLOOR   COLOR : %x\n", map->floor_color);
	ft_printf("CEILING COLOR : %x\n", map->ceiling_color);
}

int	main(int argc, char **argv)
{
	t_map	map;

	ft_memset(&map, 0, sizeof(t_map));
	parse_map("map.cub", &map);
	print_map(&map);
	return (0);
}
