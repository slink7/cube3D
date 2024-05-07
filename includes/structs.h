/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 12:31:34 by scambier          #+#    #+#             */
/*   Updated: 2024/05/07 14:42:46 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define PI 3.1415926535

# define GREY	0x808080
# define WHITE	0xFFFFFF
# define TILE_SIZE 73

# define KEY_UP  			'z'
# define KEY_LEFT  			'q'
# define KEY_RIGHT 			'd'
# define KEY_DOWN  			's'

# define MAP_EMPTY	0
# define MAP_WALL	1
# define MAP_FLOOR	2

# define EAST	0
# define NORTH	1
# define WEST	2
# define SOUTH	3

typedef unsigned int	t_color;

typedef struct s_vec2i {
	int	x;
	int	y;
}	t_vec2i;

typedef struct s_object {
	float	x;
	float	y;
	float	z;
}	t_object;

typedef unsigned short	t_tile;

typedef struct s_image {
	void	*addr;
	char	*data;
	int		bits_per_pixel;
	int		line_length;
	int		width;
	int		height;
	int		endian;
}				t_image;

typedef struct s_map {
	int			content[7][7];
	int			width;
	int			height;
	t_image		wall_textures;
	t_color		floor_color;
	t_color		ceiling_color;
}	t_map;

t_tile	map_get_at(t_map *map, int x, int y);
void	map_set_at(t_map *map, int x, int y, t_tile v);

typedef struct s_world {
	void		*mlx_ptr;
	void		*win_ptr;
	t_map		map;
	t_object	player;
	t_image		minimap;
	t_image		backbuffer;
}	t_world;

#endif