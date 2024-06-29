/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:37:50 by scambier          #+#    #+#             */
/*   Updated: 2024/06/29 19:38:44 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"	

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