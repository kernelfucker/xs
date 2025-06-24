/* See LICENSE file for license details */
/* xs - minimal image viewer */
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <SDL2/SDL.h>
#include "lib/stb_image.h"

#define version "0.2"

void phelp(const char *s){
	printf("usage: %s image(.png, .jpeg etc..)\n", s);
	printf("options:\n");
	printf("  --version	show version information\n");
	printf("  --help	display this\n");
}	

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("usage: %s image(.png, .jpeg etc..)\n", argv[0]);
		return 1;
	}

	if(strcmp(argv[1], "--help") == 0){
		phelp(argv[0]);
		return 0;
	}

	if(strcmp(argv[1], "--version") == 0){
		printf("%s\n", version);
		return 0;
	}

	int w, h, ch;
	unsigned char *data = stbi_load(argv[1], &w, &h, &ch, 4);
	if(!data){
		printf("couldnt not image load %s\n", stbi_failure_reason());
		return 1;
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("sdl get error: %s\n", SDL_GetError());
		stbi_image_free(data);
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("xs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);
	SDL_UpdateTexture(tex, NULL, data, w * 4);

	int done = 0;
	SDL_Event e;
	while(!done){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				done = 1;
		}
		
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, tex, NULL, NULL);
		SDL_RenderPresent(ren);
		SDL_Delay(10);

	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	stbi_image_free(data);

	return 0;
}
