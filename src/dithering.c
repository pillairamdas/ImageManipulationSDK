/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/


#include "dithering.h"
#include <time.h>


/* Bayer index matrices */
int I2[2][2] = {   {1, 2},
                   {3, 0} 	};

int I4[4][4] = {   { 5,  9,  6, 10},
				   {13,  1, 14,  2},
				   { 7, 11,  4,  8},
				   {15,  3, 12,  0} 	};

int I8[8][8] = {   {21, 37, 25, 41, 22, 38, 26, 42},
                   {53, 5,  57,  9, 54,  6, 58, 10},
                   {29, 45, 17, 33, 30, 46, 18, 34},
                   {61, 13, 49,  1, 62, 14, 50,  2},
                   {23, 39, 27, 43, 20, 36, 24, 40},
                   {55,  7, 59, 11, 52,  4, 56,  8},
                   {31, 47, 19, 35, 28, 44, 16, 32},
                   {63, 15, 51,  3, 60, 12, 48,  0}  };

/* Threshold matrix based on Bayer index matrices */
float T2[2][2] = { 	{ 95, 159},
					{223,  31} };

float T4[4][4] = {  { 87, 151, 103, 167},
					{215,  23, 231,  39},
					{119, 183,  71, 135},
					{247,  55, 199,   7}     };

float T8[8][8] = {  { 85, 149, 101, 165,  89, 153, 105, 169},
					{213,  21, 229,  37, 217,  25, 233,  41},
					{117, 181,  69, 133, 121, 185,  73, 137},
					{245,  53, 197,   5, 249,  57, 201,   9},
					{ 93, 157, 109, 173,  81, 145,  97, 161},
					{221,  29, 237,  45, 209,  17, 225,  33},
					{125, 189,  77, 141, 113, 177,  65, 129},
					{253,  61, 205,  13, 241,  49, 193,   1}      };

/* Quad colors */
int quad_colors[4] = {0, 85, 170, 255};


/*
Function   : fixed_thresholding
Input      : tagImageProp *input_img   - Input image
             tagImageProp *output_img  - Output image
             tagDithertingMethod index - Index specifying the dithering method used
Output     : None
Description: Perform fixed thresholding to create a dithering impact.
*/
void fixed_thresholding(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod index)
{

	if(index == DITHERING_FIXED_BINARY) {

		int threshold_limit = 127;
		threshold_gray_image(input_img, output_img, threshold_limit);

	} else if(index == DITHERING_FIXED_QUAD) {

		int iterator = 0;
		uint8_t *buffer = NULL;

		while(iterator < input_img->pixel_count) {
			buffer = input_img->buffer + iterator;
			*(output_img->buffer + iterator) = quad_colors[(*buffer + 42) / 85];
			++iterator;
		}
	}
}


/*
Function   : random_thresholding
Input      : tagImageProp *input_img   - Input image
             tagImageProp *output_img  - Output image
             tagDithertingMethod index - Index specifying the dithering method used
Output     : None
Description: Perform random thresholding to create a dithering impact.
*/
void random_thresholding(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod index)
{

	struct   timespec tv     = {0};
	int      threshold_limit = 0;
	int      iterator        = 0;
	uint8_t *buffer          = NULL;

	while(iterator < input_img->pixel_count) {

		clock_gettime(CLOCK_MONOTONIC, &tv);
		srand((time_t)tv.tv_nsec);
		threshold_limit = rand() % 256;

		buffer = input_img->buffer + iterator;

		if(index == DITHERING_RANDOM_BINARY) {

			if(*buffer >= threshold_limit) {
				*(output_img->buffer + iterator) = 255;
			} else {
				*(output_img->buffer + iterator) = 0;
			}

		} else if(index == DITHERING_RANDOM_QUAD) {

			if(*buffer <= threshold_limit / 2) {
				*(output_img->buffer + iterator) = 0;
			} else if(*buffer <= 127) {
				*(output_img->buffer + iterator) = 85;
			} else if(*buffer <= 128 + (threshold_limit / 2)) {
				*(output_img->buffer + iterator) = 170;
			} else {
				*(output_img->buffer + iterator) = 255;
			}

		}

		++iterator;
	}
}


/*
Function   : bayer_matrix_dithering
Input      : tagImageProp *input_img   - Input image
             tagImageProp *output_img  - Output image
             tagDithertingMethod index - Index specifying the dithering method used
Output     : None
Description: Perform bayer index matrix dithering.
*/
void bayer_matrix_dithering(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod index)
{
	int      iterator = 0;
	int      horiz    = 0;
	int      vert     = 0;
	uint8_t *buffer   = NULL;


	iterator = 0;
	while(iterator < input_img->pixel_count) {

		horiz = iterator % input_img->width;
		vert  = iterator / input_img->width;

		buffer = input_img->buffer + iterator;

		if(index == DITHERING_BAYER_2_BINARY) {

			if(*buffer > T2[vert % 2][horiz % 2]) {
				*(output_img->buffer + iterator) = 255;
			} else {
				*(output_img->buffer + iterator) = 0;
			}

		} else if(index == DITHERING_BAYER_4_BINARY) {

			if(*buffer > T4[vert % 4][horiz % 4]) {
				*(output_img->buffer + iterator) = 255;
			} else {
				*(output_img->buffer + iterator) = 0;
			}

		} else if(index == DITHERING_BAYER_8_BINARY) {

			if(*buffer > T8[vert % 8][horiz % 8]) {
				*(output_img->buffer + iterator) = 255;
			} else {
				*(output_img->buffer + iterator) = 0;
			}

		} else if(index == DITHERING_BAYER_2_QUAD) {

			if(*buffer <= T2[vert % 2][horiz % 2]/2) {

				*(output_img->buffer + iterator) = 0;

			} else if(*buffer <= 127) {

				*(output_img->buffer + iterator) = 85;

			} else if(*buffer <= 128 + (T2[vert % 2][horiz % 2]/2)) {

				*(output_img->buffer + iterator) = 170;

			} else {

				*(output_img->buffer + iterator) = 255;

			}

		} else if(index == DITHERING_BAYER_4_QUAD) {

			if(*buffer <= T4[vert % 2][horiz % 2]/2) {

				*(output_img->buffer + iterator) = 0;

			} else if(*buffer <= 127) {

				*(output_img->buffer + iterator) = 85;

			} else if(*buffer <= 128 + (T4[vert % 2][horiz % 2]/2)) {

				*(output_img->buffer + iterator) = 170;

			} else {

				*(output_img->buffer + iterator) = 255;

			}

		} else if(index == DITHERING_BAYER_8_QUAD) {

			if(*buffer <= T8[vert % 2][horiz % 2]/2) {

				*(output_img->buffer + iterator) = 0;

			} else if(*buffer <= 127) {

				*(output_img->buffer + iterator) = 85;

			} else if(*buffer <= 128 + (T8[vert % 2][horiz % 2]/2)) {

				*(output_img->buffer + iterator) = 170;

			} else {

				*(output_img->buffer + iterator) = 255;

			}

		}
		++iterator;
	}
}
