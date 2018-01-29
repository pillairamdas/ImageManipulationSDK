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


#ifndef __REFLECT_H__
#define __REFLECT_H__

#include "common.h"
#include "utils.h"



/*
Function   : reflect_to_side
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Perform reflection to the side on input image
*/
void reflect_to_side(tagImageProp *input_img, tagImageProp *output_img);

#endif // #ifndef __REFLECT_H__