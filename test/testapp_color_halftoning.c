/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "interface.h"

int main(int argc, char *argv[])
{
	tagImageProp  *input_img    = NULL;
	tagImageProp  *output_img   = NULL;

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

	/* Allocate memory for the output image object */
	output_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	memcpy(output_img, input_img, sizeof(tagImageProp));

	/* Allocate memory for the output image */
	output_img->buffer = (uint8_t *) malloc(output_img->size);
	if(output_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Separable color halftoning */
	color_halftone(input_img, output_img, COLOR_HALFTONE_SEPARABLE);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_color_halftone_separable.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_color_halftone_separable.raw (for Separable color halftoning)\n");

	/* MBVQ color halftoning */
	color_halftone(input_img, output_img, COLOR_HALFTONE_MBVQ);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_color_halftone_mbvq.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_color_halftone_mbvq.raw (for MBVQ color halftoning)\n");


FREE_MEM:

if(output_img != NULL) {
	if(output_img->buffer != NULL) {
		free(output_img->buffer);
		output_img->buffer = NULL;
	}
	free(output_img);
	output_img = NULL;
}

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
