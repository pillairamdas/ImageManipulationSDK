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

#ifndef __COLOR_CONVERSION_H__
#define __COLOR_CONVERSION_H__

#include "common.h"
#include "utils.h"

/*
Function   : fetch_color_levels
Input      : tagImageProp *input_img  - Input image
             int32_t **color_level    - Color levels for each channel.
             int32_t **boundary_level - Boundary levels to be populated.
             int32_t *len             - Color depth of the channel
             int32_t channel_count    - Channel index
Output     : None
Description: Identify the color levels and boundary levels based on the colors
*/
void fetch_color_levels(tagImageProp *input_img, int32_t **color_level, int32_t **boundary_level, int32_t *len, int32_t channel_count);

/*
Function   : convert_color
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             long long int colors     - Number of colors
Output     : tagStatus - Error Status for the function
Description: Change the color pallete
*/
tagStatus convert_color(tagImageProp *input_img, tagImageProp *output_img, long long int colors);

#endif // #ifndef __COLOR_CONVERSION_H__