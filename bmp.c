#include "bmp.h"

#include <string.h>
#include <stdlib.h>


#define COLOR_DEPTH 24
#define BMP_HEADER_SIZE 14

struct BmpHeader{
	uint16_t id;
	uint32_t size;
	uint16_t as1;               //application specific
	uint16_t as2;               //same
	uint32_t offset;
}__attribute__((__packed__));

struct DibHeader{
	uint32_t dib_size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t color_depth;
	uint32_t compression_mode;
	uint32_t image_size;
	uint32_t density_width;
	uint32_t density_height;
	uint32_t color1;            //number of colours in the palette (unused for now)
	uint32_t color2;            //number of important colours (also unused)
};


int create_bitmap(Pixel data[], int width, int height, unsigned char** reference){
	int line_length = ((COLOR_DEPTH * width + 31) / 32) * 4;
	int image_size = line_length * height;

	struct DibHeader dib = {
		40, width, height, 1, COLOR_DEPTH, 0, image_size, 2048, 2048, 0, 0  
	};

	struct BmpHeader header;
	header.id = 0x4D42; //"BM" 
	header.size = BMP_HEADER_SIZE + sizeof(dib) + image_size;
	header.offset = BMP_HEADER_SIZE + sizeof(dib);
	
	int offset = BMP_HEADER_SIZE + sizeof(dib);
	
	char* pixel_data = malloc(offset + image_size);
	
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			int i = y * width + x;
			int j = (y * line_length) + (x * 3);

			memcpy(&pixel_data[offset + j], &data[i], sizeof(Pixel));
		}
	}
	memcpy(&pixel_data[0], &header, BMP_HEADER_SIZE);
	memcpy(&pixel_data[BMP_HEADER_SIZE], &dib, sizeof(dib));
	
	*reference = pixel_data;
	return header.size;
}

