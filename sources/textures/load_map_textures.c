/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map_textures.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scambier <scambier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 19:51:16 by scambier          #+#    #+#             */
/*   Updated: 2024/06/29 20:15:45 by scambier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"
#include "mlx.h"

static int	load_texture(void *mlx, t_image *img)
{
	if (!ft_strncmp(img->path + ft_strlen(img->path) - 4, ".xpm", 5))
	{
		img->content = mlx_xpm_file_to_image(mlx, img->path, &img->width, &img->height);
		if (img->content == 0)
			return (ft_fprintf(2, "Error: %s\n", img->path) & 0);
		img->addr = mlx_get_data_addr(img->content, &img->bits_per_pixel, &img->line_length, &img->endian);
		return (1);
	}
	else if (!ft_strncmp(img->path + ft_strlen(img->path) - 4, ".png", 5))
		return (ft_fprintf(2, "Error: PNG not yet supported\n") & 0);
	return (ft_fprintf(2, "Error: %s not supported\n", ft_strrchr(img->path, '.')) & 0);
}

int	load_map_textures(void *mlx, t_map *map)
{
	int	k;

	k = -1;
	while (++k < TEXTURE_COUNT)
		load_texture(mlx, map->wall_textures + k);
	return (1);
}