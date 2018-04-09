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

#ifndef __WARPING_H__
#define __WARPING_H__

#include "common.h"
#include "utils.h"

/*
Function   : find_distance_from_center
Input      : float center - Center pixel index of image
			 int x        - The horizontal index of image coordinate
			 int y        - The vertical index of image coordinate
Output     : float - Distance from center of point(x,y)
Description: Return the distance from the center of the
             image from any point (x, y)
*/
float find_distance_from_center(float center, int x, int y);

/*
Function   : warp_image
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output image
Output     : None
Description: Warp the square image to map the form of a circle
*/
void  warp_image(tagImageProp *input_img, tagImageProp *output_img);

/*
Function   : dewarp_image
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output image
Output     : None
Description: Dewarp the circle image to map the form of a square
*/
void  dewarp_image(tagImageProp *input_img, tagImageProp *output_img);

#endif //__WARPING_H__
