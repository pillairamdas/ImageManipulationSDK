#include "format_conversion.h"

/*
Function   : get_grayscale_lightness 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate grayscale image based on lightness method
*/
void get_grayscale_lightness(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator = 0;
	uint8_t     *buffer   = NULL;

	/*
	/* Iterate ove the input image pixels and convert to GRAYSCALE image using 
	/* using formula: GRAY = max(R, G, B) + min(R, G, B) / 2;
	*/
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;
		*(output_img->buffer + iterator*output_img->bpp) = (max_of_3(*(buffer + R_CHANNEL), \
																	 *(buffer + G_CHANNEL), \
																	 *(buffer + B_CHANNEL)) 
		                                                  + min_of_3(*(buffer + R_CHANNEL), \
		                                                  			 *(buffer + G_CHANNEL), \
		                                                  			 *(buffer + B_CHANNEL))) / 2;
		iterator++;   
	}
}


/*
Function   : get_grayscale_average 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate grayscale image based on average method
*/
void get_grayscale_average(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator = 0;
	uint8_t     *buffer   = NULL;

	/*
	/* Iterate ove the input image pixels and convert to GRAYSCALE image using 
	/* using formula: GRAY = (R + G + B) / 3;
	*/
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;		
		*(output_img->buffer + iterator*output_img->bpp) = (*(buffer + R_CHANNEL) + \
															*(buffer + G_CHANNEL) + \
															*(buffer + B_CHANNEL)) / 3;
		iterator++;   
	}
}


/*
Function   : get_grayscale_luminosity 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate grayscale image based on luminosity method
*/
void get_grayscale_luminosity(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator = 0;
	uint8_t     *buffer   = NULL;

	/*
	/* Iterate ove the input image pixels and convert to GRAYSCALE image using 
	/* using formula: GRAY = 0.21R + 0.72G + 0.07B;
	*/
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;		
		*(output_img->buffer + iterator*output_img->bpp) = (0.21 * (float)*(buffer + R_CHANNEL) + \
														    0.72 * (float)*(buffer + G_CHANNEL) + \
														    0.07 * (float)*(buffer + B_CHANNEL));
		iterator++;   
	}
}


/*
Function   : convert_to_cmy 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to CMY
*/
void convert_to_cmy(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator      = 0;
	int32_t      channel_count = 0;
	uint8_t     *buffer        = NULL;

	/* Invert the RGB pixel intensities */
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;
		
		channel_count = 0;

		while(channel_count < output_img->bpp) {
			*(output_img->buffer + iterator*output_img->bpp + channel_count)     = MAX_PIXEL_LEVEL - *(buffer + channel_count);
			++channel_count;
		}

		iterator++;
	}	
}


/*
Function   : convert_to_c 
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to CYAN
*/
void convert_to_c(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator = 0;
	uint8_t     *buffer   = NULL;

	/* Invert the R pixels and store them as grayscale */
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;
		
		*(output_img->buffer + iterator*output_img->bpp)     = MAX_PIXEL_LEVEL - *(buffer + R_CHANNEL);
		iterator++;
	}	
}


/*
Function   : convert_to_m
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to MAGENTA
*/
void convert_to_m(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator = 0;
	uint8_t     *buffer   = NULL;

	/* Invert the G pixels and store them as grayscale */
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;
		*(output_img->buffer + iterator * output_img->bpp + G_CHANNEL) = MAX_PIXEL_LEVEL - *(buffer + G_CHANNEL);
		iterator++;
	}	
}


/*
Function   : convert_to_y
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate RGB to YELLOW
*/
void convert_to_y(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator = 0;
	uint8_t     *buffer   = NULL;

	/* Invert the B pixels and store them as grayscale */
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;
		*(output_img->buffer + iterator*output_img->bpp + B_CHANNEL) = MAX_PIXEL_LEVEL - *(buffer + B_CHANNEL);
		iterator++;
	}	
}


/*
Function   : convert_to_negativeRGB
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Generate a negative RGB image.
*/
void convert_to_negativeRGB(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      iterator      = 0;
	int32_t      channel_count = 0;
	uint8_t     *buffer        = NULL;

	/* Invert the RGB pixel intensities */
	while(iterator < output_img->pixel_count) {
		buffer = input_img->buffer + iterator*input_img->bpp;
		
		channel_count = 0;

		while(channel_count < output_img->bpp) {
			*(output_img->buffer + iterator*output_img->bpp + channel_count)     = MAX_PIXEL_LEVEL - *(buffer + channel_count);
			++channel_count;
		}

		iterator++;
	}		
}