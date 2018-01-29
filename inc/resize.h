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

#ifndef __RESIZE_H__
#define __RESIZE_H__

#include "common.h"
#include "utils.h"

/*
Function   : bilinear_interpolation
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Resize the input image
*/
void bilinear_interpolation(tagImageProp *input_img, tagImageProp *output_img);

#endif // #ifndef __RESIZE_H__