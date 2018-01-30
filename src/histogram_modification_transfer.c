#include "histogram_modification_transfer.h"

// #define PRINT_OUTPUT

/*
Function   : histogram_modify 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagHistModifications method - Operation: 1. Equalize histogram
             										  2. Modify histogram
Output     : None
Description: Modify histogram based on the operation
*/
void histogram_modify(tagImageProp *input_img, tagImageProp *output_img, tagHistModifications method)
{

	/* Temporary Variables */
	int32_t      iterator               = 0;
	int32_t      channel_count          = 0;
	int32_t      tempval                = 0;
	uint8_t *buffer                 = NULL;

	/* Array to store histogram values */
	int32_t 	 gray_level_bin[RGB_BITS_PER_PIXEL][RGB_PIXEL_LEVELS] = {0};

	/* Array to store the probability table */
	float    prob_bins[RGB_PIXEL_LEVELS] 	= {0.0};

	/* Array to store the cdf values  */
	float    cdf_bins[RGB_PIXEL_LEVELS]	 	= {0.0};

	/* Generic custom decided transfer function */
	float    transfer_func          			= 0.0;

	/* Array to store the corresponding transfer function range */
	float    tx_red_bins[RGB_PIXEL_LEVELS]	 	= {0.0};
	float    tx_green_bins[RGB_PIXEL_LEVELS] 	= {0.0};
	float    tx_blue_bins[RGB_PIXEL_LEVELS] 	= {0.0};



	/* Find the histogram of the image */
	while(channel_count < input_img->bpp) {

		iterator = 0;
		while(iterator < input_img->pixel_count) {

			buffer = input_img->buffer + iterator * input_img->bpp + channel_count;
			gray_level_bin[channel_count][*(buffer)]++;
			++iterator;
		}		
		++channel_count;
	}


	iterator = 0;
	LOG(OUTPUT, "Histogram of input image: \n");
	while(iterator < RGB_PIXEL_LEVELS) {
		LOG(OUTPUT, "Red %d Green %d Blue %d\n", gray_level_bin[RED_CHANNEL][iterator], \
			                                gray_level_bin[GREEN_CHANNEL][iterator], \
			                                gray_level_bin[BLUE_CHANNEL][iterator]);
		++iterator;
	}
	LOG(OUTPUT, "\n");

	/* Required operation is histogram equalization */
	if(method == HIST_EQUALIZE_TRANSFER_FUNCTION) {

		channel_count = 0;
		while(channel_count < input_img->bpp) {

			/* Probability Table */
			iterator = 0;
			while(iterator < RGB_PIXEL_LEVELS) {
				prob_bins[iterator] = (float)gray_level_bin[channel_count][iterator] / (float)input_img->pixel_count;
				++iterator;
			}

			/* cdf table */	
			cdf_bins[0] = prob_bins[0];
			iterator = 1;
			LOG(OUTPUT, "CDF of input image histogram: \n");
			while(iterator < RGB_PIXEL_LEVELS) {
				cdf_bins[iterator] = prob_bins[iterator] + cdf_bins[iterator - 1];

				LOG(OUTPUT, "Channel: %d CDF %f \n", channel_count, cdf_bins[iterator]);

				++iterator;
			}

			/* Map the output image based on the CDF for the input image */
			iterator = 0;
			while(iterator < output_img->pixel_count) {
				buffer = input_img->buffer + iterator * output_img->bpp + channel_count;
				*(output_img->buffer + iterator*output_img->bpp + channel_count) = cdf_bins[*(buffer)] * MAX_PIXEL_LEVEL;
				++iterator;
			}

			++channel_count;
		}

	} else if(method == HIST_CUSTOM_TRANSFER_FUNCTION) { // Operation is custom transfer function

		/* Create the transfer function array for each channel */
		iterator = 0;
		while(iterator < RGB_PIXEL_LEVELS) {

			/* Custom transfer function: i ^ 0.7 / 255 ^ 0.7 */
			transfer_func = powf((float)iterator , 0.7) / (float) pow((float)MAX_PIXEL_LEVEL, 0.7);

			/* Using same transfer function for all channels */
			tx_red_bins[iterator] = transfer_func;
			tx_green_bins[iterator] = transfer_func;
			tx_blue_bins[iterator] = transfer_func;

			++iterator;

		}


		/* Create the output image based on transfer function bins */
		iterator = 0;
		while(iterator < output_img->pixel_count) {
			buffer = input_img->buffer + iterator * output_img->bpp;

			/* Add custom RGB weights */
			/* Weight R by 1.5 times, G by 2/3 and B by 2/3 */

			tempval = *(buffer + RED_CHANNEL) * 1.5;
			if(tempval > MAX_PIXEL_LEVEL) {
				tempval = MAX_PIXEL_LEVEL;
			}

			*(output_img->buffer + iterator*output_img->bpp + RED_CHANNEL) = tx_red_bins[tempval] * MAX_PIXEL_LEVEL;

			tempval = (*(buffer + GREEN_CHANNEL) * 2 ) / 3;
			*(output_img->buffer + iterator*output_img->bpp + GREEN_CHANNEL) = tx_green_bins[tempval] * MAX_PIXEL_LEVEL;

			tempval = (*(buffer + BLUE_CHANNEL) * 2) / 3;
			*(output_img->buffer + iterator*output_img->bpp + BLUE_CHANNEL) = tx_blue_bins[tempval] * MAX_PIXEL_LEVEL;

			++iterator;
		}


	}

}