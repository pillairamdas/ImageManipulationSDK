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

#ifndef __DITHERING_H__
#define __DITHERING_H__

#include "common.h"
#include "utils.h"
#include "thresholding.h"


/*
Function   : fixed_thresholding
Input      : tagImageProp *input_img   - Input image
             tagImageProp *output_img  - Output image
             tagDithertingMethod index - Index specifying the dithering method used
Output     : None
Description: Perform fixed thresholding to create a dithering impact.
*/
void fixed_thresholding(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod index);

/*
Function   : random_thresholding
Input      : tagImageProp *input_img   - Input image
             tagImageProp *output_img  - Output image
             tagDithertingMethod index - Index specifying the dithering method used
Output     : None
Description: Perform random thresholding to create a dithering impact.
*/
void random_thresholding(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod index);

/*
Function   : bayer_matrix_dithering
Input      : tagImageProp *input_img   - Input image
             tagImageProp *output_img  - Output image
             tagDithertingMethod index - Index specifying the dithering method used
Output     : None
Description: Perform bayer index matrix dithering.
*/
void bayer_matrix_dithering(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod index);

#endif // #ifndef __DITHERING_H__
