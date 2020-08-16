#include "drawing.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void point_i(Image *image, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b) {
	if (x >= image->width || y >= image->height) {
		#ifdef DEBUG 
		fprintf(stderr, "Attempting to plot integer point out of bounds\n");
		#endif
		return;
	}

	Pixel p = {b, g, r};

	image->data[x + image->width * y] = p;
}

void point_f(Image *image, float x, float y, uint8_t r, uint8_t g, uint8_t b) {
	if (x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0) {
		#ifdef DEBUG 
		fprintf(stderr, "Attempting to plot point out of bounds\n");
		#endif
		return;
	}

	int x_int = (int)(image->width * x);
	int y_int = (int)(image->height * y);

	Pixel p = {b, g, r};

	image->data[x_int + image->width * y_int] = p;
}


float fpart(float x) {
	return x - floorf(x);
}

float rfpart(float x) {
	return 1 - fpart(x);
}

void line_f(Image *image, float x0, float y0, float x1, float y1, uint8_t r, uint8_t g, uint8_t b) {
	
	// conditioning our data
	bool steep = fabsf(y1 - y0) > fabsf(x1 - x0);

	float intermediary;

	if (steep) {
		intermediary = x0;
		x0 = y0;
		y0 = intermediary;

		intermediary = x1;
		x1 = y1;
		y1 = intermediary; 
	}

	if (x0 > x1) {
		intermediary = x0;
		x0 = x1;
		x1 = intermediary;

		intermediary = y0;
		y0 = y1;
		y1 = intermediary;
	}

	float dx = x1 - x0;
	float dy = y1 - y0;
	float gradient;
	if (dx == 0.0) {
		gradient = 1.0;
	} else {
		gradient = dy/dx;
	}

	// starting to draw the first endpoint

	float x_end = roundf(x0);
	float y_end = y0 + gradient * (x_end - x0);

	float x_gap = rfpart(x0 + 0.5);
	float x_pixel1 = x_end;
	float y_pixel1 = floorf(y_end);

	uint8_t r_intensity_r = (uint8_t)((float)r * rfpart(y_end) * x_gap);
	uint8_t r_intensity_g = (uint8_t)((float)g * rfpart(y_end) * x_gap);
	uint8_t r_intensity_b = (uint8_t)((float)b * rfpart(y_end) * x_gap);
	uint8_t intensity_r = (uint8_t)((float)r * fpart(y_end) * x_gap);
	uint8_t intensity_g = (uint8_t)((float)g * fpart(y_end) * x_gap);
	uint8_t intensity_b = (uint8_t)((float)b * fpart(y_end) * x_gap);

	if (steep) {
		point_i(image, y_pixel1, x_pixel1, r_intensity_r, r_intensity_g, r_intensity_b);
		point_i(image, y_pixel1+1, x_pixel1, intensity_r,   intensity_g,   intensity_b);
	} else {
		point_i(image, x_pixel1, y_pixel1, r_intensity_r, r_intensity_g, r_intensity_b);
		point_i(image, x_pixel1, y_pixel1+1, intensity_r,   intensity_g,   intensity_b);
	}

	float y_intersect = y_end + gradient; // first y-intersection with the pixel grid

	// drawing the second endpoint

	x_end = roundf(x1);
	y_end = y1 + gradient * (x_end - x1);

	x_gap = fpart(x1 + 0.5);
	float x_pixel2 = x_end;
	float y_pixel2 = floorf(y_end);

	r_intensity_r = (uint8_t)((float)r * rfpart(y_end) * x_gap);
	r_intensity_g = (uint8_t)((float)g * rfpart(y_end) * x_gap);
	r_intensity_b = (uint8_t)((float)b * rfpart(y_end) * x_gap);
	intensity_r = (uint8_t)((float)r * fpart(y_end) * x_gap);
	intensity_g = (uint8_t)((float)g * fpart(y_end) * x_gap);
	intensity_b = (uint8_t)((float)b * fpart(y_end) * x_gap);

	if (steep) {
		point_i(image, y_pixel2, x_pixel2, r_intensity_r, r_intensity_g, r_intensity_b);
		point_i(image, y_pixel2+1, x_pixel2, intensity_r,   intensity_g,   intensity_b);
	} else {
		point_i(image, x_pixel2, y_pixel2, r_intensity_r, r_intensity_g, r_intensity_b);
		point_i(image, x_pixel2, y_pixel2+1, intensity_r,   intensity_g,   intensity_b);
	}

	// main loop

	if (steep) {
		for (float x = x_pixel1 + 1.0; x < x_pixel2; x += 1.0) {
				r_intensity_r = (uint8_t)((float)r * rfpart(y_intersect));
				r_intensity_g = (uint8_t)((float)g * rfpart(y_intersect));
				r_intensity_b = (uint8_t)((float)b * rfpart(y_intersect));
				intensity_r = (uint8_t)((float)r * fpart(y_intersect));
				intensity_g = (uint8_t)((float)g * fpart(y_intersect));
				intensity_b = (uint8_t)((float)b * fpart(y_intersect));

				point_i(image, y_intersect,     x, r_intensity_r, r_intensity_g, r_intensity_b);
				point_i(image, y_intersect + 1, x,   intensity_r,   intensity_g,   intensity_b);

				y_intersect += gradient;
		}
	} else {
		for (float x = x_pixel1 + 1.0; x < x_pixel2; x += 1.0) {
				r_intensity_r = (uint8_t)((float)r * rfpart(y_intersect));
				r_intensity_g = (uint8_t)((float)g * rfpart(y_intersect));
				r_intensity_b = (uint8_t)((float)b * rfpart(y_intersect));
				intensity_r = (uint8_t)((float)r * fpart(y_intersect));
				intensity_g = (uint8_t)((float)g * fpart(y_intersect));
				intensity_b = (uint8_t)((float)b * fpart(y_intersect));

				point_i(image, x, y_intersect,   r_intensity_r, r_intensity_g, r_intensity_b);
				point_i(image, x, y_intersect + 1, intensity_r,   intensity_g,   intensity_b);

				y_intersect += gradient;
		}
	}
}


void vline(Image *image, int x, float y0, float y1, uint8_t r, uint8_t g, uint8_t b) {
	// conditioning our data
	if (y0 > y1) {
		float intermediary = y1;
		y1 = y0;
		y0 = intermediary;
	}

	// starting to draw the first endpoint
	float y_gap = rfpart(y0 + 0.5);
	float x_pixel1 = (float)x;
	float y_pixel1 = roundf(y0);

	uint8_t r_intensity_r = (uint8_t)((float)r * (1.0 - y_gap));
	uint8_t r_intensity_g = (uint8_t)((float)g * (1.0 - y_gap));
	uint8_t r_intensity_b = (uint8_t)((float)b * (1.0 - y_gap));
	uint8_t intensity_r = (uint8_t)((float)r * y_gap);
	uint8_t intensity_g = (uint8_t)((float)g * y_gap);
	uint8_t intensity_b = (uint8_t)((float)b * y_gap);

	point_i(image, x_pixel1, y_pixel1+1, r_intensity_r, r_intensity_g, r_intensity_b);
	point_i(image, x_pixel1, y_pixel1, intensity_r,   intensity_g,   intensity_b);

	// drawing the second endpoint
	y_gap = fpart(y1 + 0.5);
	float x_pixel2 = (float)x;
	float y_pixel2 = roundf(y1);

	r_intensity_r = (uint8_t)((float)r *  (1.0 - y_gap));
	r_intensity_g = (uint8_t)((float)g *  (1.0 - y_gap));
	r_intensity_b = (uint8_t)((float)b *  (1.0 - y_gap));
	intensity_r = (uint8_t)((float)r * y_gap);
	intensity_g = (uint8_t)((float)g * y_gap);
	intensity_b = (uint8_t)((float)b * y_gap);

	point_i(image, x_pixel2, y_pixel2-1, r_intensity_r, r_intensity_g, r_intensity_b);
	point_i(image, x_pixel2, y_pixel2, intensity_r,   intensity_g,   intensity_b);

	// main loop
	for (float y = y_pixel1 + 1.0; y < y_pixel2 ; y += 1.0) {
		point_i(image, x, y, r, g, b);
	}

}

void left_triangle_f(Image *image, Point p0, Point p1, Point p2, uint8_t r, uint8_t g, uint8_t b) {
	float slope0 = (p1.y - p0.y) / (p1.x - p0.x);
	float slope1 = (p2.y - p0.y) / (p2.x - p0.x);

	float current_y0 = p0.y;
	float current_y1 = p0.y;

	#ifdef DEBUG
	printf("p0: %f, %f \n", p0.x, p0.y);
	printf("p1: %f, %f \n", p1.x, p1.y);
	printf("p2: %f, %f \n", p2.x, p2.y);
	#endif
	for (int x = p0.x; x <= p1.x; x++)
	{
		// drawLine((int)curx1, scanlineY, (int)curx2, scanlineY);
		vline(image, x, current_y0, current_y1, r, g ,b);
		current_y0 += slope0;
		current_y1 += slope1;
	}
}

void right_triangle_f(Image *image, Point p0, Point p1, Point p2, uint8_t r, uint8_t g, uint8_t b) {
	float slope0 = (p2.y - p0.y) / (p2.x - p0.x);
	float slope1 = (p2.y - p1.y) / (p2.x - p1.x);

	float current_y0 = p0.y;
	float current_y1 = p1.y;

	for (int x = p0.x; x <= p2.x; x++)
	{
		// drawLine((int)curx1, scanlineY, (int)curx2, scanlineY);
		vline(image, x, current_y0, current_y1, r, g ,b);
		current_y0 += slope0;
		current_y1 += slope1;
	}
}


void triangle_f(Image *image, Point p0, Point p1, Point p2, uint8_t r, uint8_t g, uint8_t b) {
	/* at first sort the three vertices by x-coordinate ascending so v1 is the topmost vertice */
	
	Point intermediary;

	if (p0.x > p1.x) {
		intermediary = p0;
		p0 = p1;
		p1 = intermediary;
	} 

	if (p0.x > p2.x) {
		intermediary = p0;
		p0 = p2;
		p2 = intermediary;
	} 
	
	if (p1.x > p2.x) {
		intermediary = p1;
		p1 = p2;
		p2 = intermediary;
	} 
	/* here we know that p0.x <= p1.x <= p2.x */

	/* general case - split the triangle in a topflat and bottom-flat one */
	Point p3;
	p3.y = p0.y + ((p1.x - p0.x) / (p2.x - p0.x)) * (p2.y - p0.y);
	p3.x = p1.x;


	left_triangle_f(image, p0, p1, p3, r, g, b);
	right_triangle_f(image, p1, p3, p2, r, g, b);

	// Debug Points
	// point_i(image, p0.x, p0.y, 255, 0, 0);
	// point_i(image, p1.x, p1.y, 255, 0, 0);
	// point_i(image, p2.x, p2.y, 255, 0, 0);
	// point_i(image, p3.x, p3.y, 255, 0, 255);
}


Image *create_image(unsigned int width, unsigned int height) {
	size_t size = sizeof(unsigned int) + sizeof(unsigned int) + (width * height * sizeof(Pixel));


	Image *image = (Image*)malloc(size);
	image->width = width;
	image->height = height;
	//memset(image->data, 0, (width * height * sizeof(Pixel)));
	return image;
}