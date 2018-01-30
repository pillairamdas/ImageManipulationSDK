#include "oil_effect.h"

/*
Function   : oil_effect
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int32_t window_size          - Window size
Output     : tagStatus - Error Status for the function
Description: Get oil effect on the input image
*/
tagStatus oil_effect(tagImageProp *input_img, tagImageProp *output_img, int32_t window_size)
{

	/* Temporary Variables */
	int32_t      i                              = 0;
	int32_t      j                              = 0;
	int32_t      iterator 		  	            = 0;
	uint8_t *buffer             			= NULL;

	/* Window size */
	int32_t      N                  			= 0;

	/* Half of window size for readability */
	int32_t      Nby2							= 0;

	/* Indices of window */
	int32_t      window_horiz       			= 0;
	int32_t      window_vert        			= 0;

	/* Indices of pixel of input image */
	int32_t      horiz              			= 0;
	int32_t      vert               			= 0;

	/* Color bins to check similarity in pixel color levels in window */
	int32_t 	 red_bin[RGB_PIXEL_LEVELS]      = {0};
	int32_t      blue_bin[RGB_PIXEL_LEVELS]     = {0};
	int32_t      green_bin[RGB_PIXEL_LEVELS]    = {0};

	if((window_size > 11) || (window_size < 3) || ((window_size & 0x1) == 0)) {
		LOG(ERROR, "%d is not a valid window size\n", window_size);
		return ERROR_WINDOW_SIZE;
	}


	N = window_size;
	Nby2 = N/2;


	/* Iterate through the whole image pixel by pixel */
	iterator = 0;
	while(iterator < output_img->pixel_count) {

		/* Get the horizontal and vertical indices of the input pixel */
		horiz = iterator % output_img->width;
		vert  = iterator / output_img->width;

		/* Clear the color bins for each pixel */
		memset(red_bin, 0, RGB_PIXEL_LEVELS * sizeof(int32_t));
		memset(blue_bin, 0, RGB_PIXEL_LEVELS * sizeof(int32_t));
		memset(green_bin, 0, RGB_PIXEL_LEVELS * sizeof(int32_t));

		/* Iterate over the window elements */ 
		for(i = -Nby2; i <= Nby2; i++) {
			for(j = -Nby2; j <= Nby2; j++) {

				/* Get the vertical and horizontal indices of the window */
				window_vert  = i + vert;
				window_horiz = j + horiz;
				
				/* Check the corner cased for the window */
				if(window_vert < 0)
					window_vert = abs(window_vert) - 1;
				else if(window_vert >= input_img->height)
					window_vert = 2 * input_img->height - 1 - window_vert;

				if(window_horiz < 0)
					window_horiz = abs(window_horiz) - 1;
				else if(window_horiz >= input_img->width)
					window_horiz = 2 * input_img->width - 1 - window_horiz;

				
				/* Get the buffer the window position */ 
				buffer = get_input_pixel_ptr(input_img, window_vert, window_horiz);

				/* Note the color levels of window pixels */
				++red_bin[*(buffer + RED_CHANNEL)];
				++green_bin[*(buffer + GREEN_CHANNEL)];
				++blue_bin[*(buffer + BLUE_CHANNEL)];

			}
		}

		/*
		/* Assign the pixel intensity occuring maximum times on the window 
		/* to the output image pixel 
		*/ 
		*(output_img->buffer + iterator*output_img->bpp + RED_CHANNEL) = index_max(red_bin, RGB_PIXEL_LEVELS);
		*(output_img->buffer + iterator*output_img->bpp + GREEN_CHANNEL) = index_max(green_bin, RGB_PIXEL_LEVELS);
		*(output_img->buffer + iterator*output_img->bpp + BLUE_CHANNEL) = index_max(blue_bin, RGB_PIXEL_LEVELS);

		++iterator;
	}

	return ERROR_NONE;
}	
