#include "stdafx.h"
#include "draw.h"

bool init();
void close();

SDL_Window*		gWindow = NULL;
SDL_Renderer*	gRenderer = NULL;
SDL_Texture*	gTexture = NULL;
SDL_Texture*	gTexture_2 = NULL;
SDL_Surface*	gSurface = NULL;
SDL_Surface*	gSurface_2 = NULL;

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

	gWindow = SDL_CreateWindow("Overlapping planes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) return false;

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL) return false;

	SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
	return true;
}

void close() {
	SDL_DestroyTexture(gTexture);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gTexture = NULL;
	gRenderer = NULL;
	gWindow = NULL;
	SDL_Quit();
}

void drawTriangle() {
}

int main(int argc, char *argv[]) {
	bool r_dragged = false; 
	bool r_2_dragged = false;
	if (!init()) return 1;

	gSurface = SDL_CreateRGBSurface(0, 600, 600, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	if (gTexture == NULL) return 1;

	bool quit = false;

	SDL_Rect r;
	SDL_Rect r_new;
	SDL_Rect r_2;
	SDL_Rect r_2_new;

	Sint16 leftright = 1;
	Sint16 max_x = 600;
	Sint16 max_y = 600;
	int nextstep = 1;

	r.x = max_x / 2 - 50;
	r.y = max_y / 2 - 50;
	r.w = 100;
	r.h = 100;
	r_new = r;

	r_2.x = max_x / 2;
	r_2.y = max_y / 2;
	r_2.w = 100;
	r_2.h = 100;
	r_2_new = r_2;
	
	SDL_FillRect(gSurface, &r, 0x000000FF);
	SDL_FillRect(gSurface, &r_2, 0x0000FF00);

	while (!quit) {
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) != 0) {
			if (sdlEvent.type == SDL_QUIT || (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_ESCAPE)) quit = true;
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_DOWN)
			{
				r_new.y = r.y + r.h + 1 < max_y ? r.y + 1 : r.y;
				printf("Key down\n");
			}
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_UP)
			{
				r_new.y = r.y > 0 ? r.y - 1 : r.y;
			}
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_LEFT)
			{
				r_new.x = r.x > 0 ? r.x - 1 : r.x;
			}
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_RIGHT)
			{
				r_new.x = r.x + r.w + 1 < max_x ? r.x + 1 : r.x;
			}
			if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
				int x = sdlEvent.button.x;
				int y = sdlEvent.button.y;

				if (x > r.x && x < r.x + r.w && y > r.y && y < r.y + r.h) {
					r_dragged = true;
				}
				if (x > r_2.x && x < r_2.x + r_2.w && y > r_2.y && y < r_2.y + r_2.h) {
					r_2_dragged = true;
				}
			}
			if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
				if (r_dragged) {
					r_dragged = false;
				}
				if (r_2_dragged) {
					r_2_dragged = false;
				}
			}
			if (sdlEvent.type == SDL_MOUSEMOTION) {
				int x = sdlEvent.motion.x;
				int y = sdlEvent.motion.y;

				if (r_dragged == true) {
					r_new.x = x > 0 && x < max_x - r.w ? x : r.x;
					r_new.y = y > 0 && y < max_y - r.h ? y : r.y;
				}
				if (r_2_dragged == true) {
					r_2_new.x = x > 0 && x < max_x - r_2.w ? x : r_2.x;
					r_2_new.y = y > 0 && y < max_y - r_2.h ? y : r.y;
				}
			}
		}
		
		SDL_RenderClear(gRenderer);
		SDL_LockSurface(gSurface);
		SDL_FillRect(gSurface, &r, 0x00000000);
		SDL_FillRect(gSurface, &r_2, 0x00000000);
		r = r_new; 
		r_2 = r_2_new;
		SDL_FillRect(gSurface, &r, 0x000000FF);		
		SDL_FillRect(gSurface, &r_2, 0x0000FF00);
		SDL_UnlockSurface(gSurface);
		SDL_UpdateTexture(gTexture, NULL, gSurface->pixels, gSurface->pitch);
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}
