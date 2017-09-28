#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

void drawTriangle(SDL_Surface *s, int tr[4], Uint32 color);

void drawRectangle(SDL_Surface *s, int r[4], Uint32 color);

void line(SDL_Surface* surface, int x1, int y1, int x2, int y2, UINT32 color);

#endif