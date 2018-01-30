#include "histogram_equalization_uniform.h"


/*
Function   : uniform_histogram_equalize 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Apply uniform histogram equalization on the image.
*/
void uniform_histogram_equalize(tagImageProp *input_img, tagImageProp *output_img)
{
	/* Temporary Variables */
	int32_t      iterator 		  	 = 0;
	int32_t 	 loop_count			 = 0;
	int32_t      channel_count       = 0;
	uint8_t     *buffer              = NULL;
	int32_t      tempval1            = 0;
	
	/* Gray to read histogram */
	int32_t 	 gray_bin[RGB_PIXEL_LEVELS]            = {0};

	/* Bucket indicator for each pixel intensity, that is,
	/* this tells us how many pixels are required by this bucket
	/* to fill up completely */
	int32_t      G[RGB_PIXEL_LEVELS]                   = {0};

	/* Mapping table: Gives a meaning of which intensity
	/* is contributing how much to which pixel intensities */
	int32_t      H[RGB_PIXEL_LEVELS][RGB_PIXEL_LEVELS] = {0};

	/* Number of pixels to be allocated to each pixel intensity bucket */
	int32_t      nLevel              				   = 0;

	/* Measure used to know how many pixels spent from the bucket */
	int32_t      diff     			 				   = 0;

	/* Tells the number of pixel intensities which will have 1 
	/* more pixels than the median  */
	int32_t      nRem                				   = 0;


	/* Compute the nuber of pixels for each intensity bucket */
	nLevel = input_img->pixel_count / RGB_PIXEL_LEVELS;

	/* If some remainder, compute that */ 
	nRem   = input_img->pixel_count % RGB_PIXEL_LEVELS;

	/* Loop through each channel one at a time */
	channel_count = 0;
	while(channel_count < input_img->bpp) {

		/* Reset the gray bin for histogram for each channel */
		memset(gray_bin, 0, RGB_PIXEL_LEVELS * sizeof(int32_t));
		
		/* Initially fill all the buckets */
		/* If Remainder is present, add 1 pixel to each intensity to compensate */
		iterator = 0;
		while(iterator < RGB_PIXEL_LEVELS) {
			if(iterator < nRem)
				G[iterator] = nLevel+1;
			else	
				G[iterator] = nLevel;
			++iterator;
		}

		/* Compute the histogram for this channel */
		iterator = 0;
		while(iterator < input_img->pixel_count) {

			buffer = input_img->buffer + iterator * input_img->bpp + channel_count;

			gray_bin[*(buffer)]++;
			++iterator;
		}

#ifdef PRINT_OUTPUT
		iterator = 0;
		while(iterator < RGB_PIXEL_LEVELS) {
			LOG(DEBUG, "Histogram[%d] %d\n", channel_count, gray_bin[iterator]);
			++iterator;
		}
#endif 
		
		/* Reset the mapping table for this channel */
		memset(H, 0, RGB_PIXEL_LEVELS*RGB_PIXEL_LEVELS*sizeof(int32_t));


		/* Iterate over all pixel intensities to create the mapping table */
		iterator = 0;
		loop_count = 0;
		while(iterator < RGB_PIXEL_LEVELS) {

			/* If the current bucket has some pixels to be removed */ 
			while(G[loop_count] > 0) {

				/* Check if current pixel intensity has less buffer 
				/* than required, that is, will the current pixel intensity
				/* be depleted in filling the bucket */ 
				if(gray_bin[iterator] <= G[loop_count]) {

					/* Remove the amount of pixel to be removed from the bucket
					/* equal to the amount of pixels present in this
					/* pixel intensity */
					G[loop_count] = G[loop_count] - gray_bin[iterator];

					/* Create an entry in the mapping table that this pixel
					/* intensity is contributing to loop_count pixel intensity
					/* with value of its total number of pixels */
					H[iterator][loop_count] = gray_bin[iterator];

					/* Now that the number of pixels for this intensity is 
					/* depleted, let's reset it to 0.
					gray_bin[iterator] = 0; */


					/* Check if just in case, the bucket is also completely 
					/* filled, if so, let's move to next bucket */
					if(G[loop_count] == 0)
						++loop_count;
					break;

				} else {
					/* This else means that the current intensity has 
					/* more than it is required by the current bucket */

					/* So the the current filled amount is what is 
					/* remaining in this bucket */
					diff = G[loop_count];

					/* Now that this bucket is completely filled, we should
					/* set its required amount ot 0 */
					G[loop_count] = 0;

					/* Create an entry in the mapping table that this intensity filled
					/* diff amount of pixels to the current bucket */
					H[iterator][loop_count] = diff;

					/* Now that the bucket is filled, let's move to next bucket */
					loop_count++;

					/* Since diff amount of pixels are spent on the previous 
					/* bucket, we should reset the number of pixels available
					/ to original number - diff */
					gray_bin[iterator] = gray_bin[iterator] - diff;
					
				}
			}
			++iterator;
		}

		/* Iterate over each pixel of the input image, and map the 
		/* corresponding entry for that pixel in the mapping table to identify
		/* to which pixel intensity does this map to and reduce the count of 
		/* remaining contribution of this pixel intensity */
		iterator = 0;
		loop_count = 0;
		while(iterator < input_img->pixel_count) {

			buffer = input_img->buffer + iterator * input_img->bpp + channel_count;
			loop_count = 0;

			/* Iterate thorugh all 256 pixel levels */
			while(loop_count < RGB_PIXEL_LEVELS) {

				/* If the mapping entry or contribution for current pixel 
				/* intensity to another intensity is more than 0, this means
				/* map this intensity to this other intensity. */
				if(H[*(buffer)][loop_count] > 0) {
					tempval1 = loop_count;
					H[*(buffer)][loop_count]--;
					break;
				}
				++loop_count;
			}

			/* Assign the output image */
			*(output_img->buffer + iterator * output_img->bpp + channel_count) = tempval1;
			++iterator;
		}
	++channel_count;
	}
}
