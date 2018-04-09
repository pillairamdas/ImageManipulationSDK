#include "utils.h"

/*
Function   : max_of_3
Input      : int32_t a, b, c - Three integers for comparison
Output     : int32_t - Max of three input integers
Description: Find maximum of 3 integers
*/
int32_t max_of_3(int32_t a, int32_t b, int32_t c)
{
	if(a >= b && a >= c)
		return a;
	if(b >= a && b >= c)
		return b;
	return c;
}


/*
Function   : min_of_3
Input      : int32_t a, b, c - Three integers for comparison
Output     : int32_t - Min of three input integers
Description: Find minimum of 3 integers
*/
int32_t min_of_3(int32_t a, int32_t b, int32_t c)
{
	if(a <= b && a <= c)
		return a;
	if(b <= a && b <= c)
		return b;
	return c;
}


/*
Function   : dump_to_file 
Input      : uint8_t *buf - Buffer to write
			 int32_t size     - Size of buffer
			 char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Dump contents of buffer to file
*/
int32_t dump_to_file(uint8_t* buf, int32_t size, char *str)
{
	int32_t out_fd = -1;
	int32_t status = 0;

	/* Open the file for writing */
	out_fd = open(str, O_CREAT|O_WRONLY, 0666);
	if(out_fd < 0) {
		LOG(ERROR, "Unable to open output file %s with error: %s\n", str, strerror(errno));
		exit(1);
	}	

	/* Write to file */
	if((status = write(out_fd, buf, size)) >= 0) {
		return 0;
	} else {
		LOG(ERROR, "Write Error for %s with error: %s\n", str, strerror(errno));
		return -1;
	}

	close(out_fd);
}


/*
Function   : read_from_file
Input      : uint8_t *buf - Buffer to read to 
		     int32_t size     - Size of buffer
		     char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Read contents of file to the buffer
*/
int32_t read_from_file(uint8_t *buf, int32_t size, char *str)
{
	int32_t in_fd  = -1;
	int32_t status = 0;

	/* Open the file for reading */ 
	in_fd = open(str, O_RDONLY);
	if(in_fd < 0) {
		LOG(ERROR, "Unable to open input file %s with error: %s\n", str,strerror(errno));
		exit(1);
	}

	/* Read file to buffer */
	if((status = read(in_fd, buf, size)) >= 0) {
		return 0;
	} else {
		LOG(ERROR, "Read Error for %s with error: %s\n", str, strerror(errno));
		return -1;	
	}

	close(in_fd);
}


/*
Function   : get_input_pixel
Input      : tagImageProp *input_img - Input Image 
             uint8_t *buffer 	- Output buffer to fill in pixel
             int32_t i 				- veritical axis location
             int32_t j 				- horizontal axis location
             int32_t width 			- width of the input image
             int32_t height 		- height of the input image
             int32_t bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
void get_input_pixel(tagImageProp *input_img, uint8_t *buffer, int32_t i, int32_t j) {
	uint8_t *temp = input_img->buffer + ((i * input_img->width) + j)*input_img->bpp;
	memcpy(buffer, temp, input_img->bpp * sizeof(uint8_t));
}


/*
Function   : get_image_pixel_ptr
Input      : tagImageProp *input_img - Input Image 
             int32_t i 				- veritical axis location
             int32_t j 				- horizontal axis location
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
uint8_t* get_image_pixel_ptr(tagImageProp *input_img, int32_t i, int32_t j) {
	uint8_t *temp = input_img->buffer + ((i * input_img->width) + j)*input_img->bpp;
	return temp;
}


/*
Function   : int_cmp
Input      : const void *a, *b - Comparison parameters
Output     : Comparison value
Description: qsort int32_t comparison function
*/
int32_t int_cmp(const void *a, const void *b) 
{ 
    const int32_t *ia = (const int32_t *)a; // casting pointer types 
    const int32_t *ib = (const int32_t *)b;

    return *ia  - *ib; 
	/* integer comparison: returns negative if b > a 
	and positive if a > b */ 
} 


/*
Function   : index_max
Input      : int32_t *arr - Input array
			 int32_t size - Input array size
Output     : Index of max value
Description: Find the index of max element in the array
*/
int32_t index_max(int32_t *arr, int32_t size) {
	
	int32_t i       = 0;
	int32_t max_pos = 0;
	int32_t max     = 0;

	for(i = 0; i<size; i++) {
		if(max < arr[i]) {
			max = arr[i];
			max_pos = i;
		}
	}

	return max_pos;
}


/*
Function   : get_median
Input      : int32_t *arr - Input array
			 int32_t size - Input array size
Output     : Median of the array
Description: Find the median of array
*/
int32_t get_median(int32_t *arr, int32_t size)
{
	qsort(arr, size, sizeof(int32_t), int_cmp);
	return arr[size/2];
}


/*
Function   : get_PSNR
Input      : tagImageProp *input_img - Input image
			 tagImageProp *filtered_img - Filtered image
Output     : None
Description: Print the PSNR statistics
*/
void get_PSNR(tagImageProp *input_img, tagImageProp *filtered_img)
{
	int32_t        channel_count  = 0;
	int32_t        iterator       = 0;
	long long int  noise          = 0;
	float          mse            = 0.0;
	long long int  signal         = 0;
	float          snr            = 0.0;
	float          psnr_255       = 0.0;
	float          psnr_max       = 0.0;
	uint8_t       *inbuffer       = NULL;
	uint8_t       *outbuffer      = NULL;
	int32_t        max_intensity  = 0;


	while(channel_count < input_img->bpp) {

		max_intensity = 0;
		noise = 0;
		signal = 0;


		iterator = 0;
		while(iterator < input_img->pixel_count) {

			inbuffer  = input_img->buffer + iterator * input_img->bpp + channel_count;
			outbuffer = filtered_img->buffer + iterator * filtered_img->bpp + channel_count;
			
			signal += pow(*(inbuffer), 2);
			noise  += pow((*outbuffer - *inbuffer), 2);

			if(max_intensity < *inbuffer) {
				max_intensity = *inbuffer;
			}

			++iterator;
		}

		mse = noise / input_img->pixel_count;
		snr = 10.0 * log10(signal / (float)noise);
		psnr_255 = 10.0 * log10(pow(255,2) / mse);
		psnr_max = 10.0 * log10(pow(max_intensity,2) / mse);

		LOG(OUTPUT, "\n\n*************************************************\n");
		switch(channel_count) {
			case 0: { LOG(OUTPUT, "The statistics for RED channel is: \n"); } break;
			case 1: { LOG(OUTPUT, "The statistics for GREEN channel is: \n"); } break;
			case 2: { LOG(OUTPUT, "The statistics for BLUE channel is: \n"); } break;
			default: { LOG(OUTPUT, "Incorrect Channel count \n"); } break;
		}



		LOG(OUTPUT, "\n");
		LOG(OUTPUT, "Noise                                  : %lld\n", noise);
		LOG(OUTPUT, "Signal to Noise Ratio                  : %0.2f dB\n", snr);
		LOG(OUTPUT, "Mean Square Error                      : %0.2f\n", mse);
		LOG(OUTPUT, "Peak-Signal to Noise Ratio (Max = 255) : %0.2f dB\n", psnr_255);
		LOG(OUTPUT, "Peak-Signal to Noise Ratio (Max)       : %0.2f dB\n", psnr_max);
		LOG(OUTPUT, "\n*************************************************\n");

		++channel_count;
	}
}


/*
Function   : print_image_info
Input      : tagImageProp *input_img - Input image
Output     : None
Description: Print the properties of image
*/
void print_image_info(tagImageProp *input_img)
{
	LOG(OUTPUT, "Image: %dx%d bpp: %d\n", input_img->width, input_img->height, input_img->bpp);
}


/*
Function   : print_object_info
Input      : tagObject *obj - Input object
Output     : None
Description: Print the properties of the object
*/
void print_object_info(tagObject *obj)
{
	LOG(OUTPUT, "Obj: %dx%d T: %d B: %d L: %d R: %d\n", obj->width, obj->height, obj->top_index, obj->bottom_index, obj->left_index, obj->right_index);
}


/*
Function   : get_num_elements_for_stage1_bond
Input      : int bond           - bond to query
			 type_morphing type - type of morphing
Output     : int - Number of elements
Description: Return the number of elements for the bond of
             this particular morphing type in Stage 1
*/
int get_num_elements_for_stage1_bond(int bond, type_morphing type)
{
	int num = 0;

	switch(bond) {
		case 1:
		case 2: {
			if(type == SHRINKING) {
				num = 4;
			}
		} break;
		case 3: {
			if(type == SHRINKING) {
				num = 8;
			}
		} break;
		case 4: {
			num = 8;
			if(type == SHRINKING) {
				num = 4;
			}
		} break;
		case 5: {
			num = 8;
			if(type == SKELETONIZING) {
				num = 0;
			}
		} break;
		case 6: {
			num = 10;
			if(type == SKELETONIZING) {
				num = 8;
			}
		} break;
		case 7:
		case 8:
		case 10: {
			num = 4;
		} break;
		case 9: {
			num = 8;
		} break;
		case 11: {
			if(type == SKELETONIZING) {
				num = 4;
			}
		} break;
		default: {
			LOG(ERROR, "Stage1: Unsupported bond value %d\n", bond);
		} break;
	}

	return num;
}


/*
Function   : get_num_elements_for_stage2_bond
Input      : int bond           - bond to query
			 type_morphing type - type of morphing
Output     : int - Number of elements
Description: Return the number of elements for the bond of
             this particular morphing type in Stage 2
*/
int get_num_elements_for_stage2_bond(int bond, type_morphing type)
{
	int num = 0;

	if((type == SHRINKING) || (type == THINNING)) {
		switch(bond) {
			case 1: num = 2; break;
			case 2: num = 2; break;
			case 3: num = 12; break;
			case 4: num = 13; break;
			case 5: num = 25; break;
			case 6: num = 26; break;
			case 7: num = 39; break;
			case 8: num = 22; break;
			case 9: num = 17; break;
			case 10: num = 9; break;
			case 11: num = 4; break;
			default: {
				LOG(ERROR, "Unsupported bond value\n");
			} break;
		}
	} else { // SKELETONIZING
		switch(bond) {
			case 1: num = 4; break;
			case 2: num = 4; break;
			case 3: num = 4; break;
			case 4: num = 9; break;
			case 5: num = 22; break;
			case 6: num = 26; break;
			case 7: num = 40; break;
			case 8: num = 31; break;
			case 9: num = 20; break;
			case 10: num = 10; break;
			case 11: num = 4; break;
			default: {
				LOG(ERROR, "Stage2: Unsupported bond value: %d\n", bond);
			} break;
		}
	}

	return num;
}


/*
Function   : get_stage1_mask_for_bond
Input      : int bond           - bond to query
			 int index          - index required
			 type_morphing type - type of morphing
Output     : Mask as a byte
Description: Return the Stage 1 mask fetched for the particular
             bond and morphing type addressed by its index.
*/
uint8_t get_stage1_mask_for_bond(int bond, int index, type_morphing type)
{
	uint8_t mask = 0x0;

	if((bond == 4) && (type == SHRINKING)) {
		index += 4;
	}
	if((bond == 6) && (type == SKELETONIZING)) {
		index += 2;
	}

	mask = stage1_mask_bond[bond-1][index];

	return mask;
}


/*
Function   : get_stage2_mask_for_bond
Input      : int bond           - bond to query
			 int index          - index required
			 type_morphing type - type of morphing
Output     : Mask as a byte
Description: Return the Stage 2 mask fetched for the particular
             bond and morphing type addressed by its index.
*/
uint8_t get_stage2_mask_for_bond(int bond, int index, type_morphing type)
{
	uint8_t mask = 0x0;

	if((type == SHRINKING) || (type == THINNING))
		mask = stage2_mask_shrinking[bond-1][index];
	else
		mask = stage2_mask_skeletonizing[bond-1][index];

	return mask;
}


/*
Function   : print_mask
Input      : uint8_t mask
Output     : None
Description: Print the mask in the conventional 3x3 format
*/
void print_mask(uint8_t mask)
{
	LOG(OUTPUT, "\n");
	LOG(OUTPUT, "%u %u %u\n", ((mask >> 3) & 0x1), ((mask >> 2) & 0x1), ((mask >> 1) & 0x1));
	LOG(OUTPUT, "%u %u %u\n", ((mask >> 4) & 0x1), (0x1), ((mask >> 0) & 0x1));
	LOG(OUTPUT, "%u %u %u\n", ((mask >> 5) & 0x1), ((mask >> 6) & 0x1), ((mask >> 7) & 0x1));
}


/*
Function   : find_bond_mask
Input      : uint8_t mask
Output     : int - bond value
Description: Compute the bond value of the mask
*/
int find_bond_mask(uint8_t mask)
{

	int iterator = 0;
	int bond     = 0;


	while(iterator < 8) {
		bond += ((mask >> iterator) & 0x1)*(2 - iterator%2);
		++iterator;
	}
	return bond;
}


/*
Function   : print_mask_table
Input      : None
Output     : None
Description: Print the masks for all morphing type for both stages
*/
void print_mask_table()
{
	int     bond     = 1;
	int     pbond    = 0;
	int     iterator = 0;
	uint8_t mask     = 0x0;
	int     num      = 0;

	/* Printing mask table for SHRINKING stage 1 */
	type_morphing type = SHRINKING;
	LOG(OUTPUT, "Printing SHRINKING masks:\n");
	while(bond < 12) {

		/* Get the number of masks for the bond */
		num = get_num_elements_for_stage1_bond(bond, type);
		if(num > 0) {

			LOG(OUTPUT, "\n\nBond: %d\n", bond);
			iterator = 0;
			while(iterator < num) {
				/* Get each mask individually */
				mask = get_stage1_mask_for_bond(bond, iterator, type);
				/* Print the mask for convinient checking */
				print_mask(mask);
				++iterator;
			}

			/* Get the bond from mask for verification */
			pbond = find_bond_mask(mask);
			LOG(OUTPUT, "Printed bond: %d\n", pbond);
		}
		++bond;
	}
	LOG(OUTPUT, "\n");

	/* Printing mask table for THINNING stage 1 */
	type = THINNING;
	LOG(OUTPUT, "Printing THINNING masks:\n");
	bond = 1;
	while(bond < 12) {

		/* Get the number of masks for the bond */
		num = get_num_elements_for_stage1_bond(bond, type);

		if(num > 0) {

			LOG(OUTPUT, "\n\nBond: %d\n", bond);
			iterator = 0;
			while(iterator < num) {
				/* Get each mask individually */
				mask = get_stage1_mask_for_bond(bond, iterator, type);
				/* Print the mask for convinient checking */
				print_mask(mask);
				++iterator;
			}

			/* Get the bond from mask for verification */
			pbond = find_bond_mask(mask);
			LOG(OUTPUT, "Printed bond: %d\n", pbond);
		}
		++bond;
	}
	LOG(OUTPUT, "\n");

	/* Printing mask table for SKELETONIZING stage 1 */
	type = SKELETONIZING;
	LOG(OUTPUT, "Printing SKELETONIZING masks:\n");
	bond = 1;

	while(bond < 12) {

		/* Get the number of masks for the bond */
		num = get_num_elements_for_stage1_bond(bond, type);
		if(num > 0) {
			LOG(OUTPUT, "\n\nBond: %d\n", bond);
			iterator = 0;
			while(iterator < num) {
				/* Get each mask individually */
				mask = get_stage1_mask_for_bond(bond, iterator, type);
				/* Print the mask for convinient checking */
				print_mask(mask);
				++iterator;
			}

			/* Get the bond from mask for verification */
			pbond = find_bond_mask(mask);
			LOG(OUTPUT, "Printed bond: %d\n", pbond);
		}
		++bond;
	}


	/* Printing mask table for SHRINKING stage 2 */
	LOG(OUTPUT, "\n\n\nMasks for Stage 2 of SHRINKING and THINNING\n");
	bond = 1;
	type = SHRINKING;
	while(bond < 12) {

		/* Get the number of masks for the bond */
		num = get_num_elements_for_stage2_bond(bond, type);
		if(num > 0) {
			LOG(OUTPUT, "\n\nBond: %d\n", bond);
			iterator = 0;
			while(iterator < num) {
				/* Get each mask individually */
				mask = get_stage2_mask_for_bond(bond, iterator, type);
				/* Print the mask for convinient checking */
				print_mask(mask);
				++iterator;
			}

			/* Get the bond from mask for verification */
			pbond = find_bond_mask(mask);
			LOG(OUTPUT, "Printed bond: %d\n", pbond);
		}
		++bond;
	}

	/* Printing mask table for SKELETONIZING stage 2 */
	LOG(OUTPUT, "\n\n\nMasks for Stage 2 of SKELETONIZING\n");
	bond = 1;
	type = SKELETONIZING;
	while(bond < 12) {

		/* Get the number of masks for the bond */
		num = get_num_elements_for_stage2_bond(bond, type);
		if(num > 0) {
			LOG(OUTPUT, "\n\nBond: %d\n", bond);
			iterator = 0;
			while(iterator < num) {
				/* Get each mask individually */
				mask = get_stage2_mask_for_bond(bond, iterator, type);
				/* Print the mask for convinient checking */
				print_mask(mask);
				++iterator;
			}

			/* Get the bond from mask for verification */
			pbond = find_bond_mask(mask);
			LOG(OUTPUT, "Printed bond: %d\n", pbond);
		}
		++bond;
	}

}


/*
Function   : create_output_image
Input      : tagImageProp *input_img  - Input Image
             tagImageProp *output_img - Output Image
             uint8_t      *X          - Binarized Image
             tagForegroundColor fg    - Foreground color
Output     : None
Description: Create the output image from the binarized image
*/
void create_output_image(tagImageProp *input_img, tagImageProp *output_img, uint8_t *X, tagForegroundColor fg)
{

      int iterator = 0;
      uint8_t *temp_buf = NULL;

      if(fg == FG_BLACK) {
            memset(output_img->buffer, 0xFF, output_img->size);
      }
      while(iterator < input_img->pixel_count) {
            temp_buf = X + iterator;
            if(fg == FG_WHITE)
                  *(output_img->buffer + iterator) = *temp_buf * 255;
            else {
                  if(*temp_buf > 0)
                        *(output_img->buffer + iterator) = 0;
            }
            ++iterator;
      }
}


/*
Function   : binarise_input
Input      : tagImageProp *input_img - Input image
             uint8_t      *X         - Binarized image
             tagForegroundColor fg   - foreground image
Output     : None
Description: Binarize the input image to contain 1
             for foreground and 0 for background.
*/
void binarise_input(tagImageProp *input_img, uint8_t *X, tagForegroundColor fg)
{
      int   iterator = 0;
      uint8_t *temp_buf = NULL;

      while(iterator < input_img->pixel_count) {

            temp_buf = input_img->buffer + iterator;

            if(fg == FG_WHITE) {
                  if(*temp_buf > 0) {
                        *(X + iterator) = 1;
                  } else {
                        *(X + iterator) = 0;
                  }
            } else {
                  if(*temp_buf == 0) {
                        *(X + iterator) = 1;
                  } else {
                        *(X + iterator) = 0;
                  }
            }
            ++iterator;
      }
}


/*
Function   : get_shrinked_dot_count
Input      : tagImageProp *input_img - Input Image
             uint8_t      *buffer    - Input image pixel
Output     : int  - Total Count
Description: Return the total number of single foreground
             pixels surrounded by background pixels.
*/
int get_shrinked_dot_count(tagImageProp *input_img, uint8_t *buffer)
{
      uint8_t window;
      int fg_dots = 0;
      int iterator = 0;
      int vert = 0;
      int horiz = 0;
      uint8_t *temp_buf = NULL;

      while(iterator < input_img->pixel_count) {

            temp_buf = buffer + iterator;
            vert = iterator / input_img->width;
            horiz = iterator % input_img->width;

            if(*temp_buf > 0) {
                  window    = get_window_at_current_location(input_img, buffer, vert, horiz);

                  if(window == 0) {
                        ++fg_dots;
                  }
            }
            ++iterator;

            if(iterator == (3 * input_img->pixel_count) / 5) {
            	LOG(DEBUG, "COUNT: %d\n", fg_dots);
            }

      }
      return fg_dots;
}


/*
Function   : get_window_at_current_location
Input      : tagImageProp *input_img - Input Image object
             uint8_t *buffer         - Buffer to operate on
             int vert                - Vertical Coordinate of the pixel
             int horiz               - Horizontal Coordinate of the pixel
Output     : uint8_t - Window
Description: Return the window formed at the current location arranged in a byte
             X3	 X2	X1
             X4  X  X0
             X5  X6 X7
             with X0 as the LSB and X7 as the MSB; X being the current pixel
*/
uint8_t get_window_at_current_location(tagImageProp *input_img, uint8_t *buffer, int vert, int horiz)
{
      uint8_t window = 0;
      uint8_t zero_value = 0;
      uint8_t *temp_buf[8];
      int pointer = (vert * input_img->width + horiz) * input_img->bpp;

      if((vert == (input_img->height - 1)) || (horiz == (input_img->width - 1))) {
            temp_buf[0] = &zero_value;
      } else {
            pointer = ((vert + 1) * input_img->width + (horiz + 1)) * input_img->bpp;
            temp_buf[0] = buffer + pointer;
      }
      if(*temp_buf[0] > 0) {
            window = window | 0x1;
      } else {
            window = window | 0x0;
      }
      window = window << 1;

      if(vert == (input_img->height - 1)) {
            temp_buf[1] = &zero_value;
      } else {
            pointer = ((vert + 1) * input_img->width + horiz) * input_img->bpp;
            temp_buf[1] = buffer + pointer;
      }
      if(*temp_buf[1] > 0) {
            window = window | 0x1;
      } else {
            window = window | 0x0;
      }
      window = window << 1;

      if((vert == (input_img->height - 1)) || (horiz == 0)) {
            temp_buf[2] = &zero_value;
      } else {
            pointer = ((vert + 1) * input_img->width + (horiz - 1)) * input_img->bpp;
            temp_buf[2] = buffer + pointer;
      }
      if(*temp_buf[2] > 0) {
            window = window | 0x1;
      } else {
            window = window | 0x0;
      }
      window = window << 1;

      if(horiz == 0) {
            temp_buf[3] = &zero_value;
      } else {
            pointer = (vert * input_img->width + (horiz - 1)) * input_img->bpp;
            temp_buf[3] = buffer + pointer;
      }
      if(*temp_buf[3] > 0) {
            window = window | 0x1;
      } else {
            window = window | 0x0;
      }
      window = window << 1;

      if((vert == 0) || (horiz == 0)) {
            temp_buf[4] = &zero_value;
      } else {
            pointer = ((vert - 1) * input_img->width + (horiz - 1)) * input_img->bpp;
            temp_buf[4] = buffer + pointer;
      }
      if(*temp_buf[4] > 0) {
            window = window | 0x1;
      } else {
            window = window | 0x0;
      }
      window = window << 1;

      if(vert == 0) {
            temp_buf[5] = &zero_value;
      } else {
            pointer = ((vert - 1) * input_img->width + horiz) * input_img->bpp;
            temp_buf[5] = buffer + pointer;
      }
      if(*temp_buf[5] > 0) {
            window = window | 0x1;
      } else {
            window = window | 0x0;
      }
      window = window << 1;


      if((vert == 0) || (horiz == (input_img->width - 1))) {
            temp_buf[6] = &zero_value;
      } else {
            pointer = ((vert - 1) * input_img->width + (horiz + 1)) * input_img->bpp;
            temp_buf[6] = buffer + pointer;
      }
      if(*temp_buf[6] > 0) {
            window = window | 0x1;
      }      else {
            window = window | 0x0;
      }
      window = window << 1;

      if(horiz == (input_img->width - 1)) {
            temp_buf[7] = &zero_value;
      } else {
            pointer = (vert * input_img->width + (horiz + 1)) * input_img->bpp;
            temp_buf[7] = buffer + pointer;
      }
      if(*temp_buf[7] > 0) {
            window = window | 0x1;
      } else {
            window = window | 0x0;
      }

      return window;

}


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
float** matrix_multiply_float(float **A, int *dim_A, float **B, int *dim_B)
{
      float **output;
      int i = 0;
      int j = 0;
      int k = 0;
      float sum = 0;


      if(dim_A[1] != dim_B[0]) {
            return NULL;
      }

      output = (float **) malloc(dim_A[0] * sizeof(float *));
      for(i = 0; i < dim_A[0]; ++i) {
            output[i] = (float *) malloc(dim_B[1] * sizeof(float));
      }

      for(i = 0; i < dim_A[0]; ++i) {

            for(j = 0; j < dim_B[1]; ++j) {

                  sum = 0;
                  for(k = 0; k < dim_A[1]; ++k) {
                        sum += A[i][k] * B[k][j];
                  }

                  output[i][j] = sum;

            }

      }

      return output;

}


/*
Function   : print_matrix_float
Input      : float **matrix - Matrix
			 int   *dim     - Dimension of matrix
Output     : None
Description: Print the matrix in conventional format
*/
void print_matrix_float(float **matrix, int *dim)
{

      int i = 0;
      int j = 0;

      for(i = 0; i < dim[0]; ++i) {
            LOG(OUTPUT, "\n");
            for(j = 0; j < dim[1]; ++j) {
                  LOG(OUTPUT, "%0.2f ", matrix[i][j]);
            }
      }

      LOG(OUTPUT, "\n");

}
