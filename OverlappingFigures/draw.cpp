#include "draw.h"
#include "stdafx.h"

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(b) << 8) | g) << 8) | r)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	assert(NULL != surface);
	assert(x <= SCREEN_WIDTH);
	assert(y <= SCREEN_HEIGHT);

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

void drawRectangle(SDL_Surface *s, POINT w[4], POINT r[4], Uint32 color) {
	line(s, w, r[0].x, r[0].y, r[1].x, r[1].y, 0x0000ff00, true);
	line(s, w, r[1].x, r[1].y, r[2].x, r[2].y, 0x0000ff00, true);
	line(s, w, r[2].x, r[2].y, r[3].x, r[3].y, 0x0000ff00, true);
	line(s, w, r[3].x, r[3].y, r[0].x, r[0].y, 0x0000ff00, true);
}

void drawTriangle(SDL_Surface *s, POINT w[4], POINT tr[3], Uint32 color) {
	line(s, w, tr[0].x, tr[0].y, tr[1].x, tr[1].y, 0x000000ff, true);
	line(s, w, tr[1].x, tr[1].y, tr[2].x, tr[2].y, 0x000000ff, true);
	line(s, w, tr[2].x, tr[2].y, tr[0].x, tr[0].y, 0x000000ff, true);
}

void cleanRectangle(SDL_Surface *s, POINT r[4], Uint32 color) {
	POINT w[4];
	w[0].x = 0; //[ left top, right top, right bottom, left bottom ]
	w[0].y = 0;
	w[1].x = 600;
	w[1].y = 0;
	w[2].x = 600;
	w[2].y = 600;
	w[3].x = 0;
	w[3].y = 600;
	line(s, w, r[0].x, r[0].y, r[1].x, r[1].y, 0x00000000, false);
	line(s, w, r[1].x, r[1].y, r[2].x, r[2].y, 0x00000000, false);
	line(s, w, r[2].x, r[2].y, r[3].x, r[3].y, 0x00000000, false);
	line(s, w, r[3].x, r[3].y, r[0].x, r[0].y, 0x00000000, false);
}

void cleanTriangle(SDL_Surface *s, POINT tr[3], Uint32 color) {
	POINT w[4];
	w[0].x = 0; //[ left top, right top, right bottom, left bottom ]
	w[0].y = 0;
	w[1].x = 600;
	w[1].y = 0;
	w[2].x = 600;
	w[2].y = 600;
	w[3].x = 0;
	w[3].y = 600;
	line(s, w, tr[0].x, tr[0].y, tr[1].x, tr[1].y, 0x00000000, false);
	line(s, w, tr[1].x, tr[1].y, tr[2].x, tr[2].y, 0x00000000, false);
	line(s, w, tr[2].x, tr[2].y, tr[0].x, tr[0].y, 0x00000000, false);
}

void line(SDL_Surface* surface, POINT w[4], int x1, int y1, int x2, int y2, Uint32 color, bool windowCheck)
{
	POINT temp;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x2 >= x1 ? 1 : -1;
	int sy = y2 >= y1 ? 1 : -1;
	if (dy <= dx) {
		int d = (dy << 1) - dx;
		int d1 = dy << 1;
		int d2 = (dy - dx) << 1;

		if (x1 > -1 && y1 > -1 && x1 <= SCREEN_WIDTH && y1 <= SCREEN_HEIGHT) {
			
			temp.x = x1;
			temp.y = y1;
			if (pointOutOfWindow(w, temp)) {
				put_pixel32(surface, x1, y1, 0x00ff00ff);
			}
			else {
				put_pixel32(surface, x1, y1, color);
			}
			
		}
		
		for (int x = x1 + sx, y = y1, i = 1; i <= dx; i++, x += sx) {
			if (d > 0) {
				d += d2; y += sy;
			}
			else
				d += d1;
			if (x > -1 && y > -1 && x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT) {
				temp.x = x;
				temp.y = y;
				if (pointOutOfWindow(w, temp)) {
					put_pixel32(surface, x, y, 0x00ff00ff);
				}
				else {
					put_pixel32(surface, x, y, color);
				}
			}
			
		}
	}
	else {
		int d = (dx << 1) - dy;
		int d1 = dx << 1;
		int d2 = (dx - dy) << 1;
		if (x1 > -1 && y1 > -1 && x1 <= SCREEN_WIDTH && y1 <= SCREEN_HEIGHT) {
			temp.x = x1;
			temp.y = y1;
			if (pointOutOfWindow(w, temp)) {
				put_pixel32(surface, x1, y1, 0x00ff00ff);
			}
			else {
				put_pixel32(surface, x1, y1, color);
			}
		}
		
		for (int x = x1, y = y1 + sy, i = 1; i <= dy; i++, y += sy) {
			if (d > 0) {
				d += d2; x += sx;
			}
			else
				d += d1;
			if (x > -1 && y > -1 && x <= SCREEN_WIDTH && y <= SCREEN_HEIGHT) {
				temp.x = x;
				temp.y = y;
				if (pointOutOfWindow(w, temp)) {
					put_pixel32(surface, x, y, 0x00ff00ff);
				}
				else {
					put_pixel32(surface, x, y, color);
				}
			}
		}
	}	
}

POINT getIntersection(POINT p1, POINT p2, POINT p3, POINT p4) {
	POINT result;
	result.x = -1;
	result.y = -1;

	float t_x = 0;
	float t_y = 0;		

	float dividor = ((p1.x - p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x - p4.x));
	if (dividor == 0) {
		return result;
	}
	t_x = ((p1.x*p2.y - p1.y*p2.x)*(p3.x - p4.x) - (p1.x - p2.x)*(p3.x*p4.y - p3.y*p4.x)) / dividor;
	t_y = ((p1.x*p2.y - p1.y*p2.x)*(p3.y - p4.y) - (p1.y - p2.y)*(p3.x*p4.y - p3.y*p4.x)) / dividor;
	if (t_x > fmax(p1.x, p2.x) || t_x < fmin(p1.x, p2.x) || t_y > fmax(p1.y, p2.y) || t_y < fmin(p1.y, p2.y) ||
		t_x > fmax(p3.x, p4.x) || t_x < fmin(p3.x, p4.x) || t_y > fmax(p3.y, p4.y) || t_y < fmin(p3.y, p4.y)) {
		return result;			
	}
	POINT temp;
	temp.x = t_x;
	temp.y = t_y;
	if (!checkPointOnLines(temp, p1, p2, p3, p4)) {
		return result;
	}
	//printf("\nintersection exists");
	result.x = t_x;
	result.y = t_y;
	//printf("\nx: %f, y: %f",t_x, t_y);
	return result;
}

POINT** getIntersectionPoints(POINT r[4], POINT tr[3]) {
	POINT** result = new POINT*[4];
	for (int i = 0; i < 3; ++i)
	{
		result[i] = new POINT[4]{ NULL, NULL, NULL, NULL};
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			int next_i = i + 1 == 3 ? 0 : i + 1;
			int next_j = j + 1 == 4 ? 0 : j + 1;
			POINT candidate = getIntersection(tr[i], tr[next_i], r[j], r[next_j]);
			if (!pointInMatrix(result, candidate)) {
				result[i][j] = candidate;
			}
		}
	}
	return result;
}

float distance(POINT p1, POINT p2) {
	return pow(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2), 0.5);
}

bool checkPointOnLines(POINT p, POINT p1, POINT p2, POINT p3, POINT p4) {
	float nu_1 = distance(p, p2) / distance(p1, p2);
	float nu_2 = distance(p, p4) / distance(p3, p4);
	if (nu_1 <= 1 && nu_1 >= 0 && nu_2 <= 1 && nu_2 >= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool pointInMatrix(POINT **m, POINT p) {
	bool result = false;
	if (p.x == -1 && p.y == -1) return false;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (m[i][j].x == p.x && m[i][j].y == p.y) {
				result = true;
			}
		}
	}
	return result;
}

POINT **getUnvisibleLines(POINT ** interseptionMatrix, POINT r[4], POINT tr[3], int r_index, int tr_index) {
	POINT** result = new POINT*[2];
	int temp_line_index = 0;
	if (r_index == 1) {
		for (int i = 0; i < 3; i++) {
			int point_number = 0;
			int indexes[2];
			for (int j = 0; j < 4; j++) {
				if (pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, interseptionMatrix[i][j])) {
					point_number++;
					indexes[point_number - 1] = j;
				}
			}
			if (point_number == 1) {
				result[temp_line_index] = new POINT[2]{ NULL, NULL };
				result[temp_line_index][0] = interseptionMatrix[i][indexes[0]];
				switch (i) {
				case 0:
					result[temp_line_index][1] = getPointInRectangle(r, tr[0], tr[1]);
					break;
				case 1:
					result[temp_line_index][1] = getPointInRectangle(r, tr[1], tr[2]);
					break;
				case 2:
					result[temp_line_index][1] = getPointInRectangle(r, tr[2], tr[0]);
					break;
				default:
					break;
				}
				temp_line_index++;
			}
			if (point_number == 2) {
				result[temp_line_index] = new POINT[2]{ NULL, NULL };
				result[temp_line_index][0] = interseptionMatrix[i][indexes[0]];
				result[temp_line_index][1] = interseptionMatrix[i][indexes[1]];
				temp_line_index++;
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			int point_number = 0;
			int indexes[2];
			for (int j = 0; j < 3; j++) {
				if (pointInWindow(SCREEN_WIDTH, SCREEN_HEIGHT, interseptionMatrix[j][i])) {
					point_number++;
					indexes[point_number - 1] = j;
				}
			}
			if (point_number == 1) {
				result[temp_line_index] = new POINT[2]{ NULL, NULL };
				result[temp_line_index][0] = interseptionMatrix[indexes[0]][i];
				switch (i) {
				case 0:
					result[temp_line_index][1] = getPointInTriangle(tr, r[0], r[1]);
					break;
				case 1:
					result[temp_line_index][1] = getPointInTriangle(tr, r[1], r[2]);
					break;
				case 2:
					result[temp_line_index][1] = getPointInTriangle(tr, r[2], r[3]);
					break;
				case 3:
					result[temp_line_index][1] = getPointInTriangle(tr, r[3], r[0]);
					break;
				default:
					break;
				}
				temp_line_index++;
			}
			if (point_number == 2) {
				result[temp_line_index] = new POINT[2]{ NULL, NULL };
				result[temp_line_index][0] = interseptionMatrix[indexes[0]][i];
				result[temp_line_index][1] = interseptionMatrix[indexes[1]][i];
				temp_line_index++;
			}
		}
	}
	result[temp_line_index] = NULL;
	return result;
}

bool pointInWindow(int width, int height, POINT p) {
	if (p.x > width || p.x < 0 || p.y < 0 || p.y > height) return false;
	else return true;
}

POINT getPointInRectangle(POINT r[4], POINT p1, POINT p2) {
	if(p1.x <= fmax(r[1].x, r[2].x) && p1.x >= fmin(r[0].x, r[3].x) &&
		p1.y <= fmax(r[2].y, r[3].y) && p1.y >= fmin(r[0].y, r[1].y)) {
		return p1;
	}
	if (p2.x <= fmax(r[1].x, r[2].x) && p2.x >= fmin(r[0].x, r[3].x) &&
		p2.y <= fmax(r[2].y, r[3].y) && p2.y >= fmin(r[0].y, r[1].y)) {
		return p2;
	}
	return p1;
}

bool pointOutOfWindow(POINT r[4], POINT p){
	if (p.x > max(r[1].x, r[2].x) || p.x < min(r[0].x, r[3].x) ||
		p.y > max(r[2].y, r[3].y) || p.y < min(r[0].y, r[1].y)) {
		return true;
	}
	return false;
}

POINT getPointInTriangle(POINT tr[3], POINT p1, POINT p2) {
	if (p1.x <= fmax(tr[1].x, tr[2].x) && p1.x >= fmin(tr[0].x, tr[1].x) &&
		p1.y <= fmax(tr[0].y, tr[2].y) && p1.y >= fmin(tr[1].y, tr[2].y)) {
		return p1;
	}
	if (p2.x <= fmax(tr[1].x, tr[2].x) && p2.x >= fmin(tr[0].x, tr[1].x) &&
		p2.y <= fmax(tr[0].y, tr[2].y) && p2.y >= fmin(tr[1].y, tr[2].y)) {
		return p2;
	}
	return p1;
}

POINT GetPoint(POINT p, double angle, POINT center)
{
	POINT newPoint;
	int dx = center.x;
	int dy = center.y;
	newPoint.x = rint((p.x - dx)  * cos(angle) - (p.y - dy) * sin(angle)) + dx;
	newPoint.y = rint((p.x - dx) * sin(angle) + (p.y - dy) * cos(angle)) + dy;
	return newPoint;
}

POINT GetTriangleGeometricCenter(POINT tr[3]) {
	/*POINT result;
	POINT mPoint1;
	POINT mPoint2;
	mPoint1.x = rint((max(tr[0].x, tr[1].x) - min(tr[0].x, tr[1].x)) / 2);
	mPoint1.y = rint((max(tr[0].y, tr[1].y) - min(tr[0].y, tr[1].y)) / 2);
	mPoint2.x = rint((max(tr[2].x, tr[1].x) - min(tr[2].x, tr[1].x)) / 2);
	mPoint2.y = rint((max(tr[2].y, tr[1].y) - min(tr[2].y, tr[1].y)) / 2);

	result = getIntersection(tr[0], mPoint2, tr[2], mPoint1);
	return result;*/

	POINT result;

	int minY = min(tr[0].y, tr[1].y);
	int maxY = max(tr[0].y, tr[1].y);
	int minX = min(tr[0].x, tr[1].x);
	int maxX = max(tr[0].x, tr[1].x);


	minY = tr[2].y < minY ? tr[2].y : minY;
	maxY = tr[2].y > maxY ? tr[2].y : maxY;
	minX = tr[2].x < minX ? tr[2].x : minX;
	maxX = tr[2].x > maxX ? tr[2].x : maxX;

	POINT p1;
	POINT p2;
	POINT p3;
	POINT p4;

	p1.x = minX;
	p1.y = minY;
	p2.x = maxX;
	p2.y = maxY;
	p3.x = maxX;
	p3.y = minY;
	p4.x = minX;
	p4.y = maxY;

	result = getIntersection(p1, p2, p3, p4);
	return result;
}

POINT GetRectangleGeometricCenter(POINT r[4]) {
	POINT result;

	int minY = min(r[0].y, r[1].y);
	int maxY = max(r[0].y, r[1].y);	
	int minX = min(r[0].x, r[1].x);	
	int maxX = max(r[0].x, r[1].x);
	int minYx;
	int maxYx;
	int minXy;
	int maxXy;
	
	minY = min(r[2].y, r[3].y) < minY ? min(r[2].y, r[3].y) : minY;
	maxY = max(r[2].y, r[3].y) > maxY ? max(r[2].y, r[3].y) : maxY;
	minX = min(r[2].x, r[3].x) < minX ? min(r[2].x, r[3].x) : minX;
	maxX = max(r[2].x, r[3].x) > maxX ? max(r[2].x, r[3].x) : maxX;
	for (int i = 0; i < 4; i++) {
		if (r[i].y == minY) {
			minYx = r[i].x;
		}
		if (r[i].y == maxY) {
			maxYx = r[i].x;
		}
		if (r[i].x == minX) {
			minXy = r[i].y;
		}
		if (r[i].x == maxX) {
			maxXy = r[i].y;
		}
	}

	POINT p1;
	POINT p2;
	POINT p3;
	POINT p4;

	p1.x = minYx;
	p1.y = minY;
	p2.x = maxYx;
	p2.y = maxY;
	p3.x = minX;
	p3.y = minXy;
	p4.x = maxX;
	p4.y = maxXy;

	result = getIntersection(p1, p2, p3, p4);
	return result;
}

