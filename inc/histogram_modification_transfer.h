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

#ifndef __HISTOGRAM_MODIFICATION_TRANSFER_H__
#define __HISTOGRAM_MODIFICATION_TRANSFER_H__

#include "common.h"
#include "utils.h"


/*
Function   : histogram_modify 
Input      : tagImageProp *input_img     - Input image
             tagImageProp *output_img    - Output Image
             tagHistModifications method - Operation: 1. Equalize histogram
             										  2. Modify histogram
Output     : None
Description: Modify histogram based on the operation
*/
void histogram_modify(tagImageProp *input_img, tagImageProp *output_img, tagHistModifications method);

#endif // #ifndef __HISTOGRAM_MODIFICATION_TRANSFER_H__