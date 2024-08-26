/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 12:31:34 by scambier          #+#    #+#             */
/*   Updated: 2024/08/26 18:48:41 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define WIN_WIDTH 1920
# define WIN_HEIGHT 1080
# define MOVE_SPEED 0.2
# define MOUSE_SENS 0.005
# define TEXTURE_SIZE 64
# define PI 3.14159265358979323846

# define KEY_UP  			'z'
# define KEY_LEFT  			'q'
# define KEY_RIGHT 			'd'
# define KEY_DOWN  			's'

# define MAP_EMPTY	0
# define MAP_WALL	1
# define MAP_FLOOR	2

#define NUM_TEXTURES 4

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

typedef unsigned char	t_tile;

typedef struct s_image {
	char	*path;
	void	*addr;
	int		*data;
	char	*datachar;
	int		bits_per_pixel;
	int		line_length;
	int		width;
	int		height;
	int		endian;
}				t_image;

typedef struct s_rays {
    double	camera_x;
    double	ray_dir_x;
    double	ray_dir_y;
    double	delta_dist_x;
    double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
    double  step;
    double  tex_pos;
    double  wall_x;
    int		map_x;
    int		map_y;
    int		step_x;
    int		step_y;
	int		side;
	int 	line_height;
	int		draw_start;
	int		draw_end;
    int     tex_num;
    int     tex_x;
    int     tex_y;
} t_rays;

typedef struct s_mouse {
    int last_x;
    int last_y;
} t_mouse;

typedef struct s_map {
	t_tile	**content;
	int		width;
	int		height;
	t_image	wall_textures[NUM_TEXTURES];
	t_color	floor_color;
	t_color	ceiling_color;
}	t_map;

typedef struct s_data {
    void    *mlx_ptr;
    void    *win_ptr;
	int		world_map[24][24];
    t_map	map;
    double  pos_x;
    double  pos_y;
    double  dir_x;
    double  dir_y;
    double  plane_x;
    double  plane_y;
    t_image   texture[NUM_TEXTURES];
    int     *texture_buffer[NUM_TEXTURES];
    t_image   img;
	t_image	backbuffer;
    t_mouse *mouse;
} t_data;

#define TEXTURE_COUNT 4

/*t_tile	map_get_at(t_map *map, int x, int y);
void	map_set_at(t_map *map, int x, int y, t_tile v);*/

typedef struct s_world {
	t_map		map;
	t_object	player;
}	t_world;

/*
typedef struct s_map {
	int			content[16][18];
	int			width;
	int			height;
	t_image		wall_textures;
	t_color		floor_color;
	t_color		ceiling_color;
}	t_map;

typedef struct s_world {
	void			*mlx_ptr;
	void			*win_ptr;
	t_map			map;
	t_object		player;
	t_image			minimap;
	t_image			backbuffer;
	t_res			res;
}	t_world;
*/

#endif
