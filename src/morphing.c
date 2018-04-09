/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "morphing.h"

//#define DEBUG_DUMP
//#define DEBUG_PRINT

#define SIZE_ITERATION_RATIO 		1.58333

/*
 * Sample Output of algorithm for shrinking
 *
 * Iteration 1:
 * -------------
 *
 * M matrix is:
 * 0000000
 * 0111110
 * 0111110
 * 0000000
 *
 * M compliment matrix is:
 * 1111111
 * 1000001
 * 1000001
 * 1111111
 *
 * Probability matrix is:
 * 0000000
 * 0000000
 * 0011110
 * 0000000
 *
 * Output matrix is:
 * 0000000
 * 0000000
 * 0011110
 * 0000000
 *
 *
 * Iteration 2:
 * -------------
 *
 * M matrix is:
 * 0000000
 * 0000000
 * 0010010
 * 0000000
 *
 * M compliment matrix is:
 * 1111111
 * 1111111
 * 1101101
 * 1111111
 *
 * Probability matrix is:
 * 0000000
 * 0000000
 * 0000000
 * 0000000
 *
 * Output matrix is:
 * 0000000
 * 0000000
 * 0001100
 * 0000000
 *
 * Iteration 3:
 * -------------
 *
 * M matrix is:
 * 0000000
 * 0000000
 * 0001100
 * 0000000
 *
 * M compliment matrix is:
 * 1111111
 * 1111111
 * 1110011
 * 1111111
 *
 * Probability matrix is:
 * 0000000
 * 0000000
 * 0001000
 * 0000000
 *
 * Output matrix is:
 * 0000000
 * 0000000
 * 0001000
 * 0000000
 *
 * Iteration 4:
 * -------------
 *
 * M matrix is:
 * 0000000
 * 0000000
 * 0000000
 * 0000000
 *
 * M compliment matrix is:
 * 1111111
 * 1111111
 * 1111111
 * 1111111
 *
 * Probability matrix is:
 * 0000000
 * 0000000
 * 0000000
 * 0000000
 *
 * Output matrix is:
 * 0000000
 * 0000000
 * 0001000
 * 0000000
 *
 * Now the output matrix of two iterations is same so we can stop
 */

/* Link list pointers */
list_shrink_sizes *head = NULL;
list_shrink_sizes *curr = NULL;


#ifdef DEBUG_PRINT
	FILE *fp = NULL;
#endif


/*
Function   : create_shrink_list
Input      : int size  - Size of star
			 int count - Count of star
Output     : None
Description: Create the link list for the star sizes
*/
void create_shrink_list(int size, int count)
{
	list_shrink_sizes *list = (list_shrink_sizes *) malloc(sizeof(list_shrink_sizes));
	if(list != NULL) {
		list->size  = size;
		list->count = count;
		list->next  = NULL;
		head = curr = list;
	} else {
		LOG(ERROR, "Memory unavailable!\n");
	}
}


/*
Function   : add_to_shrink_list
Input      : int size  - Size of star
			 int count - Count of star
Output     : None
Description: Add element to the linked list
*/
void add_to_shrink_list(int size, int count)
{
	if(head == NULL) {
		create_shrink_list(size, count);
	} else {
		list_shrink_sizes *node = (list_shrink_sizes *) malloc(sizeof(list_shrink_sizes));
		if(node != NULL) {
			node->size  = size;
			node->count = count;
			node->next  = NULL;
			curr->next  = node;
			curr        = node;
		} else {
			LOG(ERROR, "Memory unavailable!\n");
		}

	}
}


/*
Function   : perform_stage1
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *X              - Binarized Input
             uint8_t *M              - Output Marked buffer
             type_morphing type      - Type of morphological operation
Output     : None
Description: Perform stage 1 of algorithm
*/
void perform_stage1(tagImageProp *input_img, uint8_t *X, uint8_t *M, type_morphing type)
{
	/* Temporary variables */
	int      iterator  = 0;
	uint8_t  outval    = 0;
	uint8_t *temp_buf  = NULL;
	int      i         = 0;

	/* Image coordinate */
	int      horiz     = 0;
	int      vert      = 0;

	/* Bond value of the window */
	int      bond      = 0;

	/* Number of masks available for the bond value
     * for mentioned morphological type
    */
	int      num_masks = 0;

	/* Window formed around the current pixel */
	uint8_t  window    = 0;

	/* Mask retrieved */
	uint8_t  mask      = 0;


#ifdef DEBUG_PRINT
	fprintf(fp, "\nM matrix is:");
#endif

	while(iterator < input_img->pixel_count) {

		horiz = iterator % input_img->width;
		vert = iterator / input_img->width;

		temp_buf = X + iterator;
		outval = 0;


		if(*temp_buf > 0) {

			/* Get the window surrounding the current pixel */
			window = get_window_at_current_location(input_img, X, vert, horiz);

			/* Check for window validity */
			if(window > 0) {

				/* Compute the bond from the window */
				bond = find_bond_mask(window);

				/* Check for window validity using bond */
				if(bond < 12) {

					/*
					 * Get the number of masks present for the bond for
					 * the mentioned morphological type.
                    */
					num_masks = get_num_elements_for_stage1_bond(bond, type);

					i = 0;
					while(i < num_masks) {

						/* Get the stage1 mask for the bond and type */
						mask = get_stage1_mask_for_bond(bond, i, type);

						/* Compare the window and mask to check if this is HIT */
						if(window == mask) {
							outval = 1;
							break;
						}
						++i;
					}
				}
			}

		} else {
			outval = 0;
		}

		*(M + iterator) = outval;
#ifdef DEBUG_PRINT
		if(iterator % input_img->width == 0) {
			fprintf(fp, "\n");
		}
		fprintf(fp, "%u ", *(M + iterator));
#endif
		++iterator;
	}

}


/*
Function   : compute_M_compliment
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *M              - The marked buffer formed in stage 1
			 uint8_t *Mbar           - Compliment of marked buffer
Output     : None
Description: Compute the compliment of marked buffer
*/
void compute_M_compliment(tagImageProp *input_img, uint8_t *M, uint8_t *Mbar)
{

	int      iterator = 0;
	uint8_t *temp_buf = NULL;

#ifdef DEBUG_PRINT
	fprintf(fp, "\nM compliment matrix is:");
#endif

	while(iterator < input_img->pixel_count) {

		temp_buf = M + iterator;
		*(Mbar + iterator) = (*temp_buf) ^ 0x1;


#ifdef DEBUG_PRINT
		if(iterator % input_img->width == 0) {
			fprintf(fp, "\n");
		}
		fprintf(fp, "%u ", *(Mbar + iterator));
#endif
		++iterator;
	}

}


/*
Function   : perform_stage2
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *M              - The marked buffer formed in stage 1
			 uint8_t *prob_hit       - Stage 2 output
			 type_morphing type      - Type of morphological operation
Output     : None
Description: Perform Stage 2 of algorithm
*/
void perform_stage2(tagImageProp *input_img, uint8_t *M, uint8_t *prob_hit, type_morphing type)
{

	/* Temporary variables */
	int      iterator  = 0;
	uint8_t  outval    = 0;
	uint8_t *temp_buf  = NULL;
	int      i         = 0;

	/* Image coordinate */
	int      horiz     = 0;
	int      vert      = 0;

	/* Bond value of the window */
	int      bond      = 0;

	/* Number of masks available for the bond value
     * for mentioned morphological type
    */
	int      num_masks = 0;

	/* Window formed around the current pixel */
	uint8_t  window    = 0;

	/* Mask retrieved */
	uint8_t  mask      = 0;


#ifdef DEBUG_PRINT
	fprintf(fp, "\nProbability matrix is:");
#endif

	while(iterator < input_img->pixel_count) {

		horiz = iterator % input_img->width;
		vert = iterator / input_img->width;

		temp_buf = M + iterator;

		outval = 0;
		if(*temp_buf > 0) {

			/* Get the window surrounding the current pixel */
			window    = get_window_at_current_location(input_img, M, vert, horiz);

			/* Check for window validity */
			if(window > 0) {

				/* Compute the bond from the window */
				bond = find_bond_mask(window);

				/* Check for window validity using bond */
				if(bond < 12) {

					/*
					 * Get the number of masks present for the bond for
					 * the mentioned morphological type.
	                */
					num_masks = get_num_elements_for_stage2_bond(bond, type);

					i = 0;
					while(i < num_masks) {

						/* Get the stage1 mask for the bond and type */
						mask = get_stage2_mask_for_bond(bond, i, type);

						/* Compare the window and mask to check if this is HIT */
						if(window == mask) {
							outval = 1;
							break;
						}
						++i;
					}
				}
			}
		} else {
			outval = 0;
		}


		*(prob_hit + iterator) = outval;
#ifdef DEBUG_PRINT
		if(iterator % input_img->width == 0) {
			fprintf(fp, "\n");
		}
		fprintf(fp, "%u ", *(prob_hit + iterator));
#endif

		++iterator;
	}
}


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
void compute_output_binary(tagImageProp *input_img, uint8_t* prob_hit, uint8_t *Mbar,  uint8_t *X)
{
	int iterator = 0;

	iterator = 0;

#ifdef DEBUG_PRINT
	fprintf(fp, "\nOutput matrix is:");
#endif

	while(iterator < input_img->pixel_count) {

		*(X + iterator) = *(X + iterator) & (*(Mbar + iterator) | *(prob_hit + iterator));
#ifdef DEBUG_PRINT
		if(iterator % input_img->width == 0) {
			fprintf(fp, "\n");
		}
		fprintf(fp, "%u ", *(X + iterator));
#endif

		++iterator;
	}
}


/*
Function   : print_star_statistics
Input      : tagImageProp *input_img - Input Image Object
             uint8_t *X              - Binarized Input
Output     : None
Description: Print the statistics for stars
*/
void print_star_statistics(tagImageProp *input_img, uint8_t *X)
{
	list_shrink_sizes *node = NULL;
	int total_star_count = 0;
	int size = 0;
	node = head;
	LOG(OUTPUT, "Size\t\tCount of stars\n")
	while(node != NULL) {
		size  = (int) ((float)node->size * 1.58333);
		LOG(OUTPUT, "%d\t\t%d\n", size, node->count);
		total_star_count += node->count;
		node = node->next;
	}

	LOG(OUTPUT, "\nTotal Star count: %d\n", total_star_count);
}


/*
Function   : perform_shrinking_thinning_skeletonize_binary
Input      : tagImageProp *input_img       - Input image object
             tagImageProp *output_img      - Output image object
             tagForegroundColor foreground - Foreground color
             type_morphing type            - Morphological type
Output     : None
Description: Perform the morphological operations
*/
void perform_shrinking_thinning_skeletonize_binary(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor foreground, type_morphing type)
{
	int           num_iterations  = 1;
	int           last_count      = 0;
	int           star_count      = 0;
	int           curr_count      = 0;
	int           threshold_limit = 102;

	tagImageProp *threshold_img   = NULL;
#ifdef DEBUG_DUMP
	char          filename[80]  = {};
#endif
#ifdef DEBUG_PRINT
	fp = fopen("dump.txt", "w+");
#endif

	/* Allocate memory for Temporary buffer */
	threshold_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(threshold_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	memcpy(threshold_img, input_img, sizeof(tagImageProp));

	threshold_img->buffer = (uint8_t *) malloc(input_img->size);
	if(threshold_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	uint8_t *X = (uint8_t *) malloc(input_img->size);
	if(X == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	uint8_t *M = (uint8_t *) malloc(input_img->size);
	if(M == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	uint8_t *Mbar = (uint8_t *) malloc(input_img->size);
	if(Mbar == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	uint8_t *prob_hit = (uint8_t *) malloc(input_img->size);
	if(prob_hit == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	uint8_t *prev_iteration = (uint8_t *) malloc(input_img->size);
	if(prev_iteration == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	if(type == SHRINKING) {
		threshold_limit = 127;
	} else if(type == THINNING) {
		threshold_limit = 103;
	} else {
		threshold_limit = 102;
	}

	/* Threshold the input image using a limit of 127 */
	threshold_gray_image(input_img, threshold_img, threshold_limit);

	/* Binarize the thresholded image */
	binarise_input(threshold_img, X, foreground);

	/* Get the shrinked count on the binarized data */
	star_count = get_shrinked_dot_count(input_img, X);

	curr_count = star_count - last_count;
	last_count = star_count;

	LOG(DEBUG, "Num Iteration %d total %d size %d\n", num_iterations, star_count, curr_count);

	/* If there are some new stars shrinked completely, add to linked list */
	if(curr_count > 0) {
		add_to_shrink_list(num_iterations, curr_count);
	}

	while(1) {

#ifdef DEBUG_PRINT
	    fprintf(fp, "\nIteration: %d\n", num_iterations);
#endif

		/* Perform stage 1 of algorithm */
		perform_stage1(input_img, X, M, type);

		/* Compute M compliment */
		compute_M_compliment(input_img, M, Mbar);

		/* Perform Stage 2 of algorithm */
		perform_stage2(input_img, M, prob_hit, type);

		/* Get the output of current iteration */
		compute_output_binary(input_img, prob_hit, Mbar, X);

		++num_iterations;

		/* Compare the output of this iteration with output of previous iteration */
		if(memcmp(prev_iteration, X, input_img->size) == 0) {
			break;
		}


		/* Get the shrinked count on the binarized data */
		star_count = get_shrinked_dot_count(input_img, X);

		curr_count = star_count - last_count;
		last_count = star_count;

		LOG(DEBUG, "Num Iteration %d total %d size %d\n", num_iterations, star_count, curr_count);

		/* If there are some new stars shrinked completely, add to linked list */
		if(curr_count > 0) {
			add_to_shrink_list(num_iterations, curr_count);
		}

#ifdef DEBUG_DUMP
		if(type == SHRINKING) {
			LOG(DEBUG, "Num Iteration %d total %d size %d\n", num_iterations, star_count, curr_count);
			sprintf(filename, "shrinking_%d.gray", num_iterations);
		}
		else if(type == THINNING)
			sprintf(filename, "thinning_%d.gray", num_iterations);
		else
			sprintf(filename, "skeletonizing_%d.gray", num_iterations);

		create_output_image(input_img, output_img, X, foreground);

		dump_to_file(output_img->buffer, output_img->size, filename);

#endif



		memcpy(prev_iteration, X, input_img->size);

	}

	/* Create the output image with respect to the foreground color */
	create_output_image(input_img, output_img, X, foreground);

	if(type == SHRINKING) {
		print_star_statistics(input_img, X);
		LOG(OUTPUT, "\nTotal Iterations: %d\n", num_iterations - 1);
	}



#ifdef DEBUG_PRINT
	fclose(fp);
#endif

FREE_MEM:

	if(X != NULL) {
		free(X);
		X = NULL;
	}
	if(M != NULL) {
		free(M);
		M = NULL;
	}
	if(Mbar != NULL) {
		free(Mbar);
		Mbar = NULL;
	}
	if(prob_hit != NULL) {
		free(prob_hit);
		prob_hit = NULL;
	}
	if(prev_iteration != NULL) {
		free(prev_iteration);
		prev_iteration = NULL;
	}

	if(threshold_img != NULL) {
		if(threshold_img->buffer != NULL) {
			free(threshold_img->buffer);
			threshold_img->buffer = NULL;
		}
		free(threshold_img);
		threshold_img = NULL;
	}

}
