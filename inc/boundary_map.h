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

#ifndef __BOUNDARY_MAP_H__
#define __BOUNDARY_MAP_H__

#include "common.h"
#include "utils.h"


/*
Function   : find_next_obj_pixel
Input      : tagImageProp *input_img - Input image object
             uint8_t      *workboard - Workboard buffer
             int          *out_coord - Input and output cordinates of next pixel
             int           pointer   - Start location of pixel search
Output     : Location of next pixel
Description: Find the next pixel location by traversing the boundary
*/
int find_next_obj_pixel(tagImageProp *input_img, uint8_t *workboard, int *out_coord, int pointer);

/*
Function   : boundary_map
Input      : tagImageProp *input_img - Input Image
             uint8_t      *workboard - Workboard buffer
             tagObject    *obj       -
             int           vert
             int           horiz
Output     : None
Description: Find the objects by traversing the workboard
*/
void boundary_map(tagImageProp *input_img, uint8_t *workboard, tagObject *obj, int vert, int horiz);

#endif // #ifndef __BOUNDARY_MAP_H__
