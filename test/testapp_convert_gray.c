#include "interface.h"


/*
Function   : main
Input      : int argc     - Number of command line arguements
			 char *argv[] - Command line arguements
Output     : None
Description: Main function
*/
int main(int argc, char* argv[])
{
	int      	  iterator 		  	    = 0;
	tagImageProp *input_img       	    = NULL;
	tagImageProp *output_img_lightness  = NULL;
	tagImageProp *output_img_average    = NULL;
	tagImageProp *output_img_luminosity = NULL; 	


	/* Check for application validity */
	if(argc < 5) {
		LOG(ERROR, "Usage: %s <input image> <width> <height> <bits per pixel>\n\n", argv[0]);
		goto EXIT;
	}


	/* Allocate memory for input image properties */
	input_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(input_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	input_img->width  = atoi(argv[2]);
	input_img->height = atoi(argv[3]);
	input_img->bpp    = atoi(argv[4]);
	if(input_img->bpp == 1) {
		printf("Image already in grayscale\n");
		goto FREE_MEM;
	}

	input_img->pixel_count = input_img->width * input_img->height;
	input_img->size = input_img->pixel_count * input_img->bpp;
	input_img->buffer = (uint8_t *) malloc(input_img->size);
	if(input_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;	
	}	


	if(read_from_file(input_img->buffer, input_img->size, argv[1]) != 0) {
		goto FREE_MEM;
	}


	output_img_lightness = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_lightness == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}


	output_img_average = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_average == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}


	output_img_luminosity = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_luminosity == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	output_img_luminosity->width  = output_img_average->width  = output_img_lightness->width  = input_img->width;
	output_img_luminosity->height = output_img_average->height = output_img_lightness->height = input_img->height;
	output_img_luminosity->bpp    = output_img_average->bpp    = output_img_lightness->bpp    = GRAY_BITS_PER_PIXEL;

	output_img_luminosity->pixel_count = output_img_luminosity->width * output_img_luminosity->height; 
	output_img_luminosity->size   = output_img_luminosity->pixel_count * output_img_luminosity->bpp;

	output_img_lightness->pixel_count = output_img_lightness->width * output_img_lightness->height;
	output_img_lightness->size    = output_img_lightness->pixel_count * output_img_lightness->bpp;
	
	output_img_average->pixel_count = output_img_average->width * output_img_average->height;
	output_img_average->size    = output_img_average->pixel_count * output_img_average->bpp;



	output_img_luminosity->buffer = (uint8_t *) malloc(output_img_luminosity->size);
	if(output_img_luminosity->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	output_img_average->buffer = (uint8_t *) malloc(output_img_average->size);
	if(output_img_average->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}
	output_img_lightness->buffer = (uint8_t *) malloc(output_img_lightness->size);
	if(output_img_lightness->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	convert(input_img, output_img_lightness, COLORSPACE_GRAY_LIGHTNESS);
	convert(input_img, output_img_average, COLORSPACE_GRAY_AVERAGE);
	convert(input_img, output_img_luminosity, COLORSPACE_GRAY_LUMINOSITY);
	

	if((dump_to_file(output_img_lightness->buffer, output_img_lightness->size, "OUT_GRAY_Lightness.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_GRAY_Lightness.raw\n");
	if((dump_to_file(output_img_average->buffer, output_img_average->size, "OUT_GRAY_Average.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_GRAY_Average.raw\n");
	if((dump_to_file(output_img_luminosity->buffer, output_img_luminosity->size, "OUT_GRAY_Luminosity.raw")) != 0) {
		goto FREE_MEM;
	}
	LOG(OUTPUT, "Output Filename: OUT_GRAY_Luminosity.raw\n");


FREE_MEM:

	if(input_img != NULL) {
		if(input_img->buffer != NULL) {
			free(input_img->buffer);
			input_img->buffer = NULL;
		}
		free(input_img);
		input_img = NULL;
	}

	if(output_img_lightness != NULL) {
		if(output_img_lightness->buffer != NULL) {
			free(output_img_lightness->buffer);
			output_img_lightness->buffer = NULL;
		}
		free(output_img_lightness);
		output_img_lightness = NULL;
	}

	if(output_img_average != NULL) {
		if(output_img_average->buffer != NULL) {
			free(output_img_average->buffer);
			output_img_average->buffer = NULL;
		}
		free(output_img_average);
		output_img_average = NULL;
	}

	if(output_img_luminosity != NULL) {
		if(output_img_luminosity->buffer != NULL) {
			free(output_img_luminosity->buffer);
			output_img_luminosity->buffer = NULL;
		}
		free(output_img_luminosity);
		output_img_luminosity = NULL;
	}

EXIT:
	return 0;

}
