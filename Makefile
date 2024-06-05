all :
	cc main.c parsing.c ftpng/*.c ftpng/defilter/*.c -g3 -Llibft/ -lft -Lmlx/ -lmlx -Ilibft -I./ -Imlx/ -Iftpng -lX11 -lXext -Iftpng/defilter/