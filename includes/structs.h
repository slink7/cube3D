/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 12:31:34 by scambier          #+#    #+#             */
/*   Updated: 2024/05/03 15:19:24 by ymostows         ###   ########.fr       */
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

typedef unsigned int	t_color;

typedef struct s_vec2f {
	float	x;
	float	y;
}	t_vec2f;

typedef struct s_object {
	t_vec2f	position;
	float	rotation;
}	t_object;

typedef struct s_mlx {
	void	*mlx_ptr;
	void	*win_ptr;
}	t_mlx;

typedef unsigned short	t_tile;

typedef struct s_texture {
	void	*content;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_texture;

typedef struct s_map {
	t_tile		content[7][7];
	int			width;
	int			height;
	t_texture	*wall_textures[4];
	t_color		floor_color;
	t_color		ceiling_color;
}	t_map;

typedef struct s_res {
	t_map		map;
	t_object	player;
}	t_res;

t_tile	map_get_at(t_map *map, int x, int y);
void	map_set_at(t_map *map, int x, int y, t_tile v);

typedef struct s_world {
	t_map		map;
	t_object	player;
}	t_world;

#endif