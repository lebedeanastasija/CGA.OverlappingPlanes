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
	POINT w[4];

	int r_index = 0;
	int tr_index = 1;

	r[0].x = 210; //[ left top, right top, right bottom, left bottom ]
	r[0].y = 60; 
	r[1].x = 310; 
	r[1].y = 60;
	r[2].x = 310;  
	r[2].y = 160;
	r[3].x = 210; 
	r[3].y = 160;

	tr[0].x = 150; //left bottom, middle top, right bottom
	tr[0].y = 190;
	tr[1].x = 200; 
	tr[1].y = 40;
	tr[2].x = 250; 
	tr[2].y = 190;

	w[0].x = 20; //[ left top, right top, right bottom, left bottom ]
	w[0].y = 20;
	w[1].x = 580;
	w[1].y = 20;
	w[2].x = 580;
	w[2].y = 580;
	w[3].x = 20;
	w[3].y = 580;

	for (int i = 0; i < 3; i++) {
		tr_new[i] = tr[i];
	}
	for (int i = 0; i < 4; i++) {
		r_new[i] = r[i];
	}
	
	drawRectangle(gSurface, w, r, 0x0000ff00);
	drawTriangle(gSurface, w, tr, 0x000000ff);
	drawRectangle(gSurface, w, w, 0x00ff0000);

	POINT **points = getIntersectionPoints(r, tr);
	POINT **lines = getUnvisibleLines(points, r, tr, r_index, tr_index);

	POINT **points1 = getIntersectionPoints(w, tr);
	POINT **lines1 = getUnvisibleLines(points, r, tr, r_index, tr_index);

	bool last_founded = false;
	for (int i = 0; i < 3 && !last_founded; i++) {
		if (lines[i] == NULL || !pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, lines[i][0]) || !pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, lines[i][1])) {
			last_founded = true;
		} else {
			line(gSurface, w, lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, 0x00ff0000);
		}
	}
	double angle = M_PI / 180;
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
			}*/
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_LEFT)
			{
				if (r_index == 1) {
					r_new[0] = GetPoint(r[0], angle, GetRectangleGeometricCenter(r_new));
					r_new[1] = GetPoint(r[1], angle, GetRectangleGeometricCenter(r_new));
					r_new[2] = GetPoint(r[2], angle, GetRectangleGeometricCenter(r_new));
					r_new[3] = GetPoint(r[3], angle, GetRectangleGeometricCenter(r_new));
				}
				if (tr_index == 1) {
					tr_new[0] = GetPoint(tr[0], angle, GetTriangleGeometricCenter(tr_new));
					tr_new[1] = GetPoint(tr[1], angle, GetTriangleGeometricCenter(tr_new));
					tr_new[2] = GetPoint(tr[2], angle, GetTriangleGeometricCenter(tr_new));
				}
			}
			if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.keysym.sym == SDLK_RIGHT)
			{
				if (r_index == 1) {
					r_new[0] = GetPoint(r[0], -1 * angle, GetRectangleGeometricCenter(r_new));
					r_new[1] = GetPoint(r[1], -1 * angle, GetRectangleGeometricCenter(r_new));
					r_new[2] = GetPoint(r[2], -1 * angle, GetRectangleGeometricCenter(r_new));
					r_new[3] = GetPoint(r[3], -1 * angle, GetRectangleGeometricCenter(r_new));
				}
				if (tr_index == 1) {
					tr_new[0] = GetPoint(tr[0], -1 * angle, GetTriangleGeometricCenter(tr_new));
					tr_new[1] = GetPoint(tr[1], -1 * angle, GetTriangleGeometricCenter(tr_new));
					tr_new[2] = GetPoint(tr[2], -1 * angle, GetTriangleGeometricCenter(tr_new));
				}
			}


			if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
				int x = sdlEvent.button.x;
				int y = sdlEvent.button.y;
		
				if (x >= r[0].y && x < r[1].x && y > r[0].y && y < r[2].y) {
					if (x >= tr[0].x && x < tr[1].x && y > tr[1].y && y < tr[0].y) {
						if (r_index == 1) {
							r_dragged = true;
							r_index = 1;
							tr_index = 0;
						}
						else {
							tr_dragged = true;
							tr_index = 1;
							r_index = 0;
						}						
					}
					else {
						r_dragged = true;
						r_index = 1;
						tr_index = 0;
					}
				}
				else if (x >= tr[0].x && x < tr[1].x && y > tr[1].y && y < tr[0].y) {
					tr_dragged = true;
					tr_index = 1;
					r_index = 0;
				}
			}
			if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
				r_dragged = false;
				tr_dragged = false;
				if (r_dragged) {
					r_index = 1;
					tr_index = 0;
				}
				if (tr_dragged) {
					tr_index = 1;
					r_index = 0;			
				}
			}
			if (sdlEvent.type == SDL_MOUSEMOTION) {
				int x = sdlEvent.motion.x;
				int y = sdlEvent.motion.y;
				int dx = 0;
				int dy = 0;

				if (r_dragged == true) {
		
					r_new[0].x = x > 0 && x < max_x - (r[1].x - r[0].x) ? x : r[0].x;
					r_new[0].y = y > 0 && y < max_y - (r[2].y - r[1].y) ? y : r[0].y;
				
					dx = r_new[0].x - r[0].x;
					dy = r_new[0].y - r[0].y;					
					r_new[1].x = r[1].x + dx;
					r_new[1].y = r[1].y + dy;
					r_new[2].x = r[2].x + dx; 
					r_new[2].y = r[2].y + dy;
					r_new[3].x = r[3].x + dx;
					r_new[3].y = r[3].y + dy;
				}
				if (tr_dragged == true) {
					tr_new[1].x = x > 0 && x < max_x - (tr[2].x - tr[0].x) ? x : tr[1].x;
					tr_new[1].y = y > 0 && y < max_y - (tr[2].y - tr[1].y) ? y : tr[1].y;

					dx = tr_new[1].x - tr[1].x;
					dy = tr_new[1].y - tr[1].y;
					tr_new[0].x = tr[0].x + dx;
					tr_new[0].y = tr[0].y + dy;
					tr_new[2].x = tr[2].x + dx;
					tr_new[2].y = tr[2].y + dy;
				}
			}
		}
		
		SDL_RenderClear(gRenderer);
		SDL_LockSurface(gSurface);

		cleanTriangle(gSurface, tr, 0x00000000);
		cleanRectangle(gSurface, r, 0x00000000);

		Uint32 tr_color = 0x000000ff;
		Uint32 r_color = 0x0000ff00;

		if (!tr_dragged && !r_dragged) {
			if (r_index == 1) {
				drawTriangle(gSurface, w, tr_new, tr_color);
				drawRectangle(gSurface, w, r_new, r_color);
			}
			else if (tr_index == 1) {
				drawRectangle(gSurface, w, r_new, r_color);
				drawTriangle(gSurface, w, tr_new, tr_color);
			}
		}
		else {
			if (tr_dragged) {
				drawRectangle(gSurface, w, r_new, r_color);
				drawTriangle(gSurface, w, tr_new, tr_color);
				int r_index = 0;
				int tr_index = 1;
			}
			if (r_dragged) {
				drawTriangle(gSurface, w, tr_new, tr_color);
				drawRectangle(gSurface, w, r_new, r_color);
				int tr_index = 0;
				int r_index = 1;
			}
		}	
		

		/*for (int i = 0; i < 3 && !last_founded; i++) {
			if (lines[i] == NULL || !pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, lines[i][0]) || !pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, lines[i][1])) {
				last_founded = true;
			}
			else {
				line(gSurface, lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, 0x00000000);
			}
		}*/

		points = getIntersectionPoints(r_new, tr_new);
		lines = getUnvisibleLines(points, r_new, tr_new, r_index, tr_index);
		last_founded = false;

		for (int i = 0; i < 3 && !last_founded; i++) {
			if (lines[i] == NULL || !pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, lines[i][0]) || !pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, lines[i][1])) {
				last_founded = true;
			}
			else {
				line(gSurface, w, lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, 0x00ff0000);
			}
		}

		drawRectangle(gSurface, w, w, 0x00ff0000);

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
