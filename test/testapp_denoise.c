#include "interface.h"

int main(int argc, char *argv[])
{

	int 	       status 		= 0;
	tagImageProp  *input_img    = NULL;	
	tagImageProp  *original_img    = NULL;	
	tagImageProp  *output_img_median  = NULL;	
	tagImageProp  *output_img   = NULL;
	int            window_size  = 0;

	/* Check for application validity */
	if(argc < 7) {
		printf("\n\nUsage: %s <input image> <original_image> <input_width> <input_height> <input bits per pixel> <window size>\n\n", argv[0]);
		goto EXIT;
	}


	input_img = (tagImageProp *) malloc (sizeof(tagImageProp));
	if(input_img == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;		
	}

	input_img->width  = atoi(argv[3]);
	input_img->height = atoi(argv[4]);
	input_img->bpp    = atoi(argv[5]);
	window_size       = atoi(argv[6]);

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


	original_img = (tagImageProp *) malloc (sizeof(tagImageProp));
	if(original_img == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;		
	}

	original_img->width  = atoi(argv[3]);
	original_img->height = atoi(argv[4]);
	original_img->bpp    = atoi(argv[5]);
	window_size       = atoi(argv[6]);

	original_img->pixel_count = original_img->width * original_img->height;
	original_img->size = original_img->pixel_count * original_img->bpp;

	original_img->buffer = (uint8_t *) malloc(original_img->size);
	if(original_img->buffer == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	if(read_from_file(original_img->buffer, original_img->size, argv[2]) != 0) {
		goto FREE_MEM;
	}


	output_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;		
	}

	output_img->width  = input_img->width;
	output_img->height = input_img->height;
	output_img->bpp    = input_img->bpp; 
	output_img->pixel_count = output_img->width * output_img->height;
	output_img->size = output_img->pixel_count * output_img->bpp;

	output_img->buffer = (uint8_t *) malloc(output_img->size);
	if(output_img->buffer == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	output_img_median = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img_median == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;		
	}

	output_img_median->width  = input_img->width;
	output_img_median->height = input_img->height;
	output_img_median->bpp    = input_img->bpp; 
	output_img_median->pixel_count = output_img_median->width * output_img_median->height;
	output_img_median->size = output_img_median->pixel_count * output_img_median->bpp;

	output_img_median->buffer = (uint8_t *) malloc(output_img_median->size);
	if(output_img_median->buffer == NULL) {
		printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
		goto FREE_MEM;
	}

	remove_image_noise(input_img, output_img_median, window_size, NOISE_REMOVE_MEDIAN_FILTER);
	remove_image_noise(output_img_median, output_img, window_size, NOISE_REMOVE_GAUSSIAN_FILTER);

	printf("\n########################################################\n");
	printf("Statistics for Noisy Image\n");
	print_psnr(original_img, input_img);
	printf("\n########################################################\n");
	printf("Statistics for Image after Median filtering\n");
	print_psnr(original_img, output_img_median);
	printf("\n########################################################\n");
	printf("Statistics for Image after Gaussian filtering\n");
	print_psnr(original_img, output_img);
	printf("\n########################################################\n");	


	if((dump_to_file(output_img->buffer, output_img->size, "Basic_Filtering.raw")) != 0) {
		goto FREE_MEM;
	}


	remove_image_noise(output_img_median, output_img, window_size, NOISE_REMOVE_LINEAR_FILTER);
	printf("Statistics for Image after Linear filtering\n");
	print_psnr(original_img, output_img);
	printf("\n########################################################\n");	


	if((dump_to_file(output_img->buffer, output_img->size, "Linear_Filtering.raw")) != 0) {
		goto FREE_MEM;
	}

	
	if((dump_to_file(output_img_median->buffer, output_img_median->size, "Median_Filtered.raw")) != 0) {
		goto FREE_MEM;
	}




	// int i = 0;
	// uint8_t *buffer;
	// while(i < output_img_median->pixel_count) {
	// 	buffer = output_img_median->buffer + i*output_img_median->bpp;
	// 	printf("%d %d %d\n", *(buffer + 0), *(buffer + 1) , *(buffer + 2));
	// 	++i;
	// }



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



