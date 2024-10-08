/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:42:19 by scambier          #+#    #+#             */
/*   Updated: 2024/08/29 15:03:39 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "structs.h"

int		load_map(char *path, t_map *map);
int		unload_map(t_map *map);
int		check_walls(t_map *map);
int		load_map_textures(void *mlx, t_map *map);

#endif