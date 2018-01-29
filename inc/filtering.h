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

#ifndef __FILTERING_H__
#define __FILTERING_H__

#include "common.h"
#include "utils.h"

/*
Function   : create_linear_kernel 
Input      : int *kernel - Kernel to be filled
             int window_size - Window size;
Output     : int filter_sum  - Normalization factor
Description: Create a linear kernel of window size
*/
int create_linear_kernel(int **kernel, int window_size);

/*
Function   : create_gaussian_kernel
Input      : int *kernel - Kernel to be filled
             int window_size - Window size;
Output     : int filter_sum  - Normalization factor
Description: Create a gaussian approximation kernel of window size
*/
int create_gaussian_kernel(int **kernel, int window_size);


/*
Function   : median_filtering
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int window_size          - Size of the window
Output     : ERROR_NONE on success
			 ERROR_WINDOW_SIZE on incorrect window size
Description: Perform median filtering on the input image
*/
void median_filtering(tagImageProp *input_img, tagImageProp *output_img, int window_size);


/*
Function   : gaussian_filtering 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             int window_size          - dimension of window
Output     : None
Description: Apply gaussian filter on the input image
*/
tagStatus apply_filter(tagImageProp *input_img, tagImageProp *output_img, int window_size, tagFilterType filter);

#endif // #ifndef __FILTERING_H__
