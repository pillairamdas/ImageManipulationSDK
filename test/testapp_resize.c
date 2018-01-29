#include "interface.h"

int main(int argc, char *argv[])
{

	int 	       status 		= 0;
	tagImageProp  *input_img    = NULL;	
	tagImageProp  *output_img   = NULL;

	/* Check for application validity */
	if(argc < 7) {
		LOG(ERROR, "Usage: %s <input image> <input_width> <input_height> <input bits per pixel> <output_width> <output_height>\n\n", argv[0]);
		goto EXIT;
	}


	input_img = (tagImageProp *) malloc (sizeof(tagImageProp));
	if(input_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;		
	}

	input_img->width  = atoi(argv[2]);
	input_img->height = atoi(argv[3]);
	input_img->bpp    = atoi(argv[4]);

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

	output_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;		
	}

	output_img->width  = atoi(argv[5]);
	output_img->height = atoi(argv[6]);
	output_img->bpp    = input_img->bpp; 
	output_img->pixel_count = output_img->width * output_img->height;
	output_img->size = output_img->pixel_count * output_img->bpp;

	output_img->buffer = (uint8_t *) malloc(output_img->size);
	if(output_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}


	resize(input_img, output_img);

	if((dump_to_file(output_img->buffer, output_img->size, "RESIZE_out.raw")) != 0) {
		goto FREE_MEM;
	}

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