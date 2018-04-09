/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#ifndef __COLOR_HALFTONING_H__
#define __COLOR_HALFTONING_H__

#define ENABLE_MBVQ

#include "common.h"
#include "utils.h"

/* Enumerator for vertices of CMY Cube */
typedef enum tagCMYCubeCoord {
	WHITE    = 0x0,
	YELLOW   = 0x1,
	CYAN     = 0x4,
	MAGENTA  = 0x2,
	GREEN    = 0x5,
	RED      = 0x3,
	BLUE     = 0x6,
	BLACK    = 0x7
} tagCMYCubeCoord;

/* Enumerator for MBVQ pyramids */
typedef enum tagMBVQType {
	CMYW     = 0x4210,
	MYGC     = 0x2154,
	RGMY     = 0x3521,
	KRGB     = 0x7356,
	RGBM     = 0x3562,
	CMGB     = 0x4256,
	NONE     = 0x0000
} tagMBVQType;


/*
Function   : floyds_error_diffusion
Input      : tagImageProp *input_img  - Input image
             float        *cmy_img    - Output Image in CMY Representation
Output     : None
Description: Convert from RGB to CMY
*/
void get_cmy_representation(tagImageProp *input_img, float* cmy_img);

/*
Function   : threshold_cmy_pixel
Input      : float *pixel - Pixel value at current location
             float *error - Error computed at the current location
Output     : None
Description: Threshold the pixel to CMY cube vertices based on Euclidean distance.
*/
void threshold_cmy_pixel(float *pixel, float *error);

/*
Function   : get_rgb_representation
Input      : tagImageProp *output_img  - Output Image
             tagImageProp *cmy_img - Input image in CMY representation
Output     : None
Description: Convert from CMY to RGB representation
*/
void get_rgb_representation(tagImageProp *output_img, float* cmy_img);


/*
Function   : separable_color_halftoning
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Separable color halftoning implementation
*/
void separable_color_halftoning(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : get_mbvq_type
Input      : float *pixel - Pixel value at current location
Output     : tagMBVQType
Description: Find the MBVQ pyramid for the pixel intensity
*/
tagMBVQType get_mbvq_type(uint8_t *buffer);

/*
Function   : threshold_with_mbvq
Input      : float *pixel - Pixel value at current location
             float *error - Error computed at the current location
             tagMBVQType type - MBVQ pyramid type
Output     : None
Description: Threshold the pixel to CMY cube vertices based on
             Euclidean distance for the MBVQ type.
*/
void threshold_with_mbvq(float  *pixel, float  *error, tagMBVQType type);

/*
Function   : mbvq_color_halftoning
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: MBVQ color halftoning implementation
*/
void mbvq_color_halftoning(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : color_halftone_images
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagColorHalftoneMethod method - Method to perform color halftoning
Output     : None
Description: Framework function for color halftoning of images
*/
void color_halftone_images(tagImageProp *input_img, tagImageProp *output_img, tagColorHalftoneMethod method);

#endif // #ifndef __COLOR_HALFTONING_H__
