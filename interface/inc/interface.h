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


#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "common.h" 
#include "format_conversion.h"
#include "resize.h"
#include "color_conversion.h"
#include "oil_effect.h"
#include "histogram_modification_transfer.h"
#include "filtering.h"
#include "utils.h"
#include "histogram_equalization_uniform.h"
#include "reflect.h"

/*
Function   : convert
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Convert RGB to mentioned format
*/
void convert(tagImageProp *input_img, tagImageProp *output_img, tagColorspace cs);


/*
Function   : resize
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Resize the input image
*/
void resize(tagImageProp *input_img, tagImageProp *output_img);


/*
Function   : change_color_palette
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             long long int colors     - Number of colors
Output     : tagStatus
Description: Change the color pallete
*/
tagStatus change_color_palette(tagImageProp *input_img, tagImageProp *output_img, long long int colors);


/*
Function   : photo_effect
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             long long int colors     - RGB Color palette levels
             int32_t window_size      - Window size
             tagEffects effects       - Effects to incorporate like OIL EFFECT, FILM EFFECT
Output     : tagStatus
Description: Bring in effects to the image.
*/
tagStatus photo_effect(tagImageProp *input_img, tagImageProp *output_img, long long int colors, int32_t window_size, tagEffects effects);


/*
Function   : contrast_manipulate
Input      : tagImageProp *input_img       - Input image
             tagImageProp *output_img      - Output Image
             tagHistEqualizeMethods method - Histogram equalization methods
Output     : None
Description: Bring in effects to the image.
*/
tagStatus contrast_manipulate(tagImageProp *input_img, tagImageProp *output_img, tagHistModifications method);

/*
Function   : contrast_manipulate
Input      : tagImageProp *input_img       - Input image
             tagImageProp *output_img      - Output Image
             int32_t window_size           - Window Size
             tagNoiseRemovalMethods method - Noise Removal methods
Output     : tagStatus
Description: Bring in effects to the image.
*/
tagStatus remove_image_noise(tagImageProp *input_img, tagImageProp *output_img, int32_t window_size, tagNoiseRemovalMethods method);

/*
Function   : print_psnr
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Print PSNR statistics
*/
void print_psnr(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : reflect
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Reflect the input image
*/
void reflect(tagImageProp *input_img, tagImageProp *output_img);

#endif // #ifndef __INTERFACE_H__

