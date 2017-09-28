#include "draw.h"
#include "stdafx.h"

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(b) << 8) | g) << 8) | r)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	assert(NULL != surface);

	/*assert(x < SCREEN_WIDTH);
	assert(y < SCREEN_HEIGHT);*/

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

void drawRectangle(SDL_Surface *s, int r[4], Uint32 color) {
	for (int i = r[0]; i < r[0] + r[2]; i++)
		for (int j = r[1]; j < r[1] + r[3]; j++)
			put_pixel32(s, i, j, color);
}

void drawTriangle(SDL_Surface *s, int r[4], Uint32 color) {
	for (int i = r[0], k = 0; i < r[0] + r[2]; i++, k++)
		for (int j = r[1] + k; j < r[1] + r[3] - k; j++)
			put_pixel32(s, i, j, color);
}

void line(SDL_Surface* surface, int x1, int y1, int x2, int y2, UINT32 color)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x2 >= x1 ? 1 : -1;
	int sy = y2 >= y1 ? 1 : -1;
	if (dy <= dx) {
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;
		put_pixel32(surface, x1, y1, color);
		for (int x = x1 + sx, y = y1, i = 1; i <= dx; i++, x += sx) {
			if (d > 0) {
				d += d2; y += sy;
			}
			else
				d += d1;
			put_pixel32(surface, x, y, color);
		}
	}
	else {
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		put_pixel32(surface, x1, y1, color);
		for (int x = x1, y = y1 + sy, i = 1; i <= dy; i++, y += sy) {
			if (d > 0) {
				d += d2; x += sx;
			}
			else
				d += d1;
			put_pixel32(surface, x, y, color);
		}
	}
}

