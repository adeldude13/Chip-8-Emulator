#include "chip-8.h"
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc == 2) {
		initChip(argv[1]);
	} else {
		printf("Usage: chip8 [file.ch8]");
	}
	return 0;
}
