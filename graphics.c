#include "graphics.h"
#include <SDL2/SDL.h>


SDL_Window *win;
SDL_Renderer *rend;
SDL_Texture *texture;

void SDL_UPDATE(void *buffer, int p) {
	SDL_UpdateTexture(texture, NULL, buffer, p);
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, texture, NULL, NULL);
	SDL_RenderPresent(rend);
}

int input(uint8_t keys[]) {
	int EXIT = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					EXIT = 1;
				} break;

				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							EXIT = 1;
						} break;

						case SDLK_x:
						{
							keys[0] = 1;
						} break;

						case SDLK_1:
						{
							keys[1] = 1;
						} break;

						case SDLK_2:
						{
							keys[2] = 1;
						} break;

						case SDLK_3:
						{
							keys[3] = 1;
						} break;

						case SDLK_q:
						{
							keys[4] = 1;
						} break;

						case SDLK_w:
						{
							keys[5] = 1;
						} break;

						case SDLK_e:
						{
							keys[6] = 1;
						} break;

						case SDLK_a:
						{
							keys[7] = 1;
						} break;

						case SDLK_s:
						{
							keys[8] = 1;
						} break;

						case SDLK_d:
						{
							keys[9] = 1;
						} break;

						case SDLK_z:
						{
							keys[0xA] = 1;
						} break;

						case SDLK_c:
						{
							keys[0xB] = 1;
						} break;

						case SDLK_4:
						{
							keys[0xC] = 1;
						} break;

						case SDLK_r:
						{
							keys[0xD] = 1;
						} break;

						case SDLK_f:
						{
							keys[0xE] = 1;
						} break;

						case SDLK_v:
						{
							keys[0xF] = 1;
						} break;
					}
				} break;

				case SDL_KEYUP:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_x:
						{
							keys[0] = 0;
						} break;

						case SDLK_1:
						{
							keys[1] = 0;
						} break;

						case SDLK_2:
						{
							keys[2] = 0;
						} break;

						case SDLK_3:
						{
							keys[3] = 0;
						} break;

						case SDLK_q:
						{
							keys[4] = 0;
						} break;

						case SDLK_w:
						{
							keys[5] = 0;
						} break;

						case SDLK_e:
						{
							keys[6] = 0;
						} break;

						case SDLK_a:
						{
							keys[7] = 0;
						} break;

						case SDLK_s:
						{
							keys[8] = 0;
						} break;

						case SDLK_d:
						{
							keys[9] = 0;
						} break;

						case SDLK_z:
						{
							keys[0xA] = 0;
						} break;

						case SDLK_c:
						{
							keys[0xB] = 0;
						} break;

						case SDLK_4:
						{
							keys[0xC] = 0;
						} break;

						case SDLK_r:
						{
							keys[0xD] = 0;
						} break;

						case SDLK_f:
						{
							keys[0xE] = 0;
						} break;

						case SDLK_v:
						{
							keys[0xF] = 0;
						} break;
					}
				} break;
			}
		}

		return EXIT;
}

void SDL_EXIT() {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
}


int initGraphics(char *title, int w, int h, int tw, int th) {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL Failed To Init: %s", SDL_GetError());
		return 0;
	}


	if((win = SDL_CreateWindow(title, 0, 0, w, h, SDL_WINDOW_SHOWN)) == NULL) {
		printf("SDL Faild To Create Window: %s", SDL_GetError());	
		return 0;
	}


	if((rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		printf("SDL Failed To Create a Renderer: %s\n", SDL_GetError());
		return 0;
	}

	if((texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, tw, th)) == NULL) {
		printf("SDL Failed To Create a Texture: %s", SDL_GetError());
		return 0;
	}
	return 1;
}
