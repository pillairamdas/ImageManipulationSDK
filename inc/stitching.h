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

#ifndef __STITCHING_H__
#define __STITCHING_H__

#include "common.h"
#include "utils.h"




/*
Function   : stitch_image
Input      : tagImageProp *left_img   - Left image
             tagImageProp *middle_img - Middle Image
             tagImageProp *right_img  - Right Image
Output     : tagImageProp* - Output Image
Description: Stitch the left and right images on the middle image
			 based on manually selected key points and
			 inverse homography matrix computed using MATLAB.
*/
tagImageProp* stitch_image(tagImageProp *left_img, tagImageProp *middle_img, tagImageProp *right_img);

#endif // #ifndef __STITCHING_H__
