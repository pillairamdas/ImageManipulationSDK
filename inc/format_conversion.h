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

#ifndef __FORMAT_CONVERSION_H__
#define __FORMAT_CONVERSION_H__

#include "common.h"
#include "utils.h"


/*
Function   : get_grayscale_lightness 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate grayscale image based on lightness method
*/
void get_grayscale_lightness(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : get_grayscale_average 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate grayscale image based on average method
*/
void get_grayscale_average(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : get_grayscale_luminosity 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate grayscale image based on luminosity method
*/
void get_grayscale_luminosity(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : convert_to_cmy 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to CMY
*/
void convert_to_cmy(tagImageProp *input_img, tagImageProp *output_img);


/*
Function   : convert_to_c 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to CYAN
*/
void convert_to_c(tagImageProp *input_img, tagImageProp *output_img);


/*
Function   : convert_to_m
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to MAGENTA
*/
void convert_to_m(tagImageProp *input_img, tagImageProp *output_img);


/*
Function   : convert_to_y
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to YELLOW
*/
void convert_to_y(tagImageProp *input_img, tagImageProp *output_img);


/*
Function   : convert_to_negativeRGB
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate a negative RGB image.
*/
void convert_to_negativeRGB(tagImageProp *input_img, tagImageProp *output_img);


#endif // #ifndef __FORMAT_CONVERSION_H__