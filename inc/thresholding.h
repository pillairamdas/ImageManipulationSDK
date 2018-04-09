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

#ifndef __THRESHOLDING_H__
#define __THRESHOLDING_H__

#include "common.h"
#include "utils.h"


/*
Function   : threshold_gray_image
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int32_t threshold_limit  - Threshold limit
Output     : None
Description: Perform thresholding on the input gray scale image
*/
void threshold_gray_image(tagImageProp *input_img, tagImageProp *output_img, int32_t threshold_limit);

#endif //__THRESHOLDING_H__
