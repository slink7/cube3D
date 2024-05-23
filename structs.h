/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 12:31:34 by scambier          #+#    #+#             */
/*   Updated: 2024/05/10 20:12:36 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define MAP_EMPTY	0
# define MAP_WALL	1
# define MAP_FLOOR	2

# define EAST	0
# define NORTH	1
# define WEST	2
# define SOUTH	3

typedef struct s_vec2f {
	float	x;
	float	y;
}	t_vec2f;

typedef struct s_object {
	t_vec2f	position;
	float	rotation;
}	t_object;

typedef unsigned short	t_tile;

typedef struct s_image {
	void	*content;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}				t_image;

typedef unsigned int	t_color;

typedef struct s_map {
	t_tile	**content;
	int		width;
	int		height;
	t_image	wall_textures[4];
	t_color	floor_color;
	t_color	ceiling_color;
}	t_map;

t_tile	map_get_at(t_map *map, int x, int y);
void	map_set_at(t_map *map, int x, int y, t_tile v);

typedef struct s_world {
	t_map		map;
	t_object	player;
}	t_world;

typedef unsigned char	t_uint8;
typedef unsigned short	t_uint16;
typedef unsigned int	t_uint32;
typedef unsigned long	t_uint64;

#endif