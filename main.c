#include "chip-8.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// Todo: Make This Fnuction Safe for Float Numbers. 
int toInt(char *number, size_t length) {
	int j = (int)length - 1;
	int res = 0;
	for(size_t i=0; i<length; i++) {
		res += ((int)(number[i])-48) * pow(10, j);
		j-=1;
	}
	return res;
}


int main(int argc, char **argv) {
	if(argc != 4) {
		printf("Usage: %s <Scale> <Delay> <ROM>", argv[0]);
		exit(0);
	}
	int scale = toInt(argv[1], strlen(argv[1]));
	int delay = toInt(argv[2], strlen(argv[2]));
	char *file = argv[3];
	initChip(file, scale, delay);	
	return 0;
}
