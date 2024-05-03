/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:34:12 by scambier          #+#    #+#             */
/*   Updated: 2024/05/03 15:08:23 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>

#include "libft.h"

#include "parsing.h"

int	main(int argc, char **argv)
{
	// int fd = open("map.cub", O_RDONLY);
	// ft_printf("[%s]\n", get_next_line(fd));
	// ft_printf("[%s]\n", get_next_line(fd));
	// ft_printf("[%s]\n", get_next_line(fd));
	parse_map("map.cub", 0);
	return (0);
}
