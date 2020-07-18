#ifndef DRAWING_H
#define DRAWING_H

#include "bmp.h"

typedef struct {
    unsigned int width;
    unsigned int height;
    // unsigned int depth;
    Pixel data[];
} Image;

typedef struct Point{
    float x;
    float y;
} Point;


void line_f(Image *image, float x0, float y0, float x1, float y1, uint8_t r, uint8_t g, uint8_t b);
Image *create_image(unsigned int width, unsigned int height);

#endif
