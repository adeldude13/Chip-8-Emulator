CC := clang

out := main


Args := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm -o $(out)

Files := main.c chip-8.c graphics.c



main: $(Files)
	$(CC) $(Args) $(Files)
