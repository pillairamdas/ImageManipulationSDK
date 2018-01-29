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

#ifndef __OIL_EFFECT_H__
#define __OIL_EFFECT_H__

#include "common.h"
#include "utils.h"
#include "color_conversion.h"


/*
Function   : oil_effect
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int window_size          - Window size
Output     : tagStatus - Error Status for the function
Description: Get oil effect on the input image
*/
tagStatus oil_effect(tagImageProp *input_img, tagImageProp *output_img, int window_size);

#endif // #ifndef __OIL_EFFECT_H__