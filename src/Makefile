CC := clang
scale := 10
rom := ../ROMS/Airplane.ch8
out := ../bin/main
Args := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm -o $(out)
CFiles = main.c chip-8.c graphics.c
Headers := chip-8.h graphics.h
Files := $(CFiles) $(Headers)

main: $(Files)
	$(CC) $(Args) $(CFiles)
run:
	$(out) $(scale) $(rom)
