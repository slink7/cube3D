/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 19:09:00 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/26 19:09:00 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "incl.h"
#include "funct.h"

void	init_direction_noth_south(t_data *data, t_tile dir)
{
	if (dir == 'N')
	{
		data->dir_x = -1.0;
		data->dir_y = 0.0;
		data->plane_x = 0.0;
		data->plane_y = 0.66;
	}
	else if (dir == 'S')
	{
		data->dir_x = 1.0;
		data->dir_y = 0.0;
		data->plane_x = 0.0;
		data->plane_y = -0.66;
	}
}

void	init_direction_east_west(t_data *data, t_tile dir)
{
	if (dir == 'W')
	{
		data->dir_x = 0.0;
		data->dir_y = -1.0;
		data->plane_x = -0.66;
		data->plane_y = 0.0;
	}
	else if (dir == 'E')
	{
		data->dir_x = 0.0;
		data->dir_y = 1.0;
		data->plane_x = 0.66;
		data->plane_y = 0.0;
	}
}

int	init_player(t_data *data)
{
	int	i;
	int	j;

	j = -1;
	while (++j < data->map.height)
	{
		i = -1;
		while (++i < data->map.width)
		{
			if (ft_strchr("ENWS", data->map.content[j][i]) != 0)
			{
				init_direction_noth_south(data, data->map.content[j][i]);
				init_direction_east_west(data, data->map.content[j][i]);
				data->pos_x = j;
				data->pos_y = i;
				data->map.content[j][i] = '0';
				return (1);
			}
		}
	}
	return (0 & ft_fprintf(2, "Player spawn not found.\n"));
}
