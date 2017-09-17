#include "stdafx.h"
#include "draw.h"

bool init();
void close();

SDL_Window*		gWindow = NULL;
SDL_Renderer*	gRenderer = NULL;
SDL_Texture*	gTexture = NULL;
SDL_Surface*	gSurface = NULL;

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

int main(int argc, char *argv[]) {
	bool r_dragged = false;
	if (!init()) return 1;
	gSurface = SDL_CreateRGBSurface(0, 600, 600, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	if (gTexture == NULL) return 1;

	bool quit = false;

	SDL_Rect r;
	SDL_Rect r_new;

	Sint16 leftright = 1;
	Sint16 max_x = 600;
	Sint16 max_y = 600;
	int nextstep = 1;

	r.x = max_x / 2 - 50;
	r.y = max_y / 2 - 50;
	r.w = 100;
	r.h = 100;
	r_new = r;
	
	SDL_FillRect(gSurface, &r, 0x000000FF); 


	while (!quit) {
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) != 0) {
			//printf("%d\n", sdlEvent.type);
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
				printf("x: %d, y: %d, r_x: %d, r_y: %d\n", x, y, r.x, r.y);

				if (x > r.x && x < r.x + r.w && y > r.y && y < r.y + r.h) {
					r_dragged = true;
					printf("%d\n", r_dragged);
				}
			}
			if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
				int x = sdlEvent.button.x;
				int y = sdlEvent.button.y;
				printf("%d\n", r_dragged);

				if (r_dragged) {
					r_dragged = false;
				}
			}
			if (sdlEvent.type == SDL_MOUSEMOTION) {
				printf("Move\n");
				int x = sdlEvent.motion.x;
				int y = sdlEvent.motion.y;

				if (r_dragged == true) {
					r_new.x = x > 0 && x < max_x - r.w ? x : r.x;
					r_new.y = y > 0 && y < max_y - r.h ? y : r.y;
				}
			}
		}

		/*SDL_RenderClear(gRenderer);
		draw(gSurface);
		SDL_UpdateTexture(gTexture, NULL, gSurface->pixels, gSurface->pitch);
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
		SDL_RenderPresent(gRenderer);*/
		
		/*r_new.x = r.x + 1 * leftright;
		if (r_new.x < 0 || r_new.x + r.w > max_x)
		{ 
			leftright = -leftright;
		}*/
		
		SDL_RenderClear(gRenderer);
		SDL_LockSurface(gSurface);
		SDL_FillRect(gSurface, &r, 0x00000000);
		r = r_new; 
		SDL_FillRect(gSurface, &r, 0x000000FF);
		SDL_UnlockSurface(gSurface);
		SDL_UpdateTexture(gTexture, NULL, gSurface->pixels, gSurface->pitch);
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
		SDL_RenderPresent(gRenderer);
		//SDL_UpdateRect(screen, 0, 0, max_x, max_y);
	}

	close();
	return 0;
}
