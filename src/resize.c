#include "resize.h"

/*
Function   : bilinear_interpolation
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Resize the input image
*/
void bilinear_interpolation(tagImageProp *input_img, tagImageProp *output_img) 
{
	/* Temporary Variables */
	int iterator      = 0;
	int channel_count = 0;

	/* Bilinear Interpolation Variables */
	int   base_w 	= 0;
	int   base_wp1 	= 0;
	int   base_h 	= 0;
	int   base_hp1 	= 0;
	int   horiz 	= 0;
	int   vert 		= 0;
	float sw 		= 0.0;
	float sh 		= 0.0;
	float delta_w 	= 0.0;
	float delta_h 	= 0.0;
	float wc 		= 0.0;
	float hc 		= 0.0;

	/* Temporary Buffers */
	uint8_t *buffern1 = NULL;
	uint8_t *buffern2 = NULL;
	uint8_t *buffern3 = NULL;
	uint8_t *buffern4 = NULL;



	/* Bilinear Interpolation */

	/* Ratio of change in dimensions of image */
	sw = (float) input_img->width / (float) output_img->width;
	sh = (float) input_img->height / (float) output_img->height;

	iterator = 0;
	while(iterator < output_img->pixel_count) {

		/* Horizontal and vertical indices of pixel of image */
		horiz = iterator % output_img->width;
		vert  = iterator / output_img->width;

		/* Mapping from new indices to indices of input image */
		wc = (float) horiz * sw;
		hc = (float) vert * sh;

		/* Corresponding old indices of input image */
		base_h = hc;
		base_w = wc;

		/* Displacement of new pixel wrt input image pixels */
		delta_w = wc - (float) base_w;
		delta_h = hc - (float) base_h;

		/* Displacement of new pixel wrt input image farther end pixels */
		base_wp1 = base_w;
		base_hp1 = base_h;

		/* Check if these are not corner cases */
		if(base_h < input_img->height) 
			base_hp1++;
		if(base_w < input_img->width)
			base_wp1++;


		/* Get the four surrounding pixels of input image */ 
		buffern1 = get_input_pixel_ptr(input_img, base_h, base_w);
		buffern2 = get_input_pixel_ptr(input_img, base_hp1, base_w);
		buffern3 = get_input_pixel_ptr(input_img, base_h, base_wp1);
		buffern4 = get_input_pixel_ptr(input_img, base_hp1, base_wp1);		


		/*
		/* Interpolate the intensity of the new pixel using 
		/* the four surrounding input image pixels with 
		/* bilinear interpolation.
		*/
		channel_count = 0;
		while(channel_count < output_img->bpp) {
			*(output_img->buffer + iterator*output_img->bpp + channel_count) =  *(buffern1 + channel_count) * (1 - delta_h) * (1 - delta_w) + \
																				*(buffern2 + channel_count) * (delta_h) * (1 - delta_w) + \
																				*(buffern3 + channel_count) * (1 - delta_h) * (delta_w) + \
																				*(buffern4 + channel_count) * (delta_w) * (delta_h);
			++channel_count;
		}
		++iterator;
	}

}
