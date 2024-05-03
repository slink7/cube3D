/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 12:31:34 by scambier          #+#    #+#             */
/*   Updated: 2024/05/03 13:09:46 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define MAP_EMPTY	32
# define MAP_WALL	49
# define MAP_FLOOR	48

# define TEXTURE_NORTH	0
# define TEXTURE_EAST	1
# define TEXTURE_SOUTH	2
# define TEXTURE_WEST	3

typedef struct s_vec2f {
	float	x;
	float	y;
}	t_vec2f;

typedef struct s_object {
	t_vec2f	position;
	float	rotation;
}	t_object;

typedef struct s_map {
	char		**content;
	int			width;
	int			height;
	t_texture	wall_textures[4];
	t_color		floor_color;
	t_color		ceiling_color;
}	t_map;

typedef struct s_world {
	t_map		map;
	t_object	player;
}	t_world;

typedef struct s_texture {
	void	*content;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_texture;

typedef unsigned int	t_color;

#endif