/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "color_halftoning.h"
char color_str[8][80] = { {"WHITE"}, {"YELLOW"}, {"MAGENTA"}, {"RED"}, {"CYAN"}, {"GREEN"}, {"BLUE"}, {"BLACK"} };



/* Floyd-Steinberg's error diffusion matrix */
float cht_floyds_edm[3][3] = {
						   	   {0.0000, 0.0000, 0.0000},
							   {0.0000, 0.0000, 0.4375},
							   {0.1875, 0.3125, 0.0625}
                         	 };


/*
Function   : floyds_error_diffusion
Input      : tagImageProp *input_img  - Input image
             float        *cmy_img    - Output Image in CMY Representation
Output     : None
Description: Convert from RGB to CMY
*/
void get_cmy_representation(tagImageProp *input_img, float* cmy_img)
{
	int iterator = 0;
	uint8_t *i_buffer = NULL;
	float *o_buffer = NULL;

	while(iterator < input_img->pixel_count) {

		i_buffer = input_img->buffer + iterator * input_img->bpp;
		o_buffer = cmy_img + iterator * input_img->bpp;

		*(o_buffer + C_CHANNEL) = 1.0 - (float) (*(i_buffer + R_CHANNEL) / 255.0);
		*(o_buffer + M_CHANNEL) = 1.0 - (float) (*(i_buffer + G_CHANNEL) / 255.0);
		*(o_buffer + Y_CHANNEL) = 1.0 - (float) (*(i_buffer + B_CHANNEL) / 255.0);

		++iterator;
	}
}


/*
Function   : threshold_cmy_pixel
Input      : float *pixel - Pixel value at current location
             float *error - Error computed at the current location
Output     : None
Description: Threshold the pixel to CMY cube vertices based on Euclidean distance.
*/
void threshold_cmy_pixel(float *pixel, float *error)
{
	/* Temporary variables */
	int   iterator   = 0;
	float sum        = 0;
	float min_cost   = 999;
	int   min_vertex = -1;
	float temp       = 0;

	/* Iterate over the 8 vertices and check which has the minimum cost */
	while(iterator < 8) {

		sum = 0;

		sum += pow(((iterator >> 2) & 0x1) - *(pixel + C_CHANNEL),2);
		sum += pow(((iterator >> 1) & 0x1) - *(pixel + M_CHANNEL),2);
		sum += pow(((iterator) & 0x1) - *(pixel + Y_CHANNEL),2);

		if(min_cost > sum) {

			min_cost   = sum;
			min_vertex = iterator;
		}

//		LOG(DEBUG, "Vertex: %s Cost: %0.2f\n", color_str[iterator], sum);

		++iterator;
	}

//	LOG(DEBUG, "MinVertex: %s MinCost: %0.2f\n", color_str[min_vertex], min_cost);


	temp = *(pixel + C_CHANNEL);
	*(pixel + C_CHANNEL) = (min_vertex >> 2) & 0x1;
	*(error + C_CHANNEL) = temp - *(pixel + C_CHANNEL);

	temp = *(pixel + M_CHANNEL);
	*(pixel + M_CHANNEL) = (min_vertex >> 1) & 0x1;
	*(error + M_CHANNEL) = temp - *(pixel + M_CHANNEL);

	temp = *(pixel + Y_CHANNEL);
	*(pixel + Y_CHANNEL) = min_vertex & 0x1;
	*(error + Y_CHANNEL) = temp - *(pixel + Y_CHANNEL);

}


/*
Function   : get_rgb_representation
Input      : tagImageProp *output_img  - Output Image
             tagImageProp *cmy_img - Input image in CMY representation
Output     : None
Description: Convert from CMY to RGB representation
*/
void get_rgb_representation(tagImageProp *output_img, float* cmy_img)
{
	int iterator = 0;
	uint8_t *o_buffer = NULL;
	float   *i_buffer = NULL;

	while(iterator < output_img->pixel_count) {

		o_buffer = output_img->buffer + iterator * output_img->bpp;
		i_buffer = cmy_img + iterator * output_img->bpp;

		*(o_buffer + R_CHANNEL) = (int)((1 - *(i_buffer + C_CHANNEL)) * 255.0);
		*(o_buffer + G_CHANNEL) = (int)((1 - *(i_buffer + M_CHANNEL)) * 255.0);
		*(o_buffer + B_CHANNEL) = (int)((1 - *(i_buffer + Y_CHANNEL)) * 255.0);

		++iterator;
	}
}


/*
Function   : separable_color_halftoning
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Separable color halftoning implementation
*/
void separable_color_halftoning(tagImageProp *input_img, tagImageProp *output_img)
{
	/* Temporary variable */
	int channel_count = 0;


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
	float *error    = NULL;
	float *cmy_img  = NULL;
	float *pixel    = NULL;
	float *temp_buf = NULL;


	cmy_img = (float *) malloc(input_img->size    * sizeof(float));
	if(cmy_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}
	error   = (float *) malloc(CMY_BITS_PER_PIXEL * sizeof(float));
	if(error == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	get_cmy_representation(input_img, cmy_img);

	height = output_img->height;
	width = output_img->width;

	i = 0;
	while(i < height) {

		/* Serpentine Logic */
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

			pixel = cmy_img + (i * input_img->width + j) * input_img->bpp;
			threshold_cmy_pixel(pixel, error);

			/* Error diffusion using Floyd-Steinberg's error diffusion algorithm */
			channel_count = 0;
			while(channel_count < input_img->bpp) {

				if(i < height - 1) {
					temp_buf = cmy_img + ((i+1) * input_img->width + j) * input_img->bpp + channel_count;
					*temp_buf += *(error + channel_count) * cht_floyds_edm[2][1];
					if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
					if( j > 0) {
						temp_buf = cmy_img + ((i+1) * input_img->width + j-1) * input_img->bpp + channel_count;
						*temp_buf += *(error + channel_count) * cht_floyds_edm[2][2 * rev_flag];
						if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
					}
					if(j < width - 1) {
						temp_buf = cmy_img + ((i+1) * input_img->width + j+1) * input_img->bpp + channel_count;
						*temp_buf += *(error + channel_count) * cht_floyds_edm[2][2 * (1 - rev_flag)];
						if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
					}
				}

				if((rev_flag == 0) && (j < j_end)) {
					temp_buf = cmy_img + ((i) * input_img->width + j+1) * input_img->bpp + channel_count;
					*temp_buf += *(error + channel_count) * cht_floyds_edm[1][2];
					if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
				}

				if((rev_flag == 1) && (j > j_end)) {
					temp_buf = cmy_img + (i * input_img->width + j-1) * input_img->bpp + channel_count;
					*temp_buf += *(error + channel_count) * cht_floyds_edm[1][2];
					if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
				}

				++channel_count;
			}

		} while(abs(j_end - j) != 0);
		++i;
	}

	get_rgb_representation(output_img, cmy_img);

FREE_MEM:

	if(error != NULL) {
		free(error);
		error = NULL;
	}

	if(cmy_img != NULL) {
		free(cmy_img);
		cmy_img = NULL;
	}

}


/*
Function   : get_mbvq_type
Input      : float *pixel - Pixel value at current location
Output     : tagMBVQType
Description: Find the MBVQ pyramid for the pixel intensity
*/
tagMBVQType get_mbvq_type(uint8_t *buffer)
{
	uint8_t R = *(buffer + R_CHANNEL);
	uint8_t G = *(buffer + G_CHANNEL);
	uint8_t B = *(buffer + B_CHANNEL);

	if((R+G) > 255) {
		if((G+B) > 255) {
			if((R+G+B) > 510) {
				return CMYW;
			} else {
				return MYGC;
			}
		} else {
			return RGMY;
		}
	} else {
		if(!((G+B) > 255)) {
			if(!((R+G+B) > 255)) {
				return KRGB;
			} else {
				return RGBM;
			}
		} else {
			return CMGB;
		}
	}
}


/*
Function   : threshold_with_mbvq
Input      : float *pixel - Pixel value at current location
             float *error - Error computed at the current location
             tagMBVQType type - MBVQ pyramid type
Output     : None
Description: Threshold the pixel to CMY cube vertices based on
             Euclidean distance for the MBVQ type.
*/
void threshold_with_mbvq(float *pixel, float *error, tagMBVQType type)
{
	/* Temporary Variables */
	int   iterator   = 0;
	float sum        = 0;
	float min_cost   = 999;
	int   min_vertex = -1;
	float temp       = 0;
	int   val        = 0;

	while(iterator < 4) {

		sum = 0;

		val = ((uint32_t)type >> (3 - iterator)*4) & 0x0F;

		sum += pow(((val >> 2) & 0x1) - *(pixel + C_CHANNEL),2);
		sum += pow(((val >> 1) & 0x1) - *(pixel + M_CHANNEL),2);
		sum += pow(((val) & 0x1) - *(pixel + Y_CHANNEL),2);

		if(min_cost > sum) {
			min_cost = sum;
			min_vertex = val;
		}

//		LOG(DEBUG, "Vertex: %s Cost: %0.2f\n", color_str[val], sum);

		++iterator;
	}

//	LOG(DEBUG, "MinVertex: %s MinCost: %0.2f\n", color_str[min_vertex], min_cost);

	temp = *(pixel + C_CHANNEL);
	*(pixel + C_CHANNEL) = (min_vertex >> 2) & 0x1;
	*(error + C_CHANNEL) = temp - *(pixel + C_CHANNEL);

	temp = *(pixel + M_CHANNEL);
	*(pixel + M_CHANNEL) = (min_vertex >> 1) & 0x1;
	*(error + M_CHANNEL) = temp - *(pixel + M_CHANNEL);

	temp = *(pixel + Y_CHANNEL);
	*(pixel + Y_CHANNEL) = min_vertex & 0x1;
	*(error + Y_CHANNEL) = temp - *(pixel + Y_CHANNEL);

}


/*
Function   : mbvq_color_halftoning
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: MBVQ color halftoning implementation
*/
void mbvq_color_halftoning(tagImageProp *input_img, tagImageProp *output_img)
{
	/* Temporary variable */
	int      channel_count = 0;
	uint8_t *buffer        = NULL;


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
	float *error    = NULL;
	float *cmy_img  = NULL;
	float *pixel    = NULL;
	float *temp_buf = NULL;

	tagMBVQType mbvq_type = NONE;

	cmy_img = (float *) malloc(input_img->size    * sizeof(float));
	if(cmy_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}
	error   = (float *) malloc(CMY_BITS_PER_PIXEL * sizeof(float));
	if(error == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	get_cmy_representation(input_img, cmy_img);

	height = output_img->height;
	width = output_img->width;

	i = 0;
	while(i < height) {

		/* Serpentine Logic */
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


			buffer    = get_image_pixel_ptr(input_img, i, j);

			mbvq_type = get_mbvq_type(buffer);

			pixel     = cmy_img + (i * input_img->width + j) * input_img->bpp;

//			LOG(DEBUG, "<------------ [%d %d] [0x%x] [%0.2f %0.2f %0.2f]\n", i, j, mbvq_type, *(pixel + R_CHANNEL), *(pixel + G_CHANNEL), *(pixel + B_CHANNEL));
			threshold_with_mbvq(pixel, error, mbvq_type);

//			LOG(DEBUG, " [%0.2f %0.2f %0.2f] [%0.2f %0.2f %0.2f] ------------>\n", *(pixel + R_CHANNEL), *(pixel + G_CHANNEL), *(pixel + B_CHANNEL), *(error + R_CHANNEL), *(error + G_CHANNEL), *(error + B_CHANNEL));


			/* Error diffusion using Floyd-Steinberg's error diffusion algorithm */
			channel_count = 0;
			while(channel_count < input_img->bpp) {

				if(i < height - 1) {
					temp_buf = cmy_img + ((i+1) * input_img->width + j) * input_img->bpp + channel_count;
					*temp_buf += *(error + channel_count) * cht_floyds_edm[2][1];
					if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;

					if( j > 0) {
						temp_buf = cmy_img + ((i+1) * input_img->width + j-1) * input_img->bpp + channel_count;
						*temp_buf += *(error + channel_count) * cht_floyds_edm[2][2 * rev_flag];
						if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
					}
					if(j < width - 1) {
						temp_buf = cmy_img + ((i+1) * input_img->width + j+1) * input_img->bpp + channel_count;
						*temp_buf += *(error + channel_count) * cht_floyds_edm[2][2 * (1 - rev_flag)];
						if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
					}
				}

				if((rev_flag == 0) && (j < j_end)) {
					temp_buf = cmy_img + ((i) * input_img->width + j+1) * input_img->bpp + channel_count;
					*temp_buf += *(error + channel_count) * cht_floyds_edm[1][2];
					if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
				}

				if((rev_flag == 1) && (j > j_end)) {
					temp_buf = cmy_img + (i * input_img->width + j-1) * input_img->bpp + channel_count;
					*temp_buf += *(error + channel_count) * cht_floyds_edm[1][2];
					if(*temp_buf < 0) *temp_buf = 0; else if(*temp_buf > 1) *temp_buf = 1;
				}


				++channel_count;
			}

		} while(abs(j_end - j) != 0);
		++i;
	}

	get_rgb_representation(output_img, cmy_img);

FREE_MEM:

	if(error != NULL) {
		free(error);
		error = NULL;
	}

	if(cmy_img != NULL) {
		free(cmy_img);
		cmy_img = NULL;
	}
}


/*
Function   : color_halftone_images
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagColorHalftoneMethod method - Method to perform color halftoning
Output     : None
Description: Framework function for color halftoning of images
*/
void color_halftone_images(tagImageProp *input_img, tagImageProp *output_img, tagColorHalftoneMethod method)
{
	if(method == COLOR_HALFTONE_SEPARABLE) {
		separable_color_halftoning(input_img, output_img);
	}
#ifdef ENABLE_MBVQ
	else if(method == COLOR_HALFTONE_MBVQ) {
		mbvq_color_halftoning(input_img, output_img);
	}
#endif
}
