/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "error_diffusion.h"

/* Threshold for diffusion */
#define THRESHOLD_LIMIT 		127

/* Floyd-Steinberg's error diffusion matrix */
float floyds_edm[3][3] = {
						   {0.0000, 0.0000, 0.0000},
                           {0.0000, 0.0000, 0.4375},
                           {0.1875, 0.3125, 0.0625}
                         };

/* Jarvis, Judice, and Ninke (JJN) diffusion matrix */
float jjn_edm[5][5] = {
					       {0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
					       {0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
					       {0.0000, 0.0000, 0.0000, 0.1458, 0.1042},
					       {0.0625, 0.1042, 0.1458, 0.1042, 0.0625},
					       {0.0208, 0.0625, 0.1042, 0.0625, 0.0208}
					  };

/* Stucki diffusion matrix */
float stucki_edm[5][5] = {
						   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
						   {0.0000, 0.0000, 0.0000, 0.0000, 0.0000},
						   {0.0000, 0.0000, 0.0000, 0.1905, 0.0952},
						   {0.0476, 0.0952, 0.1905, 0.0952, 0.0476},
						   {0.0238, 0.0476, 0.0952, 0.0476, 0.0238}
						 };


/*
Function   : floyds_error_diffusion
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Floyd-Steinberg's error diffusion implementation
*/
void floyds_error_diffusion(tagImageProp *input_img, tagImageProp *output_img)
{
	/* Temporary variables */
	int      iterator = 0;
	uint8_t *buffer   = NULL;
	int      temp_val = 0;
	uint8_t *temp_buf = NULL;

	/* Local copy of image height and width */
	int width  = 0;
	int height = 0;

	/* Image coordinate parameters */
	int i        = 0;
	int j_start  = 0;
	int j_end    = 0;
	int j        = 0;

	/* Reverse flag to implement serpentine flow */
	int rev_flag = 0;

	/* Error calculated at a particular pixel */
	int error    = 0;


	height = output_img->height;
	width  = output_img->width;

	i = 0;
	while(i < height) {

		/* Implementation for serpentine */
		if(i % 2 == 0) {
			rev_flag = 0;
			j_start  = -1;
			j_end    = width-1;
		} else {
			rev_flag = 1;
			j_start  = width;
			j_end    = 0;
		}

		j = j_start;
		do {

			if(rev_flag)
				j--;
			else
				j++;

			/* Get the pixel at location (i, j) */
			buffer = get_image_pixel_ptr(output_img, i, j);

			/* Threshold the current pixel with threshold limit defined above */
			if(*buffer > THRESHOLD_LIMIT) {
				error = *buffer - 255;
				*buffer = 255;
			} else {
				error = *buffer;
				*buffer = 0;
			}

			/* Handle the corner cases */
			if(i < height - 1) {
				temp_buf = get_image_pixel_ptr(output_img, i+1, j);
				temp_val = *temp_buf + (float)(error * floyds_edm[2][1]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				if( j > 0) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * floyds_edm[2][2 * rev_flag]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
				if(j < width - 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * floyds_edm[2][2 * (1 - rev_flag)]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
			}
			if((rev_flag == 0) && (j < j_end)) {
				temp_buf = get_image_pixel_ptr(output_img, i, j+1);
				temp_val = *temp_buf + (float)(error * floyds_edm[1][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			}
			if((rev_flag == 1) && (j > j_end)) {
				temp_buf = get_image_pixel_ptr(output_img, i, j-1);
				temp_val = *temp_buf + (float)(error * floyds_edm[1][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			}

		} while(j != j_end);

		++i;
	}
}


/*
Function   : jjn_error_diffusion
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Jarvis, Judice, and Ninke (JJN) error diffusion implementation
*/
void jjn_error_diffusion(tagImageProp *input_img, tagImageProp *output_img)
{
	/* Temporary variable */
	int      iterator = 0;
	uint8_t *buffer   = NULL;
	int      temp_val = 0;
	uint8_t *temp_buf = NULL;

	/* Local copy of image height and width */
	int width  = 0;
	int height = 0;


	/* Image coordinate parameters */
	int i       = 0;
	int j_start = 0;
	int j_end   = 0;
	int j       = 0;

	/* Reverse flag to implement serpentine flow */
	int rev_flag = 0;

	/* Error calculated at a particular pixel */
	int error    = 0;



	width  = output_img->width;
	height = output_img->height;

	iterator = 0;
	while(iterator < output_img->pixel_count) {

		i = iterator / width;
		j = iterator % width;

		buffer = output_img->buffer + iterator;


		/* Threshold the current pixel with threshold limit defined above */
		if(*buffer > THRESHOLD_LIMIT) {
			error = *buffer - 255;
			*buffer = 255;
		} else {
			error = *buffer;
			*buffer = 0;
		}

		/* Handle the border cases */
			if(i < height - 2) {
				temp_buf = get_image_pixel_ptr(output_img, i+1, j);
				temp_val = *temp_buf + (float)(error * jjn_edm[3][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				temp_buf = get_image_pixel_ptr(output_img, i+2, j);
				temp_val = *temp_buf + (float)(error * jjn_edm[4][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;

				if(j > 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-2);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-2);
					temp_val = *temp_buf + (float)(error * jjn_edm[4][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-1);
					temp_val = *temp_buf + (float)(error * jjn_edm[4][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if(j > 0) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-1);
					temp_val = *temp_buf + (float)(error * jjn_edm[4][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
				if(j < width - 2) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+2);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+2);
					temp_val = *temp_buf + (float)(error * jjn_edm[4][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+1);
					temp_val = *temp_buf + (float)(error * jjn_edm[4][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if( j < width - 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+1);
					temp_val = *temp_buf + (float)(error * jjn_edm[4][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}

			} else  if(i < height - 1) {
				temp_buf = get_image_pixel_ptr(output_img, i+1, j);
				temp_val = *temp_buf + (float)(error * jjn_edm[3][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;

				if(j > 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-2);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if(j > 0) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
				if(j < width - 2) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+2);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if( j < width - 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * jjn_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
			}
			if(j < width - 2) {
				temp_buf = get_image_pixel_ptr(output_img, i, j+2);
				temp_val = *temp_buf + (float)(error * jjn_edm[2][4]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				temp_buf = get_image_pixel_ptr(output_img, i, j+1);
				temp_val = *temp_buf + (float)(error * jjn_edm[2][3]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			} else if( j < width - 1) {
				temp_buf = get_image_pixel_ptr(output_img, i, j+1);
				temp_val = *temp_buf + (float)(error * jjn_edm[2][3]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			}



			++iterator;
		}

	}


	/*
	Function   : stucki_error_diffusion
	Input      : tagImageProp *input_img  - Input image
	             tagImageProp *output_img - Output Image
	Output     : None
	Description: Stucki error diffusion implementation
	*/
	void stucki_error_diffusion(tagImageProp *input_img, tagImageProp *output_img)
	{
		/* Temporary variable */
		int      iterator = 0;
		uint8_t *buffer   = NULL;
		uint8_t *temp_buf = NULL;
		int      temp_val = 0;

		/* Local copy of image height and width */
		int width  = 0;
		int height = 0;


		/* Image coordinate parameters */
		int i       = 0;
		int j_start = 0;
		int j_end   = 0;
		int j       = 0;

		/* Reverse flag to implement serpentine flow */
		int rev_flag = 0;

		/* Error calculated at a particular pixel */
		int error    = 0;


		iterator = 0;

		width  = output_img->width;
		height = output_img->height;

		while(iterator < output_img->pixel_count) {

			i = iterator / width;
			j = iterator % width;


			buffer = output_img->buffer + iterator;

			/* Threshold the current pixel with threshold limit defined above */
			if(*buffer > THRESHOLD_LIMIT) {
				error = *buffer - 255;
				*buffer = 255;
			} else {
				error = *buffer;
				*buffer = 0;
			}

			/* Handle the border cases */
			if(i < height - 2) {
				temp_buf = get_image_pixel_ptr(output_img, i+1, j);
				temp_val = *temp_buf + (float)(error * stucki_edm[3][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				temp_buf = get_image_pixel_ptr(output_img, i+2, j);
				temp_val = *temp_buf + (float)(error * stucki_edm[4][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;

				if(j > 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-2);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if(j > 0) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
				if(j < width - 2) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+2);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if( j < width - 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}

			} else  if(i < height - 1) {
				temp_buf = get_image_pixel_ptr(output_img, i+1, j);
				temp_val = *temp_buf + (float)(error * stucki_edm[3][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;

				if(j > 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if(j > 0) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
				if(j < width - 2) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if( j < width - 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
			}
			if(j < width - 2) {
				temp_buf = get_image_pixel_ptr(output_img, i, j+2);
				temp_val = *temp_buf + (float)(error * stucki_edm[2][4]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				temp_buf = get_image_pixel_ptr(output_img, i, j+1);
				temp_val = *temp_buf + (float)(error * stucki_edm[2][3]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			} else if( j < width - 1) {
				temp_buf = get_image_pixel_ptr(output_img, i, j+1);
				temp_val = *temp_buf + (float)(error * stucki_edm[2][3]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			}



			++iterator;
		}

	}


	/*
	Function   : custom_error_diffusion
	Input      : tagImageProp *input_img  - Input image
	             tagImageProp *output_img - Output Image
	Output     : None
	Description: Custom error diffusion implementation
	*/
	void custom_error_diffusion(tagImageProp *input_img, tagImageProp *output_img)
	{
		/* Temporary variable */
		int      iterator = 0;
		uint8_t *buffer   = NULL;
		uint8_t *temp_buf = NULL;
		int      temp_val = 0;

		/* Local copy of image height and width */
		int width  = 0;
		int height = 0;


		/* Image coordinate parameters */
		int i       = 0;
		int j_start = 0;
		int j_end   = 0;
		int j       = 0;

		/* Reverse flag to implement serpentine flow */
		int rev_flag = 0;

		/* Error calculated at a particular pixel */
		int error    = 0;


		iterator = 0;

		width  = output_img->width;
		height = output_img->height;

		while(iterator < output_img->pixel_count) {

			i = iterator / width;
			j = iterator % width;


			buffer = output_img->buffer + iterator;

			/* Threshold the current pixel with threshold limit defined above */
			if(*buffer > THRESHOLD_LIMIT) {
				/* Take only 80% of the error */
				error = ((*buffer - 255) * 4) / 5;
				*buffer = 255;
			} else {
				error = ((*buffer * 4) / 5);
				*buffer = 0;
			}

			/* Handle the border cases */
			if(i < height - 2) {
				temp_buf = get_image_pixel_ptr(output_img, i+1, j);
				temp_val = *temp_buf + (float)(error * stucki_edm[3][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				temp_buf = get_image_pixel_ptr(output_img, i+2, j);
				temp_val = *temp_buf + (float)(error * stucki_edm[4][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;

				if(j > 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-2);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if(j > 0) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
				if(j < width - 2) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+2);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if( j < width - 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+2, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[4][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}

			} else  if(i < height - 1) {
				temp_buf = get_image_pixel_ptr(output_img, i+1, j);
				temp_val = *temp_buf + (float)(error * stucki_edm[3][2]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;

				if(j > 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][0]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if(j > 0) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j-1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][1]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
				if(j < width - 2) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+2);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][4]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				} else if( j < width - 1) {
					temp_buf = get_image_pixel_ptr(output_img, i+1, j+1);
					temp_val = *temp_buf + (float)(error * stucki_edm[3][3]);
					if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				}
			}
			if(j < width - 2) {
				temp_buf = get_image_pixel_ptr(output_img, i, j+2);
				temp_val = *temp_buf + (float)(error * stucki_edm[2][4]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
				temp_buf = get_image_pixel_ptr(output_img, i, j+1);
				temp_val = *temp_buf + (float)(error * stucki_edm[2][3]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			} else if( j < width - 1) {
				temp_buf = get_image_pixel_ptr(output_img, i, j+1);
				temp_val = *temp_buf + (float)(error * stucki_edm[2][3]);
				if(temp_val < 0) temp_val = 0; else if(temp_val > 255) temp_val = 255; *temp_buf = temp_val;
			}

			++iterator;
		}
	}


	/*
	Function   : error_diffusion_binary
	Input      : tagImageProp *input_img        - Input image
	             tagImageProp *output_img       - Output Image
	             tagErrorDiffusionMethod method - Error diffusion method
	Output     : None
	Description: Error diffusion binary framework function
	*/
	void error_diffusion_binary(tagImageProp *input_img, tagImageProp *output_img, tagErrorDiffusionMethod method)
	{
		memcpy(output_img->buffer, input_img->buffer, input_img->size);

		if(method == ERROR_DIFFUSE_FLYD_STEIN) {
			floyds_error_diffusion(input_img, output_img);
		} else if(method == ERROR_DIFFUSE_JJN) {
			jjn_error_diffusion(input_img, output_img);
		} else if(method == ERROR_DIFFUSE_STUCKI) {
			stucki_error_diffusion(input_img, output_img);
		} else {
			custom_error_diffusion(input_img, output_img);
		}
	}
