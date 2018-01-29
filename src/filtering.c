#include "filtering.h"


int create_linear_kernel(int **kernel, int window_size) {

	/* Loop Variables */
	int i    = 0;
	int j    = 0;

	/* Sum of the kernel elements */
	int filter_sum = 0;

	/* window_size/2 for readability */
	int Nby2 = window_size/2;

	for(i = 0; i < window_size; i++) {
		for(j = 0; j < window_size; j++) {
			kernel[i][j] = 1;
			filter_sum += kernel[i][j];
		}
	}

	return filter_sum;

}


int create_gaussian_kernel(int **kernel, int window_size) 
{
	int Nby2 = window_size/2;
	int i    = 0;
	int j    = 0;
	int filter_sum = 0;
	
	/* Kernel coordinates */
	int      khoriz       = 0;
	int      kvert        = 0;

	for(i = -Nby2; i <= Nby2; i++) {
		for(j = -Nby2; j <= Nby2; j++) {

			/* Compute the corresponding kernel indices */
			if(i <= 0) {
				kvert = i + Nby2;
			} else {
				kvert = Nby2 - i;
			}

			if(j <= 0) {
				khoriz = j + Nby2;
			} else {
				khoriz = Nby2 - j;
			}

			/* Compute the kernel value for the indices */
			kernel[i + Nby2][j + Nby2] = pow(2, (khoriz + kvert));
			filter_sum += kernel[i + Nby2][j + Nby2];
		}
	}

	return filter_sum;
}



/*
Function   : median_filtering
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int window_size          - Size of the window
Output     : ERROR_NONE on success
			 ERROR_WINDOW_SIZE on incorrect window size
Description: Perform median filtering on the input image
*/
void median_filtering(tagImageProp *input_img, tagImageProp *output_img, int window_size)
{
	/* Temporary Variables */
	int      iterator    = 0;
	int      i           = 0;
	int      j           = 0;
	int      k           = 0;
	int      m           = 0;
	int      n           = 0;
	uint8_t *buffer      = NULL;

	/* Window Size */
	int      N           = 0;

	/* N/2 for readability */
	int      Nby2		 = 0;

	/* Pixel coordinates */
	int      horiz       = 0;
	int      vert        = 0;

	/* Dynamic bins to store the window pixel intensities */
	int      *red_bin    = NULL;
	int      *green_bin  = NULL;
	int      *blue_bin   = NULL;


	/* Store window size and compute Nby2 */ 
	N = window_size;
	Nby2 = N / 2;

	/* Allocate memory for bins based on window size */
	red_bin   = (int *) malloc(N * N * sizeof(int));
	if(red_bin == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}
	green_bin = (int *) malloc(N * N * sizeof(int));
	if(green_bin == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}	
	blue_bin  = (int *) malloc(N * N * sizeof(int));
	if(blue_bin == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}	

	/* Iterate over all the pixels */
	iterator = 0;
	while(iterator < input_img->pixel_count) {

		/* Compute the pixel coordinates */
		horiz = iterator % input_img->width;
		vert  = iterator / input_img->width;

		/* Reset the color bins for each pixel */
		memset(red_bin, 0x0, N * N * sizeof(int));
		memset(green_bin, 0x0, N * N * sizeof(int));
		memset(blue_bin, 0x0, N * N * sizeof(int));
	
		/* Iterate over the window */
		k = 0;
		for(i = -Nby2; i <= Nby2; i++) {
			for(j = -Nby2; j <= Nby2; j++) {
			
				/* Compute the window pixel coordinates */
				m = i + vert;
				n = j + horiz;

				/* Handle the pixel boundary conditions */
				if(m < 0)
					m = abs(m) - 1;
				if(n < 0)
					n = abs(n) - 1;

				if(m > input_img->height)
					m = 2 * input_img->height + 1 - m;
				if(n > input_img->width)
					n = 2 * input_img->width + 1 - n;

				/* Fetch the pixel on the window */
				buffer = get_input_pixel_ptr(input_img, m, n);

				/* Store the window pixel intensities in the array */
				red_bin[k]   = *(buffer + RED_CHANNEL);
				green_bin[k] = *(buffer + GREEN_CHANNEL);
				blue_bin[k]  = *(buffer + BLUE_CHANNEL);
				++k;
			}
		}

		/* Get the median and assign to the output image */
		*(output_img->buffer + iterator*input_img->bpp + RED_CHANNEL) = get_median(red_bin, N*N);
		*(output_img->buffer + iterator*input_img->bpp + GREEN_CHANNEL) = get_median(green_bin, N*N);
		*(output_img->buffer + iterator*input_img->bpp + BLUE_CHANNEL) = get_median(blue_bin, N*N);

		++iterator;
	}

FREE_MEM:

	if(blue_bin != NULL) {
		free(blue_bin);
		blue_bin = NULL;
	}
	if(green_bin != NULL) {
		free(green_bin);
		green_bin = NULL;
	}
	if(red_bin != NULL) {
		free(red_bin);
		red_bin = NULL;
	}
}


/*
Function   : gaussian_filtering 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int window_size          - dimension of window
Output     : None
Description: Apply gaussian filter on the input image
*/
tagStatus apply_filter(tagImageProp *input_img, tagImageProp *output_img, int window_size, tagFilterType filter)
{
	/* Temporary Variables */
	int      iterator 		  	= 0;
	uint8_t *buffer             = NULL;
	int 	 i                  = 0;
	int 	 j					= 0;
	int 	 m					= 0;
	int 	 n					= 0;			

	/* Window size */
	int      N             	 	= 0;

	/* N/2 for readability */
	int      Nby2				= 0;

	/* Pixel 2D corrdinates */
	int      horiz              = 0;
	int      vert               = 0;

	/* Kernel value at the coordinate */
	int    **kernel             = 0;
	int      filter_sum         = 0;
	int      row_sum            = 0;
	int 	 sum_channel_val[RGB_BITS_PER_PIXEL] = {0};



	if((window_size > 11) || (window_size < 3) || ((window_size & 0x1) == 0)) {
		LOG(ERROR, "%d is not a valid window size\n", window_size);
		return ERROR_WINDOW_SIZE;
	}

	if(filter == FILTER_MEDIAN) {
		median_filtering(input_img, output_img, window_size);
	} else {

		/* Assign window size and Nby2 */
		N = window_size;
		Nby2 = N/2;

		kernel = (int **) malloc(N * sizeof(int *));
		if(kernel == NULL) {
			LOG(ERROR, "Memory unavailable!\n");
			goto FREE_MEM;
		}
		for(i = 0; i < N; ++i) {
			kernel[i] = (int *) malloc(N * sizeof(int));
			if(kernel[i] == NULL) {
				LOG(ERROR, "Memory unavailable!\n");
				goto FREE_MEM;
			}			
		}



		if(filter == FILTER_GAUSSIAN) {
			filter_sum = create_gaussian_kernel(kernel, window_size);
		} else {
			filter_sum = create_linear_kernel(kernel, window_size);
		}


		/* Iterate over the image pixel by pixel */
		iterator = 0;
		while(iterator < input_img->pixel_count) {

			/* Get the corresponding pixel coordinates */
			horiz = iterator % input_img->width;
			vert  = iterator / input_img->width;

			/* Reset sum channel array for each pixel */
			memset(sum_channel_val, 0, RGB_BITS_PER_PIXEL * sizeof(int));

			/* Iterate over the window */
			for(i = -Nby2; i <= Nby2; i++) {
				for(j = -Nby2; j <= Nby2; j++) {

					/* Get the pixel coordinates for the window elements */
					m = i + vert;
					n = j + horiz;

					/* Handle the boundary conditions for the window */
					if(m < 0)
						m = abs(m) - 1;
					if(n < 0)
						n = abs(n) - 1;

					if(m > input_img->height)
						m = 2 * input_img->height + 1 - m;
					if(n > input_img->width)
						n = 2 * input_img->width + 1 - n;

					/* Fetch the pixel at window coordinates m,n */
					buffer = get_input_pixel_ptr(input_img, m, n);

					/* Apply the Gaussian kernel on the window pixels */
					sum_channel_val[RED_CHANNEL] += *(buffer) * kernel[i + Nby2][j + Nby2];
					sum_channel_val[GREEN_CHANNEL] += *(buffer + GREEN_CHANNEL) * kernel[i + Nby2][j + Nby2];
					sum_channel_val[BLUE_CHANNEL] += *(buffer + BLUE_CHANNEL) * kernel[i + Nby2][j + Nby2];
				}
			}

			/* Normal the gray scale values by the gaussian filter sum and assign to output*/
			*(output_img->buffer + iterator*output_img->bpp + RED_CHANNEL) = sum_channel_val[RED_CHANNEL] / filter_sum;
			*(output_img->buffer + iterator*output_img->bpp + GREEN_CHANNEL) = sum_channel_val[GREEN_CHANNEL] / filter_sum;
			*(output_img->buffer + iterator*output_img->bpp + BLUE_CHANNEL) = sum_channel_val[BLUE_CHANNEL] / filter_sum;

			++iterator;
		}
	}


FREE_MEM:

	for(i = 0; i < N; ++i) {
		if(kernel[i] != NULL) {
			free(kernel[i]);
			kernel[i] = NULL;
		}
	}

	if(kernel != NULL) {
		free(kernel);
		kernel = NULL;
	}

}