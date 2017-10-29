#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

void drawTriangle(SDL_Surface *s, POINT w[4], POINT tr[3], Uint32 color);

void drawRectangle(SDL_Surface *s, POINT w[4], POINT r[4], Uint32 color);

void cleanTriangle(SDL_Surface *s, POINT tr[3], Uint32 color);

void cleanRectangle(SDL_Surface *s, POINT r[4], Uint32 color);

void line(SDL_Surface* surface, POINT w[4], int x1, int y1, int x2, int y2, Uint32 color, bool windowCheck);

POINT getIntersection(POINT p1, POINT p2, POINT p3, POINT p4);

POINT** getIntersectionPoints(POINT r[4], POINT tr[3]);

bool checkPointOnLines(POINT p, POINT p1, POINT p2, POINT p3, POINT p4);

bool pointInMatrix(POINT **m, POINT p);

POINT **getUnvisibleLines(POINT ** interseptionMatrix, POINT r[4], POINT tr[3], int r_index, int tr_index);

bool pointInWindow(int width, int height, POINT p);

POINT getPointInRectangle(POINT r[4], POINT p1, POINT p2);

bool pointOutOfWindow(POINT w[4], POINT p);

POINT getPointInTriangle(POINT tr[3], POINT p1, POINT p2);

POINT GetPoint(POINT p, double angle, POINT center);

POINT GetTriangleGeometricCenter(POINT tr[3]);

POINT GetRectangleGeometricCenter(POINT r[4]);

#endif