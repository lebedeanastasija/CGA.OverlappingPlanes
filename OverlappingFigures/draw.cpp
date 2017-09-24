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

