#ifndef BMP_H
#define BMP_H

#include <stdint.h>
typedef struct Pixel{
	uint8_t b;
	uint8_t g;
	uint8_t r;
} Pixel;

int create_bitmap(Pixel data[], int width, int height, unsigned char** reference);

#endif
