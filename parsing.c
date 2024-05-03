/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:43:09 by scambier          #+#    #+#             */
/*   Updated: 2024/05/03 17:25:24 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>

#include "libft.h"

#include "structs.h"

int	s(int c)
{
	return (c == ' ' || c == '\t');
}

int	map_valid(int c)
{
	return (ft_strchr(" 01ENWS", c) != 0);
}

void	parse_line_info(char *line, t_map *map)
{
	if (!ft_strncmp(line, "EA ", 3))
		ft_printf("Load EA : %s\n", line + 3);
	else if (!ft_strncmp(line, "NO ", 3))
		ft_printf("Load NO : %s\n", line + 3);
	else if (!ft_strncmp(line, "WE ", 3))
		ft_printf("Load WE : %s\n", line + 3);
	else if (!ft_strncmp(line, "SO ", 3))
		ft_printf("Load SO : %s\n", line + 3);
	else if (!ft_strncmp(line, "F ", 2))
		ft_printf("Load F : %s\n", line + 2);
	else if (!ft_strncmp(line, "C ", 2))
		ft_printf("Load C : %s\n", line + 2);
}

void	parse_informations(int fd, t_map *map)
{
	char	*line;

	line = get_next_line(fd);
	while (line && ft_strchrf(line, map_valid, 1))
	{
		parse_line_info(line, map);
		free(line);
		line = get_next_line(fd);
	}
}

void	lst_cpy_toarr(t_list *lst, char **arr)
{
	if (!lst)
		return ;
	arr[0] = lst->content;
	lst_cpy_toarr(lst->next, arr + 1);
}

char	**lst_export(t_list *lst)
{
	int		size;
	char	**out;

	size = ft_lstsize(lst);
	out = ft_calloc(size + 1, sizeof(char *));
	lst_cpy_toarr(lst, out);
	return (out);
}

int	parse_map(char *path, t_map *map)
{
	t_list	*content;
	char	*line;
	int		fd;
	char	**export;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (0);
	content = 0;
	line = get_next_line(fd);
	while (line)
	{
		line[ft_strlen(line) - 1] = 0;
		ft_lstadd_back(&content, ft_lstnew(line));
		line = get_next_line(fd);
	}
	export = lst_export(content);
	while (*export)
	{
		ft_printf("[%s]\n", export[0]);
		export++;
	}
	return (1);
}
