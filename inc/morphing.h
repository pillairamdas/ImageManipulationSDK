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

#ifndef __MORPHING_H__
#define __MORPHING_H__

#include "common.h"
#include "utils.h"
#include "thresholding.h"

typedef struct list_shrink_sizes {

	int size;
	int count;
	struct list_shrink_sizes *next;

} list_shrink_sizes;


/*
Function   : create_shrink_list
Input      : int size  - Size of star
			 int count - Count of star
Output     : None
Description: Create the link list for the star sizes
*/
void create_shrink_list(int size, int count);

/*
Function   : add_to_shrink_list
Input      : int size  - Size of star
			 int count - Count of star
Output     : None
Description: Add element to the linked list
*/
void add_to_shrink_list(int size, int count);

/*
Function   : perform_stage1
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *X              - Binarized Input
             uint8_t *M              - Output Marked buffer
             type_morphing type      - Type of morphological operation
Output     : None
Description: Perform stage 1 of algorithm
*/
void perform_stage1(tagImageProp *input_img, uint8_t *X, uint8_t *M, type_morphing type);

/*
Function   : compute_M_compliment
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *M              - The marked buffer formed in stage 1
			 uint8_t *Mbar           - Compliment of marked buffer
Output     : None
Description: Compute the compliment of marked buffer
*/
void compute_M_compliment(tagImageProp *input_img, uint8_t *M, uint8_t *Mbar);

/*
Function   : perform_stage2
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *M              - The marked buffer formed in stage 1
			 uint8_t *prob_hit       - Stage 2 output
			 type_morphing type      - Type of morphological operation
Output     : None
Description: Perform Stage 2 of algorithm
*/
void perform_stage2(tagImageProp *input_img, uint8_t *M, uint8_t *prob_hit, type_morphing type);

/*
Function   : compute_output_binary(
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *prob_hit       - Output of Stage 2
			 uint8_t *Mbar           - Compliment of marked buffer
			 uint8_t *X              - binarized input
Output     : None
Description: Fill X with output using equation:
			 output = X AND (Mbar OR Prob_hit)
*/
void compute_output_binary(tagImageProp *input_img, uint8_t* prob_hit, uint8_t *Mbar,  uint8_t *X);

/*
Function   : print_star_statistics
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *X              - Binarized Input
Output     : None
Description: Print the statistics for stars
*/
void print_star_statistics(tagImageProp *input_img, uint8_t *X);

/*
Function   : perform_shrinking_thinning_skeletonize_binary
Input      : tagImageProp *input_img       - Input image object
             tagImageProp *output_img      - Output image object
             tagForegroundColor foreground - Foreground color
             type_morphing type            - Morphological type
Output     : None
Description: Perform the morphological operations
*/
void perform_shrinking_thinning_skeletonize_binary(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor foreground, type_morphing type);

#endif // #ifndef __MORPHING_H__
