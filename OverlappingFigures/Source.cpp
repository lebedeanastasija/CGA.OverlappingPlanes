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

	POINT r[4];
	POINT r_new[4];
	POINT tr[3];
	POINT tr_new[3];

	r[0].x = 200; //[ left top, right top, right bottom, left bottom ]
	r[0].y = 100; 
	r[1].x = 300; 
	r[1].y = 100;
	r[2].x = 300;  
	r[2].y = 200;
	r[3].x = 200; 
	r[3].y = 200;

	tr[0].x = 150; //left bottom, middle top, right bottom
	tr[0].y = 200;
	tr[1].x = 200; 
	tr[1].y = 50;
	tr[2].x = 250; 
	tr[2].y = 200;

	for (int i = 0; i < 3; i++) {
		tr_new[i] = tr[i];
	}
	for (int i = 0; i < 4; i++) {
		r_new[i] = r[i];
	}
	
	drawRectangle(gSurface, r, 0x0000ff00);
	drawTriangle(gSurface, tr, 0x000000ff);	

	while (!quit) {
		SDL_Event sdlEvent;
		while (SDL_PollEvent(&sdlEvent) != 0) {
			if (sdlEvent.type == SDL_QUIT || (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_ESCAPE)) quit = true;
			/*if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_DOWN)
			{
				r_new[0].y = r[3].y > ;
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
			}*/
			if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
				int x = sdlEvent.button.x;
				int y = sdlEvent.button.y;
		
				if (x >= r[0].y && x < r[1].x && y > r[0].y && y < r[2].y) {
					printf("Get rectangle\n");
					r_dragged = true;
					tr_dragged = false;
				}
				if (x >= tr[0].x && x < tr[1].x && y > tr[1].y && y < tr[0].y) {
					printf("Get triangle\n");
					tr_dragged = true;
					r_dragged = false;
				}
			}
			if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
				if (r_dragged) {
					printf("put rectangle\n");
					r_dragged = false;
				}
				if (tr_dragged) {
					printf("put triangle\n");
					tr_dragged = false;
				}
			}
			if (sdlEvent.type == SDL_MOUSEMOTION) {
				int x = sdlEvent.motion.x;
				int y = sdlEvent.motion.y;
				int dx = 0;
				int dy = 0;

				if (r_dragged == true) {
					printf("move rectangle\n");
					//move the the most top left point
					r_new[0].x = x > 0 && x < max_x - (r[1].x - r[0].x) ? x : r[0].x;
					r_new[0].y = y > 0 && y < max_y - (r[2].y - r[1].y) ? y : r[0].y;
					//move ather points
					dx = r_new[0].x - r[0].x;
					dy = r_new[0].y - r[0].y;					
					r_new[1].x = r[1].x + dx;
					r_new[1].y = r[1].y + dy;
					//minus for folowing point for rotating
					r_new[2].x = r[2].x + dx; 
					r_new[2].y = r[2].y + dy;
					r_new[3].x = r[3].x + dx;
					r_new[3].y = r[3].y + dy;
				}
				if (tr_dragged == true) {
					printf("move triangle\n");
					tr_new[1].x = x > 0 && x < max_x - (tr[2].x - tr[0].x) ? x : tr[1].x;
					tr_new[1].y = y > 0 && y < max_y - (tr[2].y - tr[1].y) ? y : tr[1].y;

					dx = tr_new[1].x - tr[1].x;
					dy = tr_new[1].y - tr[1].y;
					//minus for folowing point for rotating
					tr_new[0].x = tr[0].x + dx;
					tr_new[0].y = tr[0].y + dy;
					tr_new[2].x = tr[2].x + dx;
					tr_new[2].y = tr[2].y + dy;
				}
			}
		}
		
		SDL_RenderClear(gRenderer);
		SDL_LockSurface(gSurface);
		//cleanTriangle(gSurface, tr, 0x00000000);
		//cleanRectangle(gSurface, r, 0x00000000);	
		if (tr_dragged) {
			drawRectangle(gSurface, r_new, 0x0000ff00);
			drawTriangle(gSurface, tr_new, 0x000000ff);
		}
		if (r_dragged && !tr_dragged) {
			drawTriangle(gSurface, tr_new, 0x000000ff);
			drawRectangle(gSurface, r_new, 0x0000ff00);
		}

		for (int i = 0; i < 3; i++) {
			tr[i] = tr_new[i];
		}
		for (int i = 0; i < 4; i++) {
			r[i] = r_new[i];
		}
		SDL_UnlockSurface(gSurface);
		SDL_UpdateTexture(gTexture, NULL, gSurface->pixels, gSurface->pitch);
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
		SDL_RenderPresent(gRenderer);
	}

	close();
	return 0;
}
