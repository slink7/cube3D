/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funct.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymostows <ymostows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 18:17:28 by ymostows          #+#    #+#             */
/*   Updated: 2024/08/17 18:17:28 by ymostows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCT_H
# define FUNCT_H

/*t_tile	map_get_at(t_map *map, int x, int y);
void	map_set_at(t_map *map, int x, int y, t_tile v);*/
void	move_forward(t_data *data);
void	move_backward(t_data *data);
void	strafe_right(t_data *data);
void	strafe_left(t_data *data);
int		on_keyup(int keycode, t_data *data);
int		on_keydown(int keycode, t_data *data);
void	rotate_view(t_data *data, double angle);
int		mouse_move(int x, int y, t_data *data);
void	put_pixel(t_image *img, int x, int y, int color);
void	init_imgs(t_data *data, t_image *img, int width, int heigth);
int		load_texture(t_data *data, int index, char *path);
int		init_textures(t_data *data);
void	select_texture(t_rays *rays);
void	handle_textures(t_rays *vars);
int		destroy_textures(t_data *data, int n);
void	calcul_delta_dist(t_data *data, t_rays *vars, int x);
void	calcul_side_dist(t_data *data, t_rays *vars);
void	calcul_wall_pos(t_data *data, t_rays *vars);
void	calcul_wall_size(t_data *data, t_rays *vars);
void	draw_walls(t_data *data, t_rays *vars, int x);
void	render_floor_ceiling(t_data *data);
int		render_frame(t_data *data);
int		close_game(t_data *data);
int		init_player(t_data *data);
int		free_all(t_data *data);

#endif