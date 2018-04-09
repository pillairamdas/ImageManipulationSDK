/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "thresholding.h"

/*
Function   : threshold_gray_image
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int32_t threshold_limit  - Threshold limit
Output     : None
Description: Perform thresholding on the input gray scale image
*/
void threshold_gray_image(tagImageProp *input_img, tagImageProp *output_img, int32_t threshold_limit)
{
	int32_t    iterator = 0;
	int32_t    tempval  = 0;
	uint8_t   *buffer   = NULL;

	while(iterator < input_img->pixel_count) {

		buffer = input_img->buffer + iterator*input_img->bpp;
		if(*buffer > threshold_limit) {
			tempval = 255;
		} else {
			tempval = 0;
		}

		*(output_img->buffer + iterator*output_img->bpp) = tempval;
		++iterator;
	}
}
