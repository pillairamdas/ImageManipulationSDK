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
Input      : int32_t a, b, c - Three integers for comparison
Output     : int32_t - Max of three input integers
Description: Find maximum of 3 integers
*/
int32_t max_of_3(int32_t a, int32_t b, int32_t c);

/*
Function   : min_of_3
Input      : int32_t a, b, c - Three integers for comparison
Output     : int32_t - Min of three input integers
Description: Find minimum of 3 integers
*/
int32_t min_of_3(int32_t a, int32_t b, int32_t c);

/*
Function   : dump_to_file 
Input      : uint8_t *buf - Buffer to write
			 int32_t size     - Size of buffer
			 char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Dump contents of buffer to file
*/
int32_t dump_to_file(uint8_t* buf, int32_t size, char *str);

/*
Function   : read_from_file
Input      : uint8_t *buf - Buffer to read to 
		     int32_t size     - Size of buffer
		     char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Read contents of file to the buffer
*/
int32_t read_from_file(uint8_t *buf, int32_t size, char *str);


/*
Function   : get_input_pixel
Input      : uint8_t *input_img - Input Image 
             uint8_t *buffer 	- Output buffer to fill in pixel
             int32_t i 				- veritical axis location
             int32_t j 				- horizontal axis location
             int32_t width 			- width of the input image
             int32_t height 		      - height of the input image
             int32_t bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
void get_input_pixel(tagImageProp *input_img, uint8_t *buffer, int32_t i, int32_t j);


/*
Function   : get_input_pixel_ptr
Input      : uint8_t *input_img - Input Image 
             int32_t i 				- veritical axis location
             int32_t j 				- horizontal axis location
             int32_t width 			- width of the input image
             int32_t height 		      - height of the input image
             int32_t bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
uint8_t* get_input_pixel_ptr(tagImageProp *input_img, int32_t i, int32_t j);

/*
Function   : int_cmp
Input      : const void *a, *b - Comparison parameters
Output     : Comparison value
Description: qsort int32_t comparison function
*/
int32_t int_cmp(const void *a, const void *b);


/*
Function   : index_max
Input      : int32_t *arr - Input array
	       int32_t size - Input array size
Output     : Index of max value
Description: Find the index of max element in the array
*/
int32_t index_max(int32_t *arr, int32_t size);


/*
Function   : get_median
Input      : int32_t *arr - Input array
             int32_t size - Input array size
Output     : Median of the array
Description: Find the median of array
*/
int32_t get_median(int32_t *arr, int32_t size);


/*
Function   : get_PSNR
Input      : tagImageProp *input_img    - Input image
             tagImageProp *filtered_img - Filtered image
Output     : None
Description: Print the PSNR statistics
*/
void get_PSNR(tagImageProp *input_img, tagImageProp *filtered_img);


#endif // #ifndef __UTILS_H__