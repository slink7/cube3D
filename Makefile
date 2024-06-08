all :
	cc main.c parsing.c -g3 -Llibft/ -lft -Lmlx/ -lmlx -Ilibft -I./ -Imlx/ -lX11 -lXext

update :
	git submodule update --init --remote
	cd libft ; make