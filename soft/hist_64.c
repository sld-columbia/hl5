/*
  serial program for histogram equalisation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define N_IMAGES 1

// declare image dimensions and size as constant global variables
#define IMAGE_EDGE 64
#define image_width IMAGE_EDGE
#define image_height IMAGE_EDGE
#define image_size (IMAGE_EDGE * IMAGE_EDGE)
#define color_depth 255

unsigned char image[image_size];
unsigned char output_image[image_size];
int histogram[color_depth];
int transfer_function[color_depth];

int main(){

	int i, j, k;
	unsigned char value = 0;
	/* Initialization */
	
	for (i = 0; i < image_size; i++) {
		image[i] = value++;
		if (value > color_depth)
			value = 0;
	}

	/* Create histogram */

	for (k = 0; k < N_IMAGES; k++){
		for (i = 0; i < color_depth; i++) {
			histogram[i] = 0; 
			transfer_function[i] = 0;
		}

		for(i = 0; i < image_size; i++){
			histogram[image[i]]++;
		}

		/* Finding the normalised values using cumulative mass function */
		for(i = 0; i < color_depth; i++){
			for(j = 0; j < i+1; j++){
				transfer_function[i] += color_depth * histogram[j] / image_size;
			}
		}

		/* Evaluate output image */
		for(i = 0; i < image_size; i++){
			output_image[i] = transfer_function[image[i]];
		}
	}

	return 0;
}
