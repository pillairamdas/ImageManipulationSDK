/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "interface.h"

int main(int argc, char *argv[])
{
	tagImageProp  *left_img     = NULL;
	tagImageProp  *middle_img   = NULL;
	tagImageProp  *right_img    = NULL;
	tagImageProp  *output_img   = NULL;

	/* Check for application validity */
	if(argc < 7) {
		LOG(ERROR, "Usage: %s <left image> <middle image> <right image> <input_width> <input_height> <input bits per pixel> \n\n", argv[0]);
		goto EXIT;
	}


	/* Allocate memory for the left image object */
	left_img = (tagImageProp *) malloc (sizeof(tagImageProp));
	if(left_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Populate the image properties */
	left_img->width  = atoi(argv[4]);
	left_img->height = atoi(argv[5]);
	left_img->bpp    = atoi(argv[6]);
	left_img->pixel_count = left_img->width * left_img->height;
	left_img->size = left_img->pixel_count * left_img->bpp;

	/* Allocate memory for the left image */
	left_img->buffer = (uint8_t *) malloc(left_img->size);
	if(left_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Read the left image */
	if(read_from_file(left_img->buffer, left_img->size, argv[1]) != 0) {
		goto FREE_MEM;
	}

	/* Allocate memory for the middle image object */
	middle_img = (tagImageProp *) malloc (sizeof(tagImageProp));
	if(middle_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	memcpy(middle_img, left_img, sizeof(tagImageProp));

	/* Allocate memory for the middle image */
	middle_img->buffer = (uint8_t *) malloc(middle_img->size);
	if(middle_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Read the middle image */
	if(read_from_file(middle_img->buffer, middle_img->size, argv[2]) != 0) {
		goto FREE_MEM;
	}

	/* Allocate memory for the right image object */
	right_img = (tagImageProp *) malloc (sizeof(tagImageProp));
	if(right_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	memcpy(right_img, left_img, sizeof(tagImageProp));

	/* Allocate memory for the right image */
	right_img->buffer = (uint8_t *) malloc(right_img->size);
	if(right_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Read the right image */
	if(read_from_file(right_img->buffer, right_img->size, argv[3]) != 0) {
		goto FREE_MEM;
	}

	/* Get the output stiched image */
	output_img = stitch(left_img, middle_img, right_img);
	if(output_img != NULL && output_img->buffer != NULL) {
		if((dump_to_file(output_img->buffer, output_img->size, "OUT_stitched.raw")) != 0) {
			goto FREE_MEM;
		}
		LOG(OUTPUT, "Output Filename: OUT_stitched.raw\n");
	} else {
		LOG(ERROR, "Encountered error\n");
	}


FREE_MEM:

	/* Free the output image object */
	if(output_img != NULL) {
		if(output_img->buffer != NULL) {
			free(output_img->buffer);
			output_img->buffer = NULL;
		}
		free(output_img);
		output_img = NULL;
	}

	/* Free the left image object */
	if(left_img != NULL) {
		if(left_img->buffer != NULL) {
			free(left_img->buffer);
			left_img->buffer = NULL;
		}
		free(left_img);
		left_img = NULL;
	}

	/* Free the middle image object */
	if(middle_img != NULL) {
		if(middle_img->buffer != NULL) {
			free(middle_img->buffer);
			middle_img->buffer = NULL;
		}
		free(middle_img);
		middle_img = NULL;
	}

	/* Free the right image object */
	if(right_img != NULL) {
		if(right_img->buffer != NULL) {
			free(right_img->buffer);
			right_img->buffer = NULL;
		}
		free(right_img);
		right_img = NULL;
	}

EXIT:
	return 0;

}
