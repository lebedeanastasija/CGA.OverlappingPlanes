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

void draw(SDL_Surface *s)
{
	// ��� ���
	// ...
	for (int i = 30; i < 100; i++)
		for (int j = 30; j < 100; j++)
			put_pixel32(s, i, j, 0x00FF0000);

	// ������ ����� � HEX ����:
	//     0x00RRGGBB
	//  ��� R: �� 00 �� FF
	//      G: �� 00 �� FF
	//      B: �� 00 �� FF

	for (int i = 100; i < 200; i++)
		for (int j = 100; j < 180; j++)
			put_pixel32(s, i, j, RGB32(0, 255, 0));

	// ��� ��������� ������ ����� �������� ��� �����:
	//   RGB32(0, 255, 0) ������������ ������ 0x0000FF00
}

 bool drawRectangle(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {
	if (Surf_Dest == NULL || Surf_Src == NULL) {
		return false;
	}

	SDL_Rect DestR;

	DestR.x = X;
	DestR.y = Y;

	SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);

	return true;
}