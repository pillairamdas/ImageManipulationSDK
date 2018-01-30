#include "reflect.h"

/*
Function   : reflect_to_side
Input      : tagImageProp *input_img  - Input image
             tagImageProp *output_img - Output Image
Output     : None
Description: Perform reflection to the side on input image
*/
void reflect_to_side(tagImageProp *input_img, tagImageProp *output_img)
{
	int32_t      i              = 0;
	int32_t      j              = 0;
	int32_t      channel_count  = 0; 
	uint8_t *buffer         = NULL;


	while(i < input_img->height) {

		j = 0;
		while(j < input_img->width) {
			buffer = get_input_pixel_ptr(input_img, i, (input_img->width - j - 1));

			channel_count = 0;
			while(channel_count < input_img->bpp) {
				*(output_img->buffer + (i * output_img->width + j) * output_img->bpp + channel_count) = *(buffer + channel_count);
				++channel_count;
			}
			++j;
		}
		++i;
	}
}