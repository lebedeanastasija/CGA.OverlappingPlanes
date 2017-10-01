#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

void drawTriangle(SDL_Surface *s, POINT tr[3], Uint32 color);

void drawRectangle(SDL_Surface *s, POINT r[4], Uint32 color);

void cleanTriangle(SDL_Surface *s, POINT tr[3], Uint32 color);

void cleanRectangle(SDL_Surface *s, POINT r[4], Uint32 color);

void line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);

#endif