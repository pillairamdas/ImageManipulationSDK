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

extern uint8_t stage2_mask_skeletonizing[][40];
extern uint8_t stage2_mask_shrinking[][39];
extern uint8_t stage1_mask_bond[][10];


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
Function   : get_image_pixel_ptr
Input      : uint8_t *input_img - Input Image 
             int32_t i 				- veritical axis location
             int32_t j 				- horizontal axis location
             int32_t width 			- width of the input image
             int32_t height 		      - height of the input image
             int32_t bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
uint8_t* get_image_pixel_ptr(tagImageProp *input_img, int32_t i, int32_t j);

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



/*
Function   : print_image_info
Input      : tagImageProp *input_img - Input image
Output     : None
Description: Print the properties of image
*/
void print_image_info(tagImageProp *input_img);

/*
Function   : print_object_info
Input      : tagObject *obj - Input object
Output     : None
Description: Print the properties of the object
*/
void print_object_info(tagObject *obj);

/*
Function   : get_num_elements_for_stage1_bond
Input      : int bond           - bond to query
                   type_morphing type - type of morphing
Output     : int - Number of elements
Description: Return the number of elements for the bond of
             this particular morphing type in Stage 1
*/
int get_num_elements_for_stage1_bond(int bond, type_morphing type);

/*
Function   : get_num_elements_for_stage2_bond
Input      : int bond           - bond to query
                   type_morphing type - type of morphing
Output     : int - Number of elements
Description: Return the number of elements for the bond of
             this particular morphing type in Stage 2
*/
int get_num_elements_for_stage2_bond(int bond, type_morphing type);

/*
Function   : get_stage1_mask_for_bond
Input      : int bond           - bond to query
                   int index          - index required
                   type_morphing type - type of morphing
Output     : Mask as a byte
Description: Return the Stage 1 mask fetched for the particular
             bond and morphing type addressed by its index.
*/
uint8_t get_stage1_mask_for_bond(int bond, int index, type_morphing type);

/*
Function   : get_stage2_mask_for_bond
Input      : int bond           - bond to query
                   int index          - index required
                   type_morphing type - type of morphing
Output     : Mask as a byte
Description: Return the Stage 2 mask fetched for the particular
             bond and morphing type addressed by its index.
*/
uint8_t get_stage2_mask_for_bond(int bond, int index, type_morphing type);

/*
Function   : print_mask
Input      : uint8_t mask
Output     : None
Description: Print the mask in the conventional 3x3 format
*/
void print_mask(uint8_t mask);

/*
Function   : find_bond_mask
Input      : uint8_t mask
Output     : int - bond value
Description: Compute the bond value of the mask
*/
int find_bond_mask(uint8_t mask);

/*
Function   : print_mask_table
Input      : None
Output     : None
Description: Print the masks for all morphing type for both stages
*/
void print_mask_table();

/*
Function   : create_output_image
Input      : tagImageProp *input_img  - Input Image
             tagImageProp *output_img - Output Image
             uint8_t      *X          - Binarized Image
             tagForegroundColor fg    - Foreground color
Output     : None
Description: Create the output image from the binarized image
*/
void create_output_image(tagImageProp *input_img, tagImageProp *output_img, uint8_t *X, tagForegroundColor fg);

/*
Function   : binarise_input
Input      : tagImageProp *input_img - Input image
             uint8_t      *X         - Binarized image
             tagForegroundColor fg   - foreground image
Output     : None
Description: Binarize the input image to contain 1
             for foreground and 0 for background.
*/
void binarise_input(tagImageProp *input_img, uint8_t *X, tagForegroundColor fg);

/*
Function   : get_shrinked_dot_count
Input      : tagImageProp *input_img - Input Image
             uint8_t      *buffer    - Input image pixel
Output     : int  - Total Count
Description: Return the total number of single foreground
             pixels surrounded by background pixels.
*/
int get_shrinked_dot_count(tagImageProp *input_img, uint8_t *buffer);

/*
Function   : get_window_at_current_location
Input      : tagImageProp *input_img - Input Image object
             uint8_t *buffer         - Current Pixel
             int vert                - Vertical Coordinate of the pixel
             int horiz               - Horizontal Coordinate of the pixel
Output     : uint8_t - Window
Description: Return the window formed at the current location arranged in a byte
             X3    X2   X1
             X4  X  X0
             X5  X6 X7
             with X0 as the LSB and X7 as the MSB; X being the current pixel
*/
uint8_t get_window_at_current_location(tagImageProp *input_img, uint8_t *buffer, int vert, int horiz);

/*
Function   : matrix_multiply_float
Input      : float **A    - Matrix A
                   int   *dim_A - Dimensions of Matrix A
                   float **B    - Matrix B
                   int   *dim_B - Dimensions of Matrix B
Output     : float** - Pointer to output matrix
Description: Return the matrix multiplied [A]x[B] output matrix.
             Memory for output matrix is allocated in the function itself,
             and a pointer to it is returned.
*/
float** matrix_multiply_float(float **A, int *dim_A, float **B, int *dim_B);

/*
Function   : print_matrix_float
Input      : float **matrix - Matrix
                   int   *dim     - Dimension of matrix
Output     : None
Description: Print the matrix in conventional format
*/
void print_matrix_float(float **matrix, int *dim);

#endif // #ifndef __UTILS_H__