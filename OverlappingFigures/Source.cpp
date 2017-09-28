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
	bool tr_dragged = false;
	if (!init()) return 1;

	gSurface = SDL_CreateRGBSurface(0, 600, 600, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, gSurface);
	if (gTexture == NULL) return 1;

	bool quit = false;

	Sint16 max_x = 600;
	Sint16 max_y = 600;

	/*rectangle structure:
	[0] - top left point x,
	[1] - top left point y,
	[2] - rectangle width,
	[3] - rectangle height
	*/
	int r[4];
	int r_new[4];
	int tr[4];
	int tr_new[4];

	r[0] = max_x/2 - 50;
	r[1] = max_y / 2 - 50;
	r[2] = 100;
	r[3] = 100;

	tr[0] = max_x / 3 - 100;
	tr[1] = max_y / 3 - 100;
	tr[2] = 100;
	tr[3] = 100;

	for (int i = 0; i < 4; i++) {
		r_new[i] = r[i];
		tr_new[i] = tr[i];
	}
	
	drawRectangle(gSurface, r, 0x000000FF);
	drawTriangle(gSurface, tr, 0x0000FF00);
	line(gSurface, 100, 100, 300, 300, 0x0000000F);

	while (!quit) {
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) != 0) {
			if (sdlEvent.type == SDL_QUIT || (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_ESCAPE)) quit = true;
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_DOWN)
			{
				r_new[1] = r[1] + r[3] + 1 < max_y ? r[1] + 1 : r[1];
				printf("Key down\n");
			}
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_UP)
			{
				r_new[1] = r[1] > 0 ? r[1] - 1 : r[1];
			}
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_LEFT)
			{
				r_new[0] = r[0] > 0 ? r[0] - 1 : r[0];
			}
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_RIGHT)
			{
				r_new[0] = r[0] + r[3] + 1 < max_x ? r[0] + 1 : r[0];
			}
			if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
				int x = sdlEvent.button.x;
				int y = sdlEvent.button.y;
		
				if (x >= r[0] && x < r[0] + r[2] && y > r[1] && y < r[1] + r[3]) {
					r_dragged = true;
					tr_dragged = false;
				}
				if (x >= tr[0] && x < tr[0] + tr[2] && y > tr[1] && y < tr[1] + tr[3]) {
					tr_dragged = true;
					r_dragged = false;
				}
			}
			if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
				if (r_dragged) {
					r_dragged = false;
				}
				if (tr_dragged) {
					tr_dragged = false;
				}
			}
			if (sdlEvent.type == SDL_MOUSEMOTION) {
				int x = sdlEvent.motion.x;
				int y = sdlEvent.motion.y;

				if (r_dragged == true) {
					r_new[0] = x > 0 && x < max_x - r[2] ? x : r[0];
					r_new[1] = y > 0 && y < max_y - r[3] ? y : r[1];
				}
				if (tr_dragged == true) {
					tr_new[0] = x > 0 && x < max_x - tr[2] ? x : tr[0];
					tr_new[1] = y > 0 && y < max_y - tr[3] ? y : tr[1];
				}
			}
		}
		
		SDL_RenderClear(gRenderer);
		SDL_LockSurface(gSurface);
		drawRectangle(gSurface, r, 0x00000000);
		drawRectangle(gSurface, r_new, 0x000000ff);
		drawTriangle(gSurface, tr, 0x00000000);
		drawTriangle(gSurface, tr_new, 0x0000ff00);
		line(gSurface, 100, 100, 300, 300, 0x0000ff00);
		for (int i = 0; i < 4; i++) {
			r[i] = r_new[i];
			tr[i] = tr_new[i];
		}
		SDL_UnlockSurface(gSurface);
		SDL_UpdateTexture(gTexture, NULL, gSurface->pixels, gSurface->pitch);
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}
