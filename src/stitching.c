/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/


#include "stitching.h"

#define STITCH_LEFT
#define STITCH_RIGHT
#define LUMINANCE_VAL_TOP 		1.132
#define LUMINANCE_VAL_BOTTOM 	1.148


/*
Function   : stitch_image
Input      : tagImageProp *left_img   - Left image
             tagImageProp *middle_img - Middle Image
             tagImageProp *right_img  - Right Image
Output     : tagImageProp* - Output Image
Description: Stitch the left and right images on the middle image
			 based on manually selected key points and
			 inverse homography matrix computed using MATLAB.
*/
tagImageProp* stitch_image(tagImageProp *left_img, tagImageProp *middle_img, tagImageProp *right_img)
{
	/* Temporary variable */
	int     i               = 0;
	int     channel_count   = 0;
	uint8_t indicator_value = 0;
	uint8_t temp_val        = 0;
	float   lum_temp_val    = 0;


	/* Coordinate variables */
	int   k       = 0;
	int   j       = 0;
	int   p       = 0;
	int   q       = 0;
	float x       = 0.0;
	float y       = 0.0;
	float pb      = 0.0;
	float qb      = 0.0;
	float delta_x = 0.0;
	float delta_y = 0.0;


	/* 2D float matrix information */
	float **input;
	float **H_lm;
	float **H_rm;
	float **output;
	int   dim_input[2] = {3,1};
	int   dim_H[2]     = {3,3};

	/* Buffer requirements */
	uint8_t      *buffer     = NULL;
	uint8_t      *indicator  = NULL;
	tagImageProp *output_img = NULL;

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
	int32_t temp_buf1 = 0;
	int32_t temp_buf2 = 0;
	int32_t temp_buf3 = 0;
	int32_t temp_buf4 = 0;

	/* Memory allocation for output matrix, input matrix and homography matrices */
	input     = (float **)  malloc(dim_input[0] * sizeof(float *));
	if(input == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	H_lm      = (float **)  malloc(dim_H[0] * sizeof(float *));
	if(H_lm == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	H_rm      = (float **)  malloc(dim_H[0] * sizeof(float *));
	if(H_rm == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	output    = (float **)  malloc(dim_input[0] * sizeof(float *));
	if(output == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	for(i = 0; i < dim_H[0]; i++) {
		H_lm[i] = (float *) calloc(dim_H[1] * sizeof(float), 1);
		if(H_lm[i] == NULL) {
			LOG(ERROR, "Memory unavailable!\n");
			goto FREE_MEM;
		}
	}

	for(i = 0; i < dim_H[0]; i++) {
		H_rm[i] = (float *) calloc(dim_H[1] * sizeof(float), 1);
		if(H_rm[i] == NULL) {
			LOG(ERROR, "Memory unavailable!\n");
			goto FREE_MEM;
		}
	}

	for(i = 0; i < dim_input[0]; i++) {
		input[i] = (float *) malloc(dim_input[1] * sizeof(float));
		if(input[i] == NULL) {
			LOG(ERROR, "Memory unavailable!\n");
			goto FREE_MEM;
		}
	}

	for(i = 0; i < dim_input[0]; i++) {
		output[i] = (float *) malloc(dim_input[1] * sizeof(float));
		if(output[i] == NULL) {
			LOG(ERROR, "Memory unavailable!\n");
			goto FREE_MEM;
		}
	}


	/* Allocate memory for output and set its properties */
	output_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(output_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	output_img->width        = 2000;
	output_img->height       = 2000;
	output_img->pixel_count  = output_img->width * output_img->height;
	output_img->bpp          = middle_img->bpp;
	output_img->size         = output_img->pixel_count * output_img->bpp;
	output_img->buffer       = (uint8_t *) calloc(output_img->size, 1);
	if(output_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	indicator                = (uint8_t *) calloc(output_img->pixel_count, 1);
	if(indicator == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	/* Translate middle to output */
	delta_x = 800;
	delta_y = 800;

	j = 0;
	while(j < middle_img->height) {
		k = 0;
		while(k < middle_img->width) {

			/* Convert from image coordinates to Euclidean coordinates */
			x = k + 0.5;
			y = middle_img->height - 0.5 - j;

			/* Get the corresponding middle image pixel */
			buffer = (middle_img->buffer + (j*middle_img->width + k)*middle_img->bpp);

			/* Translate on the Euclidean coordinates */
			input[0][0] = x + delta_x;
			input[1][0] = y + delta_y;
			input[2][0] = 1;

			/* No need to any other operation */
			output[0][0] = input[0][0];
			output[1][0] = input[1][0];
			output[2][0] = input[2][0];

			/* Convert from Euclidean coordinates to image coordinates */
			q = (int)(output[0][0] - 0.5);
			p = (int)(output[1][0]*-1 - 0.5 + output_img->height);

			/* Write the middle image to the output blackboard */
			channel_count = 0;
			while(channel_count < RGB_BITS_PER_PIXEL) {
				*(output_img->buffer + (p*output_img->width + q)*middle_img->bpp + channel_count) = *(buffer + channel_count);
				++channel_count;
			}

			/* Note the pixel areas where a valid data is written */
			*(indicator + p*output_img->width + q) = 1;

			++k;
		}
		++j;
	}


#ifdef STITCH_LEFT   	/* Project left to  output */

	/* Inverse homography matrix to apply left image on blackboard output */
	H_lm[0][0] =  0.440823;    H_lm[0][1] = 0.017212; 	H_lm[0][2] = -201.499548;
	H_lm[1][0] = -0.149653;    H_lm[1][1] = 0.605773;  	H_lm[1][2] = -352.010576;
	H_lm[2][0] = -0.000457;    H_lm[2][1] = 0.000013; 	H_lm[2][2] =    1;

	j = 0;
	while(j < output_img->height) {

		k = 0;
		while(k < output_img->width) {

			/* Convert from image coordinates to Euclidean coordinates */
			x = k + 0.5;
			y = output_img->height - 0.5 - j;

			/* Get the output blackboard pixel at the current location */
			buffer = (output_img->buffer + (j*output_img->width + k)*output_img->bpp);

			input[0][0] = x;
			input[1][0] = y;
			input[2][0] = 1;

			output = matrix_multiply_float(H_lm, dim_H, input, dim_input);

			/* Convert from homogenous coordinates to Euclidean coordinates */
			output[0][0] = output[0][0] / output[2][0];
			output[1][0] = output[1][0] / output[2][0];

			/* Convert from Euclidean coordinates to image coordinates */
			qb = (float)(output[0][0] - 0.5);
			pb = (float)(output[1][0]*-1 - 0.5 + left_img->height);

			/* Perform following actions only if pixels lie within bounds of left image */
			if((pb >= 0 && pb < left_img->height) && (qb >= 0 && qb < left_img->width)) {

					/* Perform Bilinear Interpolation */
					hc = pb;
					wc = qb;

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
					if(base_h < left_img->height)
						base_hp1++;
					if(base_w < left_img->width)
						base_wp1++;


					/* Get the four surrounding pixels of input image */
					buffern1 = get_image_pixel_ptr(left_img, base_h, base_w);
					buffern2 = get_image_pixel_ptr(left_img, base_hp1, base_w);
					buffern3 = get_image_pixel_ptr(left_img, base_h, base_wp1);
					buffern4 = get_image_pixel_ptr(left_img, base_hp1, base_wp1);

					channel_count = 0;
					while(channel_count < output_img->bpp) {

						temp_val =  *(buffern1 + channel_count) * (1 - delta_h) * (1 - delta_w) + \
									*(buffern2 + channel_count) * (delta_h) * (1 - delta_w) + \
									*(buffern3 + channel_count) * (1 - delta_h) * (delta_w) + \
									*(buffern4 + channel_count) * (delta_w) * (delta_h);


						indicator_value = *(indicator + (j * output_img->width) + k);

						/* Check if some output already exists at the current location on the blackboard */
						if(indicator_value == 1) {

							/* Check how much do the images to be stitched differ at the current location */
							if(abs(temp_val - *(buffer + channel_count)) < 10) {
								*(buffer + channel_count) = (*(buffer + channel_count) + temp_val) / 2;
							}
						} else {
							*(buffer + channel_count) = temp_val;
						}

						++channel_count;
					}

					/* Note the pixel areas where a valid data is written */
					*(indicator + (j * output_img->width) + k) = 1;

			}

			++k;
		}
		++j;
	}
#endif


#ifdef STITCH_RIGHT /* Project right to  output */

	/* Inverse homography matrix to apply right image on blackboard output */
	H_rm[0][0] = 2.869143; 	H_rm[0][1] = -0.028158; 	H_rm[0][2] = -2855.702202;
	H_rm[1][0] = 0.516804;	H_rm[1][1] =  2.655502;   	H_rm[1][2] = -2706.169007;
	H_rm[2][0] = 0.001506;	H_rm[2][1] = -0.000043; 	H_rm[2][2] =     1;


	j = 0;
	while(j < output_img->height) {
		k = 0;
		while(k < output_img->width) {

			/* Convert from image coordinates to Euclidean coordinates */
			x = k + 0.5;
			y = output_img->height - 0.5 - j;

			/* Get the output blackboard pixel at the current location */
			buffer = (output_img->buffer + (j*output_img->width + k)*output_img->bpp);

			input[0][0] = x;
			input[1][0] = y;
			input[2][0] = 1;

			output = matrix_multiply_float(H_rm, dim_H, input, dim_input);

			/* Convert from homogenous coordinates to Euclidean coordinates */
			output[0][0] = output[0][0] / output[2][0];
			output[1][0] = output[1][0] / output[2][0];

			/* Convert from Euclidean coordinates to image coordinates */
			qb = (float)(output[0][0] - 0.5);
			pb = (float)(output[1][0]*-1 - 0.5 + right_img->height);


			/* Perform following actions only if pixels lie within bounds of right image */
			if((pb >= 0 && pb < right_img->height) && (qb >= 0 && qb < right_img->width)) {

					hc = pb;
					wc = qb;

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
					if(base_h < right_img->height)
						base_hp1++;
					if(base_w < right_img->width)
						base_wp1++;


					/* Get the four surrounding pixels of input image */
					buffern1 = get_image_pixel_ptr(right_img, base_h, base_w);
					buffern2 = get_image_pixel_ptr(right_img, base_hp1, base_w);
					buffern3 = get_image_pixel_ptr(right_img, base_h, base_wp1);
					buffern4 = get_image_pixel_ptr(right_img, base_hp1, base_wp1);

					channel_count = 0;
					while(channel_count < output_img->bpp) {

						if(base_h > 366) {
							lum_temp_val = LUMINANCE_VAL_TOP;
						} else {
							lum_temp_val = LUMINANCE_VAL_BOTTOM;
						}
						temp_buf1 = (int)((float)*(buffern1 + channel_count) * lum_temp_val);
						temp_buf2 = (int)((float)*(buffern2 + channel_count) * lum_temp_val);
						temp_buf3 = (int)((float)*(buffern3 + channel_count) * lum_temp_val);
						temp_buf4 = (int)((float)*(buffern4 + channel_count) * lum_temp_val);

						if(temp_buf1 > 255)
							temp_buf1 = 255;
						if(temp_buf2 > 255)
							temp_buf2 = 255;
						if(temp_buf3 > 255)
							temp_buf3 = 255;
						if(temp_buf4 > 255)
							temp_buf4 = 255;

						temp_val =  temp_buf1 * (1 - delta_h) * (1 - delta_w) + \
									temp_buf2 * (delta_h) * (1 - delta_w) + \
									temp_buf3 * (1 - delta_h) * (delta_w) + \
									temp_buf4 * (delta_w) * (delta_h);

						indicator_value = *(indicator + (j * output_img->width) + k);

						/* Check if some output already exists at the current location on the blackboard */
						if(indicator_value == 1) {

							/* Check how much do the images to be stitched differ at the current location */
							if(abs(temp_val - *(buffer + channel_count)) < 10) {
								*(buffer + channel_count) = (*(buffer + channel_count) + temp_val) / 2;
							}
						} else {
							*(buffer + channel_count) = temp_val;
						}

						++channel_count;
					}

					/* Note the pixel areas where a valid data is written */
					*(indicator + (j * output_img->width) + k) = 1;



			}

			++k;
		}
		++j;
	}
#endif

FREE_MEM:

	/* Free the indicator */
	if(indicator != NULL) {
		free(indicator);
		indicator = NULL;
	}

	/* Free the homography matrices */
	for(i = 0; i < 3; i++) {
		if(H_lm[i] != NULL) {
			free(H_lm[i]);
			H_lm[i] = NULL;
		}
	}
	free(H_lm);

	for(i = 0; i < 3; i++) {
		if(H_rm[i] != NULL) {
			free(H_rm[i]);
			H_rm[i] = NULL;
		}
	}
	free(H_rm);

	/* Free the input matrices */
	for(i = 0; i < 3; i++) {
		if(input[i] != NULL) {
			free(input[i]);
			input[i] = NULL;
		}
	}
	free(input);

	/* Free the output matrices */
	for(i = 0; i < 3; i++) {
		if(output[i] != NULL) {
			free(output[i]);
			output[i] = NULL;
		}
	}
	free(output);

	return output_img;
}


