all :
	cc main.c parsing.c ftpng/bit_stream.c ftpng/inflate.c ftpng/load_png.c -g3 -Llibft/ -lft -Ilibft -I./ -Iftpng