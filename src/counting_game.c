/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include "counting_game.h"

//#define DEBUG_DUMP

/* Link list pointers */
list_objects *obj_head = NULL;
list_objects *obj_curr = NULL;
int node_num = 0;


/*
Function   : fetch_object_data
Input      : tagImageProp *input_img - Input Image
             uint8_t      *workboard - Workboard with multiple objects
             tagObject    *obj       - Current object information
Output     : None
Description: Fetch the current object from the workboard
*/
void fetch_object_data(tagImageProp *input_img, uint8_t *workboard, tagObject *obj)
{
	/* Temporary variables */
	int      iterator   = 0;
	uint8_t *buffer     = NULL;
	uint8_t *out_buffer = NULL;

	/* Image coordinates */
	int      vert       = obj->top_index;
	int      horiz      = obj->left_index;


	out_buffer = obj->buffer;


	while(iterator < obj->height) {

		buffer = workboard + (vert + iterator)* input_img->width + horiz;
		memcpy(out_buffer, buffer, obj->width * sizeof(uint8_t));
		out_buffer = out_buffer + obj->width;
		++iterator;
	}
}


/*
Function   : remove_obj_from_workboard
Input      : tagImageProp *input_img - Input Image
             uint8_t      *workboard - Workboard with multiple objects
             tagObject    *obj       - Current object information
Output     : None
Description: Remove the current object from workboard to avoid re-hit issues
*/
void remove_obj_from_workboard(tagImageProp *input_img, uint8_t *workboard, tagObject *obj)
{
	/* Temporary variables */
	int      iterator = 0;
	uint8_t *buffer   = NULL;

	/* Image coordinates */
	int vert  = obj->top_index;
	int horiz = obj->left_index;



	while(iterator < obj->height) {
		buffer = workboard + (vert + iterator)* input_img->width + horiz;
		memset(buffer, 0x00, obj->width * sizeof(uint8_t));
		++iterator;
	}
}


/*
Function   : create_obj_list
Input      : tagObject *obj - Current object to be added to linked list
Output     : None
Description: Create the linked list with the input obj.
*/
void create_obj_list(tagObject *obj)
{
	list_objects *list = (list_objects *) malloc(sizeof(list_objects));


	if(list != NULL) {

		list->obj   = obj;
		list->count = 1;
		++node_num;
		list->num   = node_num;
		list->next  = NULL;
		obj_head    = list;
		obj_curr    = list;

	} else {
		LOG(ERROR, "Memory unavailable!\n");
	}
}


/*
Function   : get_degree_of_closeness
Input      : uint8_t *inp - buffer 1
             uint8_t *out - buffer 2
             int size     - size of buffer 1 and buffer 2
Output     : int - Percentage of closeness
Description: Find how close the buffers 1 and 2 are visually
*/
int get_degree_of_closeness(uint8_t *inp, uint8_t *out, int size)
{
	int iterator = 0;
	int similar  = 0;
	int degree   = 0;

	while(iterator < size) {

		if(*(inp + iterator) == *(out + iterator)) {
			++similar;
		}
		++iterator;
	}

	degree = (similar * 100) / size;

	return degree;
}


/*
Function   : rotate_binary_image
Input      : tagObject *obj    - Input object information
             uint8_t   *input  - Input object
             uint8_t   *output - Output object
             int        angle  - Angle of rotation
Output     : None
Description: Rotate the object by 0, 90, 180, or 270 degrees
*/
void rotate_binary_image(tagObject *obj, uint8_t *input, uint8_t *output, int angle)
{
	/* Temporary variables */
	int      iterator   = 0;
	uint8_t *in_buffer  = NULL;
	uint8_t *out_buffer = NULL;

	/* object coordinate information*/
	int inp_vert  = 0;
	int inp_horiz = 0;
	int out_vert  = 0;
	int out_horiz = 0;
	int out_width = 0;


	while(iterator < obj->pixel_count) {

		inp_horiz = iterator % obj->width;
		inp_vert  = iterator / obj->width;

		in_buffer = input + inp_vert * obj->width + inp_horiz;

		switch(angle) {
			case 0: {
				out_vert = inp_vert;
				out_horiz = inp_horiz;
				out_width = obj->width;
			} break;
			case 90: {
				out_horiz = inp_vert;
				out_vert = obj->width - inp_horiz - 1;
				out_width = obj->height;
			} break;
			case 180: {
				out_horiz = obj->width - inp_horiz - 1;
				out_vert = obj->height - inp_vert - 1;
				out_width = obj->width;
			} break;
			case 270: {
				out_horiz = obj->height - inp_vert - 1;
				out_vert = inp_horiz;
				out_width = obj->height;
			} break;
			default: {
				LOG(ERROR, "Unsupported angle rotation\n");
			} break;
		}

		 out_buffer =  output + out_vert * out_width + out_horiz;
		*out_buffer = *in_buffer;

		++iterator;

	}
}


/*
Function   : flip_binary_image
Input      : tagObject   *obj    - Input object information
             uint8_t     *input  - Input object
             uint8_t     *output - Output object
             tagflip_type type   - Flip type
Output     : None
Description: Flip the object vertically or horizontally depending on type
*/
void flip_binary_image(tagObject *obj, uint8_t *input, uint8_t *output, tagflip_type type)
{
	/* Temporary variables */
	int      iterator   = 0;
	uint8_t *in_buffer  = NULL;
	uint8_t *out_buffer = NULL;

	/* object coordinate information*/
	int inp_vert  = 0;
	int inp_horiz = 0;
	int out_vert  = 0;
	int out_horiz = 0;
	int out_width = 0;


	while(iterator < obj->pixel_count) {

		inp_horiz = iterator % obj->width;
		inp_vert  = iterator / obj->width;

		in_buffer = input + inp_vert * obj->width + inp_horiz;


		switch(type) {
			case FLIP_VERTICALLY: {
				out_horiz = inp_horiz;
				out_vert  = obj->height - inp_vert - 1;
				out_width = obj->width;
			} break;
			case FLIP_HORIZONTALLY: {
				out_horiz = obj->width - inp_horiz - 1;
				out_vert  = inp_vert;
				out_width = obj->width;
			} break;
			case FLIP_NONE: {
				out_vert = inp_vert;
				out_horiz = inp_horiz;
				out_width = obj->width;
			} break;
			default: {
				LOG(ERROR, "Unsupported flip type\n");
			} break;
		}

		out_buffer = output + out_vert * out_width + out_horiz;
		*out_buffer = *in_buffer;

		++iterator;

	}
}


/*
Function   : compare_and_edit_existing_objs
Input      : tagObject *obj - Input object information
Output     : int - Status: -1: Object already exists in list
                            0: This is new object
Description: Compare the current object with existing objects in the linked lists
*/
int compare_and_edit_existing_objs(tagObject *obj)
{
	/* Temporary variables */
	int           i          = 0;
	int           iterator   = 0;
	int           flip_modes = 0;
	list_objects *node       = NULL;
	uint8_t      *temp_buf1  = NULL;
	uint8_t      *temp_buf2  = NULL;


	/* Object coordinate information */
	int angle = 0;
	int a_vert = 0;
	int b_vert = 0;

	/* Status return */
	int status = 0;

	/* Measure of closeness */
	int degree_of_closeness = 0;

	temp_buf1 = (uint8_t *)malloc(obj->size);
	if(temp_buf1 == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		status = -1;
		goto FREE_MEM;
	}

	temp_buf2 = (uint8_t *)malloc(obj->size);
	if(temp_buf2 == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		status = -1;
		goto FREE_MEM;
	}


	node = obj_head;

	while(node != NULL) {

		flip_modes = 0;
		while(flip_modes < 3) {


			/* Compare by flipping the object */
			flip_binary_image(obj, obj->buffer, temp_buf1, (tagflip_type) flip_modes);

			i = 0;
			while(i < 4) {

				angle = i * 90;

				/* Compare by rotating the objects */
				rotate_binary_image(obj, temp_buf1, temp_buf2, angle);


				/* Check how close the two objects are */
				degree_of_closeness = get_degree_of_closeness(temp_buf2, node->obj->buffer, obj->size);
				if(degree_of_closeness > 97) {
					++node->count;
					status = -1;
					goto FREE_MEM;
				}

				++i;
			}
			++flip_modes;
		}

		node = node->next;
	}

FREE_MEM:
	if(temp_buf2 != NULL) {
		free(temp_buf2);
		temp_buf2 = NULL;
	}

	if(temp_buf1 != NULL) {
		free(temp_buf1);
		temp_buf1 = NULL;
	}

	return status;
}


/*
Function   : compare_and_add_to_object_list
Input      : tagObject *obj - Input object information
Output     : int - Status: -1: Object already exists in list
                            0: This is new object
Description: Compare the object with other list objects
             or create the list if this is first object.
*/
int compare_and_add_to_object_list(tagObject *obj)
{
	int status = 0;


	if(obj_head == NULL) {
		create_obj_list(obj);
	} else {

		status = compare_and_edit_existing_objs(obj);
		if(status != 0) {
			return status;
		}

		list_objects *node = (list_objects *) malloc(sizeof(list_objects));
		if(node != NULL) {

			node->obj      = obj;
			node->count    = 1;
			++node_num;
			node->num      = node_num;
			node->next     = NULL;
			obj_curr->next = node;
			obj_curr       = node;

		} else {

			LOG(ERROR, "Memory unavailable!\n");
			status -1;

		}


	}

	return status;
}


/*
Function   : count_objects_binary
Input      : tagImageProp *input_img       - Input Image
             tagForegroundColor foreground
Output     :
Description:
*/
void count_objects_binary(tagImageProp *input_img, tagForegroundColor foreground)
{

	/* Temporary variables */
	int           iterator      = 0;
	list_objects *node          = NULL;
	uint8_t      *temp_buf      = NULL;
#ifdef DEBUG_DUMP
	uint8_t      *temp_buf2     = NULL;
	char          filename[80]  = {};
#endif

	/* object information handle */
	tagObject    *obj           = NULL;

	/* Image coordinates */
	int           horiz         = 0;
	int           vert          = 0;

	/* Status flag */
	int           status        = 0;

	/* Total count of objects */
	int           total_count   = 0;

	/* Workboard to operate on */
	uint8_t      *workboard     = NULL;

	/* Thresholded image */
	tagImageProp *threshold_img   = NULL;
	int           threshold_limit = 128;

	/* Allocate memory for Temporary buffer */
	threshold_img = (tagImageProp *) malloc(sizeof(tagImageProp));
	if(threshold_img == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	workboard = (uint8_t *) malloc(input_img->size);
	if(workboard == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}

	memcpy(threshold_img, input_img, sizeof(tagImageProp));

	threshold_img->buffer = (uint8_t *) malloc(input_img->size);
	if(threshold_img->buffer == NULL) {
		LOG(ERROR, "Memory unavailable!\n");
		goto FREE_MEM;
	}


	/* Threshold the input image using a limit of 127 */
	threshold_gray_image(input_img, threshold_img, threshold_limit);

	/* Binarize the thresholded image */
	binarise_input(threshold_img, workboard, foreground);


	while(iterator < input_img->pixel_count) {

		horiz = iterator % input_img->width;
		vert  = iterator / input_img->width;

		temp_buf = workboard + iterator;

		if(*temp_buf == 1) {

			obj = (tagObject *) malloc(sizeof(tagObject));
			if(obj == NULL) {
				LOG(ERROR, "Memory unavailable!\n");
				goto FREE_MEM;
			}

			/* Get the object information */
			boundary_map(input_img, workboard, obj, vert, horiz);

			/* Extract the object */
			obj->buffer = (uint8_t *) malloc(obj->size);
			if(obj->buffer == NULL) {
				LOG(ERROR, "Memory unavailable!\n");
				goto FREE_MEM;
			}
			fetch_object_data(input_img, workboard, obj);
			remove_obj_from_workboard(input_img, workboard, obj);

			/* Check if this object is already present in the linked list */
			status = compare_and_add_to_object_list(obj);
			if(status != 0) {
				if(obj != NULL) {
					if(obj->buffer != NULL) {
						free(obj->buffer);
						obj->buffer = NULL;
					}
					free(obj);
					obj = NULL;
				}
			}

		}
		++iterator;
	}

	node = obj_head;
	total_count = 0;
	while(node != NULL) {

#ifdef DEBUG_DUMP
		sprintf(filename, "subobject_%d_%d_%d.gray", node->num, node->obj->width, node->obj->height);
		printf("Creating file: subobject_%d_%d_%d.gray\n", node->num, node->obj->width, node->obj->height);
		iterator = 0;
		temp_buf2 = (uint8_t *) malloc(node->obj->size);

		if(foreground == FG_BLACK) {
			memset(temp_buf2, 0xFF, node->obj->size);
		}

		while(iterator < node->obj->pixel_count) {
			temp_buf = node->obj->buffer + iterator;
			if(foreground == FG_WHITE) {
				*(temp_buf2 + iterator) = *temp_buf * 255;
			} else {
				if(*temp_buf > 0) {
					*(temp_buf2 + iterator) = 0;
				}

			}

			++iterator;
		}

		dump_to_file(temp_buf2, node->obj->size, filename);
		free(temp_buf2);
#endif
		total_count += node->count;
		node = node->next;
	}

	LOG(OUTPUT, "Total: %d Unique: %d\n", total_count, node_num);

FREE_MEM:

	if(obj != NULL) {
		if(obj->buffer != NULL) {
			free(obj->buffer);
			obj->buffer = NULL;
		}
		free(obj);
		obj = NULL;
	}

	if(workboard != NULL) {
		free(workboard);
		workboard = NULL;
	}


	if(threshold_img != NULL) {
		if(threshold_img->buffer != NULL) {
			free(threshold_img->buffer);
			threshold_img->buffer = NULL;
		}
		free(threshold_img);
		threshold_img = NULL;
	}
}
