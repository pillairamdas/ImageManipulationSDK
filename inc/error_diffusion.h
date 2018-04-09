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

#ifndef __ERROR_DIFFUSION_H__
#define __ERROR_DIFFUSION_H__

#include "common.h"
#include "utils.h"


/*
Function   : floyds_error_diffusion
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Floyd-Steinberg's error diffusion implementation
*/
void floyds_error_diffusion(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : jjn_error_diffusion
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Jarvis, Judice, and Ninke (JJN) error diffusion implementation
*/
void jjn_error_diffusion(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : stucki_error_diffusion
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Stucki error diffusion implementation
*/
void stucki_error_diffusion(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : custom_error_diffusion
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Custom error diffusion implementation
*/
void custom_error_diffusion(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : error_diffusion_binary
Input      : tagImageProp *input_img        - Input image
             tagImageProp *output_img       - Output Image
             tagErrorDiffusionMethod method - Error diffusion method
Output     : None
Description: Error diffusion binary framework function
*/
void error_diffusion_binary(tagImageProp *input_img, tagImageProp *output_img, tagErrorDiffusionMethod method);

#endif //#ifndef __ERROR_DIFFUSION_H__
