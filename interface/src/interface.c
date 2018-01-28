#include "interface.h"


/*
Function   : convert
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Convert RGB to mentioned format
*/
void convert(tagImageProp *input_img, tagImageProp *output_img, tagColorspace cs)
{
	if(cs == COLORSPACE_GRAY_LIGHTNESS) {
		get_grayscale_lightness(input_img, output_img);
	} else if(cs == COLORSPACE_GRAY_AVERAGE) {
		get_grayscale_average(input_img, output_img);
	} else if(cs == COLORSPACE_GRAY_LUMINOSITY) {
		get_grayscale_luminosity(input_img, output_img);
	} else if(cs == COLORSPACE_CMY) {
		convert_to_cmy(input_img, output_img);
	} else if(cs == COLORSPACE_CYAN) {
		convert_to_c(input_img, output_img);
	} else if(cs == COLORSPACE_MAGENTA) {
		convert_to_m(input_img, output_img);
	} else if(cs == COLORSPACE_YELLOW) {
		convert_to_y(input_img, output_img);
	} else if(cs == COLORSPACE_NEGATIVE_RGB) {
		convert_to_negativeRGB(input_img, output_img);
	}
}


/*
Function   : resize
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Resize the input image
*/
void resize(tagImageProp *input_img, tagImageProp *output_img)
{
	bilinear_interpolation(input_img, output_img);
}


/*
Function   : change_color_palette
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             long long int colors     - RGB Color palette levels
Output     : None
Description: Change the color palette of the image.
*/
void change_color_palette(tagImageProp *input_img, tagImageProp *output_img, long long int colors) 
{
	convert_color(input_img, output_img, colors);
}


/*
Function   : photo_effect
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             long long int colors     - RGB Color palette levels
             int window_size          - Window size
             tagEffects effects       - Effects to incorporate like OIL EFFECT, FILM EFFECT
Output     : None
Description: Bring in effects to the image.
*/
void photo_effect(tagImageProp *input_img, tagImageProp *output_img, long long int colors, int window_size, tagEffects effects)
{
	/* if the user wants OIL EFFECT on the input image */
	if(effects == EFFECT_OIL_EFFECT)
		oil_effect(input_img, output_img, window_size);
	else if(effects == EFFECT_FILM_EFFECT) {

		tagImageProp *reflect_out = (tagImageProp *)malloc(sizeof(tagImageProp));
		if(reflect_out == NULL) {
			printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
			goto FREE_MEM;
		}
		tagImageProp *negative_rgb_out = (tagImageProp *)malloc(sizeof(tagImageProp));
		if(negative_rgb_out == NULL) {
			printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
			goto FREE_MEM;
		}

		memcpy(reflect_out, output_img, sizeof(tagImageProp));
		memcpy(negative_rgb_out, output_img, sizeof(tagImageProp));

		reflect_out->buffer = (uint8_t *) malloc(reflect_out->size);
		if(reflect_out->buffer == NULL) {
			printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
			goto FREE_MEM;
		}

		negative_rgb_out->buffer = (uint8_t *) malloc(negative_rgb_out->size);
		if(negative_rgb_out->buffer == NULL) {
			printf("[ERROR] %d: Memory unavailable!\n", __LINE__);
			goto FREE_MEM;
		}

		reflect_to_side(input_img, reflect_out);
		convert_to_negativeRGB(reflect_out, negative_rgb_out);
		histogram_modify(negative_rgb_out, output_img, HIST_CUSTOM_TRANSFER_FUNCTION);

FREE_MEM:
		if(negative_rgb_out->buffer != NULL) {
			free(negative_rgb_out->buffer);
			negative_rgb_out->buffer = NULL;
		}
		if(negative_rgb_out != NULL) {
			free(negative_rgb_out);
			negative_rgb_out = NULL;
		}
		if(reflect_out->buffer != NULL) {
			free(reflect_out->buffer);
			reflect_out->buffer = NULL;
		}
		if(reflect_out != NULL) {
			free(reflect_out);
			reflect_out = NULL;
		}

	}
}


/*
Function   : contrast_manipulate
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagHistEqualizeMethods method - Histogram equalization methods
Output     : None
Description: Bring in effects to the image.
*/
tagStatus contrast_manipulate(tagImageProp *input_img, tagImageProp *output_img, tagHistModifications method)
{
	if(method == HIST_EQUALIZE_TRANSFER_FUNCTION) {
		histogram_modify(input_img, output_img, HIST_EQUALIZE_TRANSFER_FUNCTION);
	} else if(method == HIST_EQUALIZE_EQUAL_PROBABILITY) {
		uniform_histogram_equalize(input_img, output_img);
	} else {
		return ERROR_INCORECT_OPTION;
	}

	return ERROR_NONE;
}


/*
Function   : remove_image_noise
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagHistEqualizeMethods method - Histogram equalization methods
Output     : None
Description: Bring in effects to the image.
*/
void remove_image_noise(tagImageProp *input_img, tagImageProp *output_img, int window_size, tagNoiseRemovalMethods method)
{
	if(method == NOISE_REMOVE_MEDIAN_FILTER) {
		apply_filter(input_img, output_img, window_size, FILTER_MEDIAN);
	} else if(method == NOISE_REMOVE_GAUSSIAN_FILTER) {
		apply_filter(input_img, output_img, window_size, FILTER_GAUSSIAN);
	} else if(method == NOISE_REMOVE_LINEAR_FILTER) {
		apply_filter(input_img, output_img, window_size, FILTER_LINEAR);
	}

}


/*
Function   : print_psnr
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Print PSNR statistics
*/
void print_psnr(tagImageProp *input_img, tagImageProp *output_img)
{
	get_PSNR(input_img, output_img);
}


/*
Function   : reflect
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Reflect the input image
*/
void reflect(tagImageProp *input_img, tagImageProp *output_img)
{
	reflect_to_side(input_img, output_img);
}
