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
#include "warping.h"
#include "stitching.h"
#include "thresholding.h"
#include "dithering.h"
#include "error_diffusion.h"
#include "color_halftoning.h"
#include "morphing.h"
#include "counting_game.h"

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

/*
Function   : warping
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
Output     : None
Description: Warp the square input image to circle
*/
void warping(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : dewarping
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
Output     : None
Description: Dewarp the circle input image to circle
*/
void dewarping(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : stitch
Input      : tagImageProp *left_img   - Left image
             tagImageProp *middle_img - Middle image
             tagImageProp *right_img  - Right image
Output     : tagImageProp* - Stitched output image
Description: Stitch the left middle and right image based on
             manual selection of key points.
*/
tagImageProp* stitch(tagImageProp *left_img, tagImageProp *middle_img, tagImageProp *right_img);

/*
Function   : threshold
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Perform thresholding based on bpp
*/
void threshold(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : dithering
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
             tagDithertingMethod method - Dithering method
Output     : None
Description: Perform dithering on the grayscale input image
*/
void dithering(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod method);

/*
Function   : error_diffuse
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
             tagErrorDiffusionMethod method - Error diffusion method
Output     : None
Description: Perform error diffusion on the input grayscale image.
*/
void error_diffuse(tagImageProp *input_img, tagImageProp *output_img, tagErrorDiffusionMethod method);

/*
Function   : color_halftone
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
             tagColorHalftoneMethod method - color halftoning method
Output     : None
Description: Perform color halftoning on the input image
*/
void color_halftone(tagImageProp *input_img, tagImageProp *output_img, tagColorHalftoneMethod method);


/*
Function   : shrinking
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Perform shrinking on the input image
*/
void shrinking(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor fg);

/*
Function   : thinning
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Perform thinning on the input image
*/
void thinning(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor fg);

/*
Function   : skeletonize
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Perform skeletonizing on the input image
*/
void skeletonize(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor fg);

/*
Function   : counting_game
Input      : tagImageProp *input_img  - Input image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Count the total and unique number of objects
*/
void counting_game(tagImageProp *input_img, tagForegroundColor fg);





#endif // #ifndef __INTERFACE_H__

