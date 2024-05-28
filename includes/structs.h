/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 12:31:34 by scambier          #+#    #+#             */
/*   Updated: 2024/05/27 14:33:43 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define PI 3.14159265358979323846

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

# define NORTH	0
# define SOUTH	1
# define WEST	2
# define EAST	3

typedef unsigned int	t_color;

typedef struct s_vec2i {
	int	x;
	int	y;
}	t_vec2i;

typedef struct s_object {
	float	pos_x;
	float	pos_y;
	float	dir_x;
	float	dir_y;
	float	plane_x;
	float	plane_y;
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
	int		size;
}				t_image;

typedef struct s_map {
	int			content[16][18];
	int			width;
	int			height;
	t_image		wall_textures;
	t_color		floor_color;
	t_color		ceiling_color;
}	t_map;

typedef struct s_res
{
	int	width;
	int	height;
	int	tile_width;
	int	tile_height;
}	t_res;

typedef	struct s_raycasting
{
	float	ray_angle;
    float	rx;
	float	ry;
    float 	xdir;
	float	ydir;
    int		mx;
	int		my;
    float	distance;
    int 	r;
	int		px;
	int		py;
	int		mpx;
    int		mpy;
}	t_raycasting;



t_tile	map_get_at(t_map *map, int x, int y);
void	map_set_at(t_map *map, int x, int y, t_tile v);

typedef struct s_world {
	void			*mlx_ptr;
	void			*win_ptr;
	t_map			map;
	t_object		player;
	t_image			minimap;
	t_image			backbuffer;
	t_res			res;
	t_raycasting	rays_info;
}	t_world;

#endif