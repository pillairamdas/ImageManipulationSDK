/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "warping.h"


/*
Function   : find_distance_from_center
Input      : float center - Center pixel index of image
			 int x        - The horizontal index of image coordinate
			 int y        - The vertical index of image coordinate
Output     : float - Distance from center of point(x,y)
Description: Return the distance from the center of the
             image from any point (x, y)
*/
float find_distance_from_center(float center, int x, int y) {
	return sqrt(pow(abs(center - x), 2) + pow(abs(center - y), 2));
}


/*
Function   : warp_image
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output image
Output     : None
Description: Warp the square image to map the form of a circle
*/
void warp_image(tagImageProp *input_img, tagImageProp *output_img)
{
	/* Temporary Variables */
	int      iterator 		= 0;
	uint8_t *buffer 		= NULL;
	int      channel_count 	= 0;

	/* Image coordinates of pixel */
	int      horiz 			= 0;
	int      vert 			= 0;

	/* Center coordinates of image */
	float    center 		= 0.0;

	/* Normalized x and y in range [-1, 1] */
	float normalized_y      = 0.0;
	float normalized_x      = 0.0;

	/* Normalized x square and y square in range [0, 1] */
	float normalized_y_sq   = 0.0;
	float normalized_x_sq   = 0.0;

	/* Normalized output x and y in range [-1, 1] */
	float normalized_out_y  = 0.0;
	float normalized_out_x  = 0.0;

	/* Scaled output image coordinates */
	int   out_horiz         = 0;
	int   out_vert          = 0;


	center = (input_img->width-1) / 2.0;

	iterator = 0;
	while(iterator < input_img->pixel_count) {

		horiz  = iterator % input_img->width;
		vert   = iterator / input_img->width;

		buffer = input_img->buffer + iterator*input_img->bpp;

		/* Map input image coordinates to range [-1, 1] */
		normalized_y     = (center - vert)/center;
		normalized_x     = (horiz - center) / center;

		/*
		 * Compute x square and y sqaure values required
		 * for future computations
		*/
		normalized_y_sq  = normalized_y * normalized_y;
		normalized_x_sq  = normalized_x * normalized_x;

		/*
		 * Computer normalized output coordinates by formula:
		 * x_out = x * sqrt(1 - (y^2 / 2))
		 * y_out = y * sqrt(1 - (x^2 / 2))
		*/
		normalized_out_x = normalized_x * sqrt(1 - normalized_y_sq/2.0);
		normalized_out_y = normalized_y * sqrt(1 - normalized_x_sq/2.0);

		/* Scale the normalized coordinates to image coordinates */
		out_horiz = center * normalized_out_x + center;
		out_vert  = center - center * normalized_out_y;

		/*
		 * Write the output image based on the computed
		 * coordinate information
		*/
		channel_count = 0;
		while(channel_count < output_img->bpp) {
			*(output_img->buffer + (out_vert*output_img->width + out_horiz)*output_img->bpp + channel_count) =  *(buffer + channel_count);
			++channel_count;
		}

		++iterator;
	}
}


/*
Function   : dewarp_image
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output image
Output     : None
Description: Dewarp the circle image to map the form of a square
*/
void dewarp_image(tagImageProp *input_img, tagImageProp *output_img)
{
	/* Temporary variables */
	int      iterator      = 0;
	uint8_t *buffer        = NULL;
	int      channel_count = 0;

	/* Image coordinate representation */
	int      horiz         = 0;
	int      vert          = 0;
	float    center        = 0.0;
	float    radius        = 0.0;


	/* Bilinear Interpolation Variables */
	int   base_w 	= 0;
	int   base_wp1 	= 0;
	int   base_h 	= 0;
	int   base_hp1 	= 0;
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

	/* Normalized x and y in range [-1, 1] */
	float normalized_y      = 0.0;
	float normalized_x      = 0.0;

	/* Normalized x square and y square in range [0, 1] */
	float normalized_y_sq   = 0.0;
	float normalized_x_sq   = 0.0;

	/* Normalized output x and y in range [-1, 1] */
	float normalized_out_y  = 0.0;
	float normalized_out_x  = 0.0;

	/* Scaled output image coordinates */
	float out_horiz         = 0;
	float out_vert          = 0;

	center = (input_img->width-1) / 2.0;
	radius = (input_img->width-1) / 2.0;

	iterator = 0;
	while(iterator < output_img->pixel_count) {

		horiz  = iterator % output_img->width;
		vert   = iterator / output_img->width;
		buffer = output_img->buffer + iterator*output_img->bpp;


		/* Map input image coordinates to range [-1, 1] */
		normalized_y     = (center - vert)/center;
		normalized_x     = (horiz - center) / center;

		/*
		 * Compute x square and y sqaure values required
		 * for future computations
		*/
		normalized_y_sq  = normalized_y * normalized_y;
		normalized_x_sq  = normalized_x * normalized_x;

		/*
		 * Computer normalized output coordinates by formula:
		 * x_out = x * sqrt(1 - (y^2 / 2))
		 * y_out = y * sqrt(1 - (x^2 / 2))
		*/
		normalized_out_x = normalized_x * sqrt(1 - normalized_y_sq/2.0);
		normalized_out_y = normalized_y * sqrt(1 - normalized_x_sq/2.0);

		/* Scale the normalized coordinates to image coordinates */
		out_horiz = center * normalized_out_x + center;
		out_vert  = center - center * normalized_out_y;


		/*
		 * Performing Bilinear Interpolation to get smooth input pixels
		*/
		wc = out_horiz;
		hc = out_vert;

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


		if(find_distance_from_center(center, base_wp1, base_hp1) > radius) {
			base_hp1 = base_h;
			base_wp1 = base_w;
		}
		if(find_distance_from_center(center, base_w, base_h) > radius) {
			base_h = base_hp1;
			base_w = base_wp1;
		}


		/* Get the four surrounding pixels of input image */
		buffern1 = get_image_pixel_ptr(input_img, base_h, base_w);
		buffern2 = get_image_pixel_ptr(input_img, base_hp1, base_w);
		buffern3 = get_image_pixel_ptr(input_img, base_h, base_wp1);
		buffern4 = get_image_pixel_ptr(input_img, base_hp1, base_wp1);


		/* Populate the output image with weighted neighboring input pixels */
		channel_count = 0;
		while(channel_count < output_img->bpp) {
			*(buffer + channel_count) =  *(buffern1 + channel_count) * (1 - delta_h) * (1 - delta_w) + \
										 *(buffern2 + channel_count) * (delta_h) * (1 - delta_w) +     \
										 *(buffern3 + channel_count) * (1 - delta_h) * (delta_w) +     \
										 *(buffern4 + channel_count) * (delta_w) * (delta_h);
			++channel_count;
		}

		++iterator;
	}
}
