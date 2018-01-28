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
	int 	 	  status 			  = 0;
	int      	  iterator 		  	  = 0;
	tagImageProp *input_img       	  = NULL;
	tagImageProp *output_img_cmy      = NULL;
	tagImageProp *output_img_cyan     = NULL;
	tagImageProp *output_img_magenta  = NULL; 	
	tagImageProp *output_img_yellow   = NULL; 


	/* Check for application validity */
	if(argc < 5) {
		printf("\n\nUsage: %s <input image> <width> <height> <bits per pixel>\n\n", argv[0]);
		goto EXIT;
	}

	/* Allocate memory for input image properties */
	input_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(input_img == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
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
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;	
	}	

	if(read_from_file(input_img->buffer, input_img->size, argv[1]) != 0) {
		goto FREE_MEM;
	}


	output_img_cmy = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_cmy == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	output_img_cyan = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_cyan == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	output_img_magenta = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_magenta == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	output_img_yellow = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_yellow == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	output_img_cmy->width      = input_img->width;
	output_img_yellow->width   = input_img->width;
	output_img_magenta->width  = input_img->width;
	output_img_cyan->width     = input_img->width;

	output_img_cmy->height     = input_img->height;
	output_img_yellow->height  = input_img->height;
	output_img_magenta->height = input_img->height;
	output_img_cyan->height    = input_img->height;

	output_img_cmy->bpp        = CMY_BITS_PER_PIXEL;
	output_img_yellow->bpp     = GRAY_BITS_PER_PIXEL;
	output_img_magenta->bpp    = GRAY_BITS_PER_PIXEL;
	output_img_cyan->bpp       = GRAY_BITS_PER_PIXEL;

	output_img_cmy->pixel_count = output_img_cmy->width * output_img_cmy->height;
	output_img_cmy->size = output_img_cmy->pixel_count * output_img_cmy->bpp;

	output_img_yellow->pixel_count = output_img_yellow->width * output_img_yellow->height; 
	output_img_yellow->size = output_img_yellow->pixel_count * output_img_yellow->bpp;

	output_img_cyan->pixel_count = output_img_cyan->width * output_img_cyan->height;
	output_img_cyan->size = output_img_cyan->pixel_count * output_img_cyan->bpp;
	
	output_img_magenta->pixel_count = output_img_magenta->width * output_img_magenta->height;
	output_img_magenta->size = output_img_magenta->pixel_count * output_img_magenta->bpp;


	output_img_cmy->buffer = (uint8_t *) malloc(output_img_cmy->size);
	if(output_img_cmy->buffer == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	output_img_yellow->buffer = (uint8_t *) malloc(output_img_yellow->size);
	if(output_img_yellow->buffer == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	output_img_magenta->buffer = (uint8_t *) malloc(output_img_magenta->size);
	if(output_img_magenta->buffer == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}
	output_img_cyan->buffer = (uint8_t *) malloc(output_img_cyan->size);
	if(output_img_cyan->buffer == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	convert(input_img, output_img_cmy, COLORSPACE_CMY);	
	convert(input_img, output_img_cyan, COLORSPACE_CYAN);
	convert(input_img, output_img_magenta, COLORSPACE_MAGENTA);
	convert(input_img, output_img_yellow, COLORSPACE_YELLOW);
	

	if((dump_to_file(output_img_cmy->buffer, output_img_cmy->size, "CMY_out.raw")) != 0) {
		goto FREE_MEM;
	}	
	if((dump_to_file(output_img_cyan->buffer, output_img_cyan->size, "CYAN_out.raw")) != 0) {
		goto FREE_MEM;
	}
	if((dump_to_file(output_img_magenta->buffer, output_img_magenta->size, "MAGENTA_out.raw")) != 0) {
		goto FREE_MEM;
	}
	if((dump_to_file(output_img_yellow->buffer, output_img_yellow->size, "YELLOW_out.raw")) != 0) {
		goto FREE_MEM;
	}


FREE_MEM:
	if(input_img->buffer != NULL) {
		free(input_img->buffer);
		input_img->buffer = NULL;
	}

	if(input_img != NULL) {
		free(input_img);
		input_img = NULL;
	}

	if(output_img_cmy->buffer != NULL) {
		free(output_img_cmy->buffer);
		output_img_cmy->buffer = NULL;
	}

	if(output_img_cmy != NULL) {
		free(output_img_cmy);
		output_img_cmy = NULL;
	}

	if(output_img_cyan->buffer != NULL) {
		free(output_img_cyan->buffer);
		output_img_cyan->buffer = NULL;
	}

	if(output_img_cyan != NULL) {
		free(output_img_cyan);
		output_img_cyan = NULL;
	}

	if(output_img_magenta->buffer != NULL) {
		free(output_img_magenta->buffer);
		output_img_magenta->buffer = NULL;
	}

	if(output_img_magenta != NULL) {
		free(output_img_magenta);
		output_img_magenta = NULL;
	}

	if(output_img_yellow->buffer != NULL) {
		free(output_img_yellow->buffer);
		output_img_yellow->buffer = NULL;
	}

	if(output_img_yellow != NULL) {
		free(output_img_yellow);
		output_img_yellow = NULL;
	}

EXIT:
	return 0;

}