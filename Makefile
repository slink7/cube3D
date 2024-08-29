# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scambier <scambier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/03 14:57:20 by ymostows          #+#    #+#              #
#    Updated: 2024/08/29 16:12:13 by scambier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	cube3d

CC			=	cc

FLAG		=	-Wall -Werror -Wextra -g3

LIBFT_PATH	=	./libft/

LIBFT_FILE	=	libft.a

MLX_FILE	=	libmlx.a

LIBFT_LIB	=	$(addprefix $(LIBFT_PATH), $(LIBFT_FILE))

MLX_FLAG	=	-lX11 -lXext -lm

MLX_PATH	=	./minilibx-linux/

MLX_LIB		=	$(addprefix $(MLX_PATH), $(MLX_FILE))

MLX_EX		=	$(MLX_LIB) $(MLX_FLAG)

SRCS		=	render.c mouvements.c mouse_mouvs.c images_utils.c manage_textures.c raycasting.c render_walls.c quit.c load_map.c parsing_utils.c check_walls.c init_player.c main.c 

SRC_DIR		=	./srcs/

INC_DIR		=	./includes/

SRC			=	$(addprefix $(SRC_DIR),$(SRCS))

OBJ			=	$(SRC:.c=.o)

.c.o:
	$(CC) $(FLAG) -c $< -o $@ -I$(INC_DIR) -Ilibft/ -Iminilibx-linux/

all: $(NAME)

libft/libft.a:
	make -C $(LIBFT_PATH)

minilibx-linux/libmlx.a:
	make -sC $(MLX_PATH)

$(NAME): libft/libft.a minilibx-linux/libmlx.a $(OBJ)
	$(CC) $(OBJ) $(LIBFT_LIB) $(MLX_EX) -o $(NAME)

clean:
	make clean -sC $(MLX_PATH)
	make clean -sC $(LIBFT_PATH)
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_PATH)

re: fclean all

.PHONY: all clean fclean re