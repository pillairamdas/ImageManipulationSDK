/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include "interface.h"

int main(int argc, char *argv[])
{
	tagImageProp  *input_img      = NULL;
	tagImageProp  *warped_img     = NULL;
	tagImageProp  *dewarped_img   = NULL;

	/* Check for application validity */
	if(argc < 5) {
		LOG(ERROR, "Usage: %s <input image> <input_width> <input_height> <input bits per pixel> \n\n", argv[0]);
		goto EXIT;
	}

	/* Allocate memory for the input image object */
	input_img = (tagImageProp *) malloc (sizeof(tagImageProp));
	if(input_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Populate the input image properties */
	input_img->width  = atoi(argv[2]);
	input_img->height = atoi(argv[3]);
	input_img->bpp    = atoi(argv[4]);
	input_img->pixel_count = input_img->width * input_img->height;
	input_img->size        = input_img->pixel_count * input_img->bpp;

	/* Allocate memory for the input image */
	input_img->buffer = (uint8_t *) malloc(input_img->size);
	if(input_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Read the input image */
	if(read_from_file(input_img->buffer, input_img->size, argv[1]) != 0) {
		goto FREE_MEM;
	}

	/* Allocate memory for the warped image object */
	warped_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(warped_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	memcpy(warped_img, input_img, sizeof(tagImageProp));

	/* Allocate memory for the warped image */
	warped_img->buffer = (uint8_t *) malloc(warped_img->size);
	if(warped_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Allocate memory for the dewarped image object */
	dewarped_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(dewarped_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	memcpy(dewarped_img, input_img, sizeof(tagImageProp));

	/* Allocate memory for the dewarped image */
	dewarped_img->buffer = (uint8_t *) malloc(dewarped_img->size);
	if(dewarped_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Perform warping on the input image */
	warping(input_img, warped_img);
	if((dump_to_file(warped_img->buffer, warped_img->size, "OUT_warped.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_warped.raw (for warped output image)\n");

	/* Perform dewarping on the input image */
	dewarping(warped_img, dewarped_img);
	if((dump_to_file(dewarped_img->buffer, dewarped_img->size, "OUT_dewarped.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dewarped.raw (for dewarped output image)\n");

FREE_MEM:

	/* Free the warped image object */
	if(warped_img != NULL) {
		if(warped_img->buffer != NULL) {
			free(warped_img->buffer);
			warped_img->buffer = NULL;
		}
		free(warped_img);
		warped_img = NULL;
	}

	/* Free the dewarped image object */
	if(dewarped_img != NULL) {
		if(dewarped_img->buffer != NULL) {
			free(dewarped_img->buffer);
			dewarped_img->buffer = NULL;
		}
		free(dewarped_img);
		dewarped_img = NULL;
	}

	/* Free the input image object */
	if(input_img != NULL) {
		if(input_img->buffer != NULL) {
			free(input_img->buffer);
			input_img->buffer = NULL;
		}
		free(input_img);
		input_img = NULL;
	}

EXIT:
	return 0;

}
