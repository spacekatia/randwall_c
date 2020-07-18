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

    int x_points = 2;
    int y_points = 2;
    int num_points = x_points * y_points;
    Point* point_cloud = malloc(num_points * sizeof(Point));

    for (int y_cell = 0; y_cell < y_points; y_cell++){
        for (int x_cell = 0; x_cell < x_points; x_cell++){
            double x = (double)rand()/(double)RAND_MAX;
            double y = (double)rand()/(double)RAND_MAX;
            // double x = (double)0.5;
            // double y = (double)0.5;

            int index = y_cell * (x_points) + x_cell;
            point_cloud[index].x = (x_cell + x)/(double)x_points;
            point_cloud[index].y = (y_cell + y)/(double)y_points;
        }
    }

    line_f(image, point_cloud[0].x * RESOLUTION_X, point_cloud[0].y * RESOLUTION_Y, point_cloud[1].x * RESOLUTION_X, point_cloud[1].y * RESOLUTION_Y, 255,255,255);
    line_f(image, point_cloud[1].x * RESOLUTION_X, point_cloud[1].y * RESOLUTION_Y, point_cloud[2].x * RESOLUTION_X, point_cloud[2].y * RESOLUTION_Y, 255,255,255);
    line_f(image, point_cloud[2].x * RESOLUTION_X, point_cloud[2].y * RESOLUTION_Y, point_cloud[0].x * RESOLUTION_X, point_cloud[0].y * RESOLUTION_Y, 255,255,255);
    line_f(image, point_cloud[0].x * RESOLUTION_X, point_cloud[0].y * RESOLUTION_Y, point_cloud[3].x * RESOLUTION_X, point_cloud[3].y * RESOLUTION_Y, 255,255,255);
    line_f(image, point_cloud[1].x * RESOLUTION_X, point_cloud[1].y * RESOLUTION_Y, point_cloud[3].x * RESOLUTION_X, point_cloud[3].y * RESOLUTION_Y, 255,255,255);
    line_f(image, point_cloud[2].x * RESOLUTION_X, point_cloud[2].y * RESOLUTION_Y, point_cloud[3].x * RESOLUTION_X, point_cloud[3].y * RESOLUTION_Y, 255,255,255);

    unsigned char* bmp;
    int size = create_bitmap(image->data, RESOLUTION_X, RESOLUTION_Y, &bmp);
    
    FILE* f = fopen("test.bmp", "w");
    fwrite(bmp, 1, size, f);
    fclose(f);

    return 0;
}
