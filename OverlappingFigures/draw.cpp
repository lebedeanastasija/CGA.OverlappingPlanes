#include "draw.h"
#include "stdafx.h"

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(b) << 8) | g) << 8) | r)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{

	assert(NULL != surface);
	assert(x < SCREEN_WIDTH);
	assert(y < SCREEN_HEIGHT);

	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = pixel;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	assert(NULL != surface);
	assert(x < SCREEN_WIDTH);
	assert(y < SCREEN_HEIGHT);

	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

void drawRectangle(SDL_Surface *s, POINT r[4], Uint32 color) {
	line(s, r[0].x, r[0].y, r[1].x, r[1].y, 0x0000ff00);
	line(s, r[1].x, r[1].y, r[2].x, r[2].y, 0x0000ff00);
	line(s, r[2].x, r[2].y, r[3].x, r[3].y, 0x0000ff00);
	line(s, r[3].x, r[3].y, r[0].x, r[0].y, 0x0000ff00);
}

void drawTriangle(SDL_Surface *s, POINT tr[3], Uint32 color) {
	line(s, tr[0].x, tr[0].y, tr[1].x, tr[1].y, 0x000000ff);
	line(s, tr[1].x, tr[1].y, tr[2].x, tr[2].y, 0x000000ff);
	line(s, tr[2].x, tr[2].y, tr[0].x, tr[0].y, 0x000000ff);
}

void cleanRectangle(SDL_Surface *s, POINT r[4], Uint32 color) {
	line(s, r[0].x, r[0].y, r[1].x, r[1].y, 0x00000000);
	line(s, r[1].x, r[1].y, r[2].x, r[2].y, 0x00000000);
	line(s, r[2].x, r[2].y, r[3].x, r[3].y, 0x00000000);
	line(s, r[3].x, r[3].y, r[0].x, r[0].y, 0x00000000);
}

void cleanTriangle(SDL_Surface *s, POINT tr[3], Uint32 color) {
	line(s, tr[0].x, tr[0].y, tr[1].x, tr[1].y, 0x00000000);
	line(s, tr[1].x, tr[1].y, tr[2].x, tr[2].y, 0x00000000);
	line(s, tr[2].x, tr[2].y, tr[0].x, tr[0].y, 0x00000000);
}

void line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x2 >= x1 ? 1 : -1;
	int sy = y2 >= y1 ? 1 : -1;
	if (dy <= dx) {
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;

		if (x1 > -1 && y1 > -1 && x1 <= SCREEN_WIDTH && y1 <= SCREEN_HEIGHT) {
			put_pixel32(surface, x1, y1, color);
		}
		
		for (int x = x1 + sx, y = y1, i = 1; i <= dx; i++, x += sx) {
			if (d > 0) {
				d += d2; y += sy;
			}
			else
				d += d1;
			if (x > -1 && y > -1 && x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT) {
				put_pixel32(surface, x, y, color);
			}
			
		}
	}
	else {
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		if (x1 > -1 && y1 > -1 && x1 <= SCREEN_WIDTH && y1 <= SCREEN_HEIGHT) {
			put_pixel32(surface, x1, y1, color);
		}
		
		for (int x = x1, y = y1 + sy, i = 1; i <= dy; i++, y += sy) {
			if (d > 0) {
				d += d2; x += sx;
			}
			else
				d += d1;
			if (x > -1 && y > -1 && x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT) {
				put_pixel32(surface, x, y, color);
			}			
		}
	}	
}

