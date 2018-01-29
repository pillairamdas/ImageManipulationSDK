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

#ifndef __UTILS_H__
#define __UTILS_H__

#include "common.h"


/*
Function   : max_of_3
Input      : int a, b, c - Three integers for comparison
Output     : int - Max of three input integers
Description: Find maximum of 3 integers
*/
int max_of_3(int a, int b, int c);

/*
Function   : min_of_3
Input      : int a, b, c - Three integers for comparison
Output     : int - Min of three input integers
Description: Find minimum of 3 integers
*/
int min_of_3(int a, int b, int c);

/*
Function   : dump_to_file 
Input      : uint8_t *buf - Buffer to write
			 int size     - Size of buffer
			 char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Dump contents of buffer to file
*/
int dump_to_file(uint8_t* buf, int size, char *str);

/*
Function   : read_from_file
Input      : uint8_t *buf - Buffer to read to 
		     int size     - Size of buffer
		     char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Read contents of file to the buffer
*/
int read_from_file(uint8_t *buf, int size, char *str);


/*
Function   : get_input_pixel
Input      : uint8_t *input_img - Input Image 
             uint8_t *buffer 	- Output buffer to fill in pixel
             int i 				- veritical axis location
             int j 				- horizontal axis location
             int width 			- width of the input image
             int height 		- height of the input image
             int bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
void get_input_pixel(tagImageProp *input_img, uint8_t *buffer, int i, int j);


/*
Function   : get_input_pixel_ptr
Input      : uint8_t *input_img - Input Image 
             int i 				- veritical axis location
             int j 				- horizontal axis location
             int width 			- width of the input image
             int height 		- height of the input image
             int bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
uint8_t* get_input_pixel_ptr(tagImageProp *input_img, int i, int j);

/*
Function   : int_cmp
Input      : const void *a, *b - Comparison parameters
Output     : Comparison value
Description: qsort int comparison function
*/
int int_cmp(const void *a, const void *b);


/*
Function   : index_max
Input      : int *arr - Input array
			 int size - Input array size
Output     : Index of max value
Description: Find the index of max element in the array
*/
int index_max(int *arr, int size);


/*
Function   : get_median
Input      : int *arr - Input array
                   int size - Input array size
Output     : Median of the array
Description: Find the median of array
*/
int get_median(int *arr, int size);

void get_PSNR(tagImageProp *input_img, tagImageProp *filtered_img);


#endif // #ifndef __UTILS_H__