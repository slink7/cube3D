/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:43:09 by scambier          #+#    #+#             */
/*   Updated: 2024/05/08 20:03:18 by scambier         ###   ########.fr       */
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

int	parse_map(char *path, t_map *map)
{
	char	**lines;
	int		k;
	int		mode;
	char	*pred;

	lines = ft_get_file(path);
	mode = 0;
	k = -1;
	while (lines[++k])
	{
		if (mode == 0)
		{
			if (ft_strlen(lines[k]) < 1 || !ft_strchrf(lines[k], space_predicat, 1))
				continue ;
			if (!ft_strchrf(lines[k], map_predicat, 1))
			{
				ft_printf("Line %d: %s\nSwitching to map parsing\n", k, lines[k]);
				mode = 1;
			}
		}
		if (mode == 1)
		{
			pred = ft_strchrf(lines[k], map_predicat, 1);
			if (pred)
			{
				ft_printf("Error: '%c' is an invalid map char\n", *pred);
			}
		}
	}
	ft_strarrfree(lines);
	return (1);
}
