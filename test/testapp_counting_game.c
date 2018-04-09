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
	tagForegroundColor fg       = FG_WHITE;

	/* Check for application validity */
	if(argc < 6) {
		LOG(ERROR, "Usage: %s <input image> <input_width> <input_height> <input bits per pixel> <fg_color> \n\n", argv[0]);
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
	fg                = (tagForegroundColor) atoi(argv[5]);
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

	counting_game(input_img, fg);

FREE_MEM:

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
