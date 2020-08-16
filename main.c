#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <sys/random.h>

#include "bmp.h"
#include "drawing.h"

#define RESOLUTION_X 1920
#define RESOLUTION_Y 1080



int main(int argc, char** argv) {

	unsigned int seed;
	Image *image = create_image(RESOLUTION_X, RESOLUTION_Y);

	getrandom(&seed, sizeof(seed), 0);
	srand(seed);

	int x_points = 16;
	int y_points = 9;
	int num_points = x_points * y_points;
	Point* point_cloud = malloc(num_points * sizeof(Point));

	for (int y_cell = 0; y_cell < y_points; y_cell++){
		for (int x_cell = 0; x_cell < x_points; x_cell++){
			double x = (double)rand()/(double)RAND_MAX;
			double y = (double)rand()/(double)RAND_MAX;

			if (x_cell == 0){
				x = 0;
			} 
			if (x_cell == x_points -1){
				x=1;
			}
			if (y_cell == 0){
				y = 0;
			} 
			if (y_cell == y_points -1){
				y=1;
			}
			// double x = (double)0.5;
			// double y = (double)0.5;

			int index = y_cell * (x_points) + x_cell;
			point_cloud[index].x = (x_cell + x)/(double)x_points * RESOLUTION_X;
			point_cloud[index].y = (y_cell + y)/(double)y_points * RESOLUTION_Y;
		}
	}	


	int r1 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
	int g1 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
	int b1 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
	
	int r2 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
	int g2 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
	int b2 = (int)(255.0 * (double)rand()/(double)RAND_MAX);

	for (int y_cell = 1; y_cell < y_points; y_cell++){
		for (int x_cell = 1; x_cell < x_points; x_cell++){
			switch (rand() % 6){
			case 0 : 
				r1 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
				break;
			case 1 :
				g1 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
				break;
			case 2 :
				b1 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
				break;
			case  3 :
				r2 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
				break;
			case 4 :
				g2 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
				break;
			case 5 :
				b2 = (int)(255.0 * (double)rand()/(double)RAND_MAX);
				break;
			}


			int index = y_cell * (x_points) + x_cell;
			triangle_f(image, point_cloud[index], point_cloud[index - 1], point_cloud[index - x_points], r1, g1, b1);

			triangle_f(image, point_cloud[index - x_points -1], point_cloud[index - 1], point_cloud[index - x_points], r2, g2, b2);
		}
	}

	unsigned char* bmp;
	int size = create_bitmap(image->data, RESOLUTION_X, RESOLUTION_Y, &bmp);
	
	FILE* f = fopen("test.bmp", "w");
	fwrite(bmp, 1, size, f);
	fclose(f);

	return 0;
}
