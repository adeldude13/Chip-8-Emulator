CC := clang

out := main


Args := -ggdb3 -O0 -o $(out)

Files := main.c chip-8.c



main: $(Files)
	$(CC) $(Args) $(Files)
