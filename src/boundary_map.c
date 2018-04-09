/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "boundary_map.h"

/*
Function   : find_next_obj_pixel
Input      : tagImageProp *input_img - Input image object
             uint8_t      *workboard - Workboard buffer
             int          *out_coord - Input and output cordinates of next pixel
             int           pointer   - Start location of pixel search
Output     : Location of next pixel
Description: Find the next pixel location by traversing the boundary
*/
int find_next_obj_pixel(tagImageProp *input_img, uint8_t *workboard, int *out_coord, int pointer)
{
	int i = 0;
	int j = 0;
	int iterator = 0;

	int vert = out_coord[0];
	int horiz = out_coord[1];

	uint8_t *buffer = NULL;
	while(iterator < 7) {

		if(pointer == 8) {
			pointer = 0;
		}
		switch(pointer) {
			case 0: {
				i = vert;
				j = horiz + 1;
			} break;
			case 1: {
				i = vert - 1;
				j = horiz + 1;
			} break;
			case 2: {
				i = vert - 1;
				j = horiz;
			} break;
			case 3: {
				i = vert - 1;
				j = horiz - 1;
			} break;
			case 4: {
				i = vert;
				j = horiz - 1;
			} break;
			case 5: {
				i = vert + 1;
				j = horiz - 1;
			} break;
			case 6: {
				i = vert + 1;
				j = horiz ;
			} break;
			case 7: {
				i = vert + 1;
				j = horiz + 1 ;
			} break;
			default: LOG(ERROR, "Unexpected index %d\n", pointer);
		}

		if((i < 0) || (i == input_img->height) || (j < 0) || (j == input_img->width)) {
			++pointer;
			continue;
		}

		buffer = workboard + i * input_img->width + j;
		if((*buffer & 0x1) == 1) {
			out_coord[0] = i;
			out_coord[1] = j;
			return pointer;
		}

		++pointer;
	}
}


/*
Function   : boundary_map
Input      : tagImageProp *input_img - Input Image
             uint8_t      *workboard - Workboard buffer
             tagObject    *obj       -
             int           vert
             int           horiz
Output     : None
Description: Find the objects by traversing the workboard
*/
void boundary_map(tagImageProp *input_img, uint8_t *workboard, tagObject *obj, int vert, int horiz)
{
	int trace_coord[2] = {0};
	int found_pointer  = 1;

	obj->top_index     = input_img->height;
	obj->left_index    = input_img->width;
	obj->bottom_index  = 0;
	obj->right_index   =  0;
	trace_coord[0]     = vert;
	trace_coord[1]     = horiz;



	while(1) {

		if(found_pointer == 8) {
			found_pointer = 0;
		}

		found_pointer = find_next_obj_pixel(input_img, workboard, trace_coord, found_pointer);


		if(found_pointer < 4) {
			found_pointer += 4;
		} else {
			found_pointer -= 4;
		}
		++found_pointer;

		if(obj->top_index > trace_coord[0]) {
			obj->top_index = trace_coord[0];
		}
		if(obj->bottom_index < trace_coord[0]) {
			obj->bottom_index = trace_coord[0];
		}
		if(obj->left_index > trace_coord[1]) {
			obj->left_index = trace_coord[1];
		}
		if(obj->right_index < trace_coord[1]) {
			obj->right_index = trace_coord[1];
		}

		if((trace_coord[0] == vert) && (trace_coord[1] == horiz)) {
			break;
		}
	}

	obj->width       = obj->right_index - obj->left_index + 1;
	obj->height      = obj->bottom_index - obj->top_index + 1;
	obj->pixel_count = obj->width * obj->height;
	obj->bpp         = input_img->bpp;
	obj->size        = obj->pixel_count * obj->bpp;

}
