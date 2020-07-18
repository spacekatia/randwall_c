#include "drawing.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void point_i(Image *image, unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x >= image->width || y >= image->height) {
        fprintf(stderr, "Attempting to plot integer point out of bounds\n");
        return;
    }

    Pixel p = {b, g, r};

    image->data[x + image->width * y] = p;
}

void point_f(Image *image, float x, float y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0) {
        fprintf(stderr, "Attempting to plot point out of bounds\n");
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

    uint8_t r_intensity = (uint8_t)(255.0 * rfpart(y_end) * x_gap);
    uint8_t intensity = (uint8_t)(255.0 * fpart(y_end) * x_gap);

    if (steep) {
        point_i(image, y_pixel1, x_pixel1, r_intensity, r_intensity, r_intensity);
        point_i(image, y_pixel1+1, x_pixel1, intensity,   intensity,   intensity);
    } else {
        point_i(image, x_pixel1, y_pixel1, r_intensity, r_intensity, r_intensity);
        point_i(image, x_pixel1, y_pixel1+1, intensity,   intensity,   intensity);
    }

    float y_intersect = y_end + gradient; // first y-intersection with the pixel grid

    // drawing the second endpoint

    x_end = roundf(x1);
    y_end = y1 + gradient * (x_end - x1);

    x_gap = fpart(x1 + 0.5);
    float x_pixel2 = x_end;
    float y_pixel2 = floorf(y_end);

    r_intensity = (uint8_t)(255.0 * rfpart(y_end) * x_gap);
    intensity = (uint8_t)(255.0 * fpart(y_end) * x_gap);

    if (steep) {
        point_i(image, y_pixel2, x_pixel2, r_intensity, r_intensity, r_intensity);
        point_i(image, y_pixel2+1, x_pixel2, intensity,   intensity,   intensity);
    } else {
        point_i(image, x_pixel2, y_pixel2, r_intensity, r_intensity, r_intensity);
        point_i(image, x_pixel2, y_pixel2+1, intensity,   intensity,   intensity);
    }

    // main loop

    if (steep) {
        for (float x = x_pixel1 + 1.0; x < x_pixel2 - 1; x += 1.0) {
                uint8_t r_intensity = (uint8_t)(255.0 * rfpart(y_intersect));
                uint8_t intensity = (uint8_t)(255.0 * fpart(y_intersect));

                point_i(image, y_intersect, x, r_intensity, r_intensity, r_intensity);
                point_i(image, y_intersect + 1, x, intensity, intensity, intensity);

                y_intersect += gradient;
        }
    } else {
        for (float x = x_pixel1 + 1.0; x < x_pixel2 - 1; x += 1.0) {
                uint8_t r_intensity = (uint8_t)(255.0 * rfpart(y_intersect));
                uint8_t intensity = (uint8_t)(255.0 * fpart(y_intersect));

                point_i(image, x, y_intersect, r_intensity, r_intensity, r_intensity);
                point_i(image, x, y_intersect + 1, intensity, intensity, intensity);

                y_intersect += gradient;
        }
    }
}

void triangle_f(Image *image, float x0, float y0, float x1, float y1, float x2, float y2, uint8_t r, uint8_t g, uint8_t b) {
    
}

Image *create_image(unsigned int width, unsigned int height) {
    size_t size = sizeof(unsigned int) + sizeof(unsigned int) + (width * height * sizeof(Pixel));


    Image *image = (Image*)malloc(size);
    image->width = width;
    image->height = height;
    //memset(image->data, 0, (width * height * sizeof(Pixel)));
    return image;
}