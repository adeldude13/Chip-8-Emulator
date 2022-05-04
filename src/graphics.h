#ifndef SDL_FILE
#define SDL_FILE

#include <inttypes.h>

// returns 1 on success;
int initGraphics(char *title, int width, int height, int tWidth, int tHeight);

int input(uint8_t *keys);

void SDL_UPDATE();

void SDL_EXIT();

#endif
