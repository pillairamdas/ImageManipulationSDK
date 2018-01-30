#include "color_conversion.h"

/*
Function   : fetch_color_levels
Input      : tagImageProp *input_img  - Input image
             int32_t **color_level    - Color levels for each channel
             int32_t **boundary_level     - Boundary levels to be populated.
             int32_t *len                 - Color depth of the channel
             int32_t channel_count        - Channel index
Output     : None
Description: Identify the color levels and boundary levels based on the colors
*/
void fetch_color_levels(tagImageProp *input_img, int32_t **color_level, int32_t **boundary_level, int32_t *len, int32_t channel_count) 
{
	/* Temporary Variables */
	int32_t 	 iterator     	= 0;
	int32_t 	 i             	= 0; 
	int32_t 	 j             	= 0; 
	int32_t 	 k             	= 0; 
	int32_t 	 tempval       	= 0; 
	uint8_t *buffer   		= NULL;

	/* Stores the histogram for input image for a particular channel */
	int32_t 	 gray_bin[RGB_PIXEL_LEVELS] 	= {0};

	/* Sotres the median level of color bins */
	int32_t 	 median_level 	= 0;

	/* Stores the boundaries for each median level */
	int32_t 	 boundary    	= 0;


	/* Compute the median level based on the color depth */
	median_level = input_img->pixel_count / (2 * len[channel_count]);

	/* Compute the boundary based on the color depth */
	boundary     = input_img->pixel_count / len[channel_count];

	/* Get the histogram */
	iterator = 0;
	while(iterator < input_img->pixel_count) {

		buffer = input_img->buffer + iterator * input_img->bpp + channel_count;
		gray_bin[*(buffer)]++;
		++iterator;
	}


    /* Initialize temporary variables */
	iterator = 0;
	i 		 = 0; // Used as a color depth iterator
	j 		 = 0; // Used to check if boundary reached for each color depth. 
	k 		 = 0; // Used as depth iterator for checking boundary.

	while(iterator < RGB_PIXEL_LEVELS) {

		j       += gray_bin[iterator];
		tempval += gray_bin[iterator];

		/* Check if pixels accumulated so far is equal to median levels */
		if(tempval >= median_level) {

			*(color_level[channel_count] + i) = iterator;
			++i;
			tempval = -median_level;
		}

		/* Check if pixels accumulated so far is equal to boundary levels */
		if(j > boundary) {
			*(boundary_level[channel_count] + k) = iterator;
			++k;
			/* Readjust boundary for next median color depth level */
			boundary = boundary + input_img->pixel_count / len[channel_count];
		}
		++iterator;
	}
	*(boundary_level[channel_count] + k) = 255;
}

/*
Function   : convert_color
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             long long int colors     - Number of colors
Output     : tagStatus - Error Status for the function
Description: Change the color pallete
*/
tagStatus convert_color(tagImageProp *input_img, tagImageProp *output_img, long long int colors)
{
	/* Temporary Variables */
 	int32_t       iterator        = 0;
 	int32_t       tempval         = 0;
 	uint8_t  *buffer          = NULL;
 	int32_t       i               = 0;
 	int32_t       channel_count   = 0;

 	/* Store the depth level for R, G, B */
	int32_t       len[RGB_BITS_PER_PIXEL] = {0};

	/* The generic depth level as per color palette */
 	int32_t       color_cbr       = 0;

 	/* Color depth levels for each channel */
 	int32_t     **color_level     = NULL;

 	/* Boundary levels for color depth of each channel */
 	int32_t     **boundary_level  = NULL;


 	/* Special case of 256 colors */
	if(colors == 256) {
		len[RED_CHANNEL] = 8;
		len[GREEN_CHANNEL] = 8;
		len[BLUE_CHANNEL] = 4;
	} else {

		/* Get the cube root of the color to identify color levels for each channel */
		color_cbr = (int32_t) cbrt(colors);

		if(colors != (color_cbr * color_cbr * color_cbr)) {
			LOG(ERROR, "%lld is not a valid color level\n", colors);
			return ERROR_INCORRECT_COLOR_LEVEL;
		}

		len[RED_CHANNEL] = len[GREEN_CHANNEL] = len[BLUE_CHANNEL] = color_cbr;
	}


	/*
	/* Allocate memory for color level and boundary 
	/* levels according to the bytes per pixel of the image.
	*/
	color_level    = (int32_t **) malloc(input_img->bpp * sizeof(int32_t *));
	if(color_level == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;		
	}	
	boundary_level = (int32_t **) malloc(input_img->bpp * sizeof(int32_t *));
	if(boundary_level == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;		
	}

	channel_count = 0;
	while(channel_count < input_img->bpp) {

		/*
		/* Allocate memory for color level and boundary 
		/* levels according to the color depth required 
		/* for the color palette.
		*/
		color_level[channel_count] = (int32_t *) malloc(len[channel_count] * sizeof(int32_t));
		if(color_level[channel_count] == NULL) {
			LOG(ERROR, "Memory unavailable!\n");
			goto FREE_MEM;		
		}
		boundary_level[channel_count] = (int32_t *) malloc(len[channel_count] * sizeof(int32_t));
		if(boundary_level[channel_count] == NULL) {
			LOG(ERROR, "Memory unavailable!\n");
			goto FREE_MEM;		
		}		
		++channel_count;		
	}


	channel_count = 0;

	while(channel_count < input_img->bpp) {

		/* Fetch the color levels and boundary levels as per the color depth */
		fetch_color_levels(input_img, color_level, boundary_level, len, channel_count);
#ifdef DEBUG
		iterator = 0;
		LOG(DEBUG, "Channel %d: ", channel_count);
		while(iterator < len[channel_count]) {
			LOG(OUTPUT, " %d-%d ", *(color_level[channel_count] + iterator), *(boundary_level[channel_count] + iterator));
			++iterator;
		}
#endif

		/* 
		/* Go over the image pixel by pixel and replace the intensity 
		/* with intesity of nearest bin they belong to.
		*/
		iterator = 0;
		while(iterator < input_img->pixel_count) {		
			buffer = input_img->buffer + iterator*input_img->bpp + channel_count;
			i = 0;
			while(i < len[channel_count]) {
				if(*buffer <= boundary_level[channel_count][i]) {
					tempval = color_level[channel_count][i];
					break;
				}
				++i;
			}

			*(output_img->buffer + iterator*output_img->bpp + channel_count) = tempval;
			++iterator;
		}

		++channel_count;
	} 




FREE_MEM:

	/* Free all allocated memory */

	channel_count = 0;
	while(channel_count < input_img->bpp) {
		if(color_level[channel_count] != NULL) {
			free(color_level[channel_count]);
			color_level[channel_count] = NULL;
		}
		
		if(boundary_level[channel_count] != NULL) {
			free(boundary_level[channel_count]);
			boundary_level[channel_count] = NULL;	
		}
		
		++channel_count;		
	}

	if(color_level != NULL) {
		free(color_level);
		color_level = NULL;	
	}
	
	if(boundary_level != NULL) {
		free(boundary_level);
		boundary_level = NULL;	
	}

	return ERROR_NONE;
}
