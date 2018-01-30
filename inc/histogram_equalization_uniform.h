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

#ifndef __HISTOGRAM_EQUALIZATION_UNIFORM_H__
#define __HISTOGRAM_EQUALIZATION_UNIFORM_H__

#include "common.h"
#include "utils.h"


/*
Function   : uniform_histogram_equalize 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Apply uniform histogram equalization on the image.
*/
void uniform_histogram_equalize(tagImageProp *input_img, tagImageProp *output_img);

#endif // #ifndef __HISTOGRAM_EQUALIZATION_UNIFORM_H__
