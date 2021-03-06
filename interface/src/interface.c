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
Output     : tagStatus
Description: Change the color palette of the image.
*/
tagStatus change_color_palette(tagImageProp *input_img, tagImageProp *output_img, long long int colors) 
{
	tagStatus status = ERROR_NONE;
	status = convert_color(input_img, output_img, colors);
	return status;
}


/*
Function   : photo_effect
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             long long int colors     - RGB Color palette levels
             int32_t window_size      - Window size
             tagEffects effects       - Effects to incorporate like OIL EFFECT, FILM EFFECT
Output     : tagStatus
Description: Bring in effects to the image.
*/
tagStatus photo_effect(tagImageProp *input_img, tagImageProp *output_img, long long int colors, int32_t window_size, tagEffects effects)
{
	tagStatus status = ERROR_NONE;
	/* if the user wants OIL EFFECT on the input image */
	if(effects == EFFECT_OIL_EFFECT)
		status = oil_effect(input_img, output_img, window_size);
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

	return status;
}


/*
Function   : contrast_manipulate
Input      : tagImageProp *input_img       - Input image
             tagImageProp *output_img      - Output Image
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
Input      : tagImageProp *input_img       - Input image
             tagImageProp *output_img      - Output Image
             tagHistEqualizeMethods method - Histogram equalization methods
Output     : tagStatus
Description: Bring in effects to the image.
*/
tagStatus remove_image_noise(tagImageProp *input_img, tagImageProp *output_img, int32_t window_size, tagNoiseRemovalMethods method)
{
	tagStatus status = ERROR_NONE;
	if(method == NOISE_REMOVE_MEDIAN_FILTER) {
		status = apply_filter(input_img, output_img, window_size, FILTER_MEDIAN);
	} else if(method == NOISE_REMOVE_GAUSSIAN_FILTER) {
		printf("%s %d\n",__FILE__, __LINE__);
		status = apply_filter(input_img, output_img, window_size, FILTER_GAUSSIAN);
	} else if(method == NOISE_REMOVE_LINEAR_FILTER) {
		status = apply_filter(input_img, output_img, window_size, FILTER_LINEAR);
	}

	return status;
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

/*
Function   : warping
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
Output     : None
Description: Warp the square input image to circle
*/
void warping(tagImageProp *input_img, tagImageProp *output_img)
{
	warp_image(input_img, output_img);
}


/*
Function   : dewarping
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
Output     : None
Description: Dewarp the circle input image to circle
*/
void dewarping(tagImageProp *input_img, tagImageProp *output_img)
{
	dewarp_image(input_img, output_img);
}


/*
Function   : stitch
Input      : tagImageProp *left_img   - Left image
             tagImageProp *middle_img - Middle image
             tagImageProp *right_img  - Right image
Output     : tagImageProp* - Stitched output image
Description: Stitch the left middle and right image based on
             manual selection of key points.
*/
tagImageProp* stitch(tagImageProp *left_img, tagImageProp *middle_img, tagImageProp *right_img)
{
	return stitch_image(left_img, middle_img, right_img);
}


/*
Function   : threshold
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Perform thresholding based on bpp
*/
void threshold(tagImageProp *input_img, tagImageProp *output_img)
{
	if(input_img->bpp == 1) {
		threshold_gray_image(input_img, output_img, 128);
	}

}


/*
Function   : dithering
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
             tagDithertingMethod method - Dithering method
Output     : None
Description: Perform dithering on the grayscale input image
*/
void dithering(tagImageProp *input_img, tagImageProp *output_img, tagDithertingMethod method)
{
	if(method <= DITHERING_FIXED_QUAD ) {
		fixed_thresholding(input_img, output_img, method);
	} else if(method <= DITHERING_RANDOM_QUAD) {
		random_thresholding(input_img, output_img, method);
	} else {
		bayer_matrix_dithering(input_img, output_img, method);
	}

}


/*
Function   : error_diffuse
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
             tagErrorDiffusionMethod method - Error diffusion method
Output     : None
Description: Perform error diffusion on the input grayscale image.
*/
void error_diffuse(tagImageProp *input_img, tagImageProp *output_img, tagErrorDiffusionMethod method)
{
	error_diffusion_binary(input_img, output_img, method);
}


/*
Function   : color_halftone
Input      : tagImageProp *input_img    - Input image
             tagImageProp *output_img   - Output Image
             tagColorHalftoneMethod method - color halftoning method
Output     : None
Description: Perform color halftoning on the input image
*/
void color_halftone(tagImageProp *input_img, tagImageProp *output_img, tagColorHalftoneMethod method)
{
	color_halftone_images(input_img, output_img, method);
}


/*
Function   : shrinking
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Perform shrinking on the input image
*/
void shrinking(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor fg)
{
	if(input_img->bpp == 1) {
		perform_shrinking_thinning_skeletonize_binary(input_img, output_img, fg, SHRINKING);
	}

}


/*
Function   : thinning
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Perform thinning on the input image
*/
void thinning(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor fg)
{
	if(input_img->bpp == 1) {
		perform_shrinking_thinning_skeletonize_binary(input_img, output_img, fg, THINNING);
	}

}


/*
Function   : skeletonize
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Perform skeletonizing on the input image
*/
void skeletonize(tagImageProp *input_img, tagImageProp *output_img, tagForegroundColor fg)
{
	if(input_img->bpp == 1) {
		perform_shrinking_thinning_skeletonize_binary(input_img, output_img, fg, SKELETONIZING);
	}

}


/*
Function   : counting_game
Input      : tagImageProp *input_img  - Input image
             tagForegroundColor fg    - foreground color
Output     : None
Description: Count the total and unique number of objects
*/
void counting_game(tagImageProp *input_img, tagForegroundColor fg)
{
	count_objects_binary(input_img, fg);
}

