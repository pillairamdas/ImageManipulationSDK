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

	/* Perform fixed binary dithering */
	dithering(input_img, output_img, DITHERING_FIXED_BINARY);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_fixed_binary.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_fixed_binary.raw (for fixed binary dithering)\n");

	/* Perform fixed quad dithering */
	dithering(input_img, output_img, DITHERING_FIXED_QUAD);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_fixed_quad.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_fixed_quad.raw (for fixed quad dithering)\n");

	/* Perform random binary dithering */
	dithering(input_img, output_img, DITHERING_RANDOM_BINARY);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_random_binary.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_random_binary.raw (for random binary dithering)\n");

	/* Perform random quad dithering */
	dithering(input_img, output_img, DITHERING_RANDOM_QUAD);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_random_quad.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_random_quad.raw (for random quad dithering)\n");

	/* Perform bayer index 2 binary dithering */
	dithering(input_img, output_img, DITHERING_BAYER_2_BINARY);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_bayer_2_binary.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_bayer_2_binary.raw (for bayer index 2 binary dithering)\n");

	/* Perform bayer index 4 binary dithering */
	dithering(input_img, output_img, DITHERING_BAYER_4_BINARY);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_bayer_4_binary.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_bayer_4_binary.raw (for bayer index 4 binary dithering)\n");

	/* Perform bayer index 8 binary dithering */
	dithering(input_img, output_img, DITHERING_BAYER_8_BINARY);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_bayer_8_binary.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_bayer_8_binary.raw (for bayer index 8 binary dithering)\n");

	/* Perform bayer index 2 quad dithering */
	dithering(input_img, output_img, DITHERING_BAYER_2_QUAD);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_bayer_2_QUAD.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_bayer_2_QUAD.raw (for bayer index 2 quad dithering)\n");

	/* Perform bayer index 4 quad dithering */
	dithering(input_img, output_img, DITHERING_BAYER_4_QUAD);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_bayer_4_QUAD.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_dither_bayer_4_QUAD.raw (for bayer index 4 quad dithering)\n");

	/* Perform bayer index 8 quad dithering */
	dithering(input_img, output_img, DITHERING_BAYER_8_QUAD);
	if((dump_to_file(output_img->buffer, output_img->size, "OUT_dither_bayer_8_QUAD.raw")) != 0) {
		goto FREE_MEM;
	};
	LOG(OUTPUT, "Output Filename: OUT_dither_bayer_8_QUAD.raw (for bayer index 8 quad dithering)\n");

FREE_MEM:

	if(output_img->buffer != NULL) {
		free(output_img->buffer);
		output_img->buffer = NULL;
	}
	if(output_img != NULL) {
		free(output_img);
		output_img = NULL;
	}
	if(input_img->buffer != NULL) {
		free(input_img->buffer);
		input_img->buffer = NULL;
	}
	if(input_img != NULL) {
		free(input_img);
		input_img = NULL;
	}

EXIT:
	return 0;

}
