/*
/* Name            : Ganesa Rukmani Ramdas Pillai
/* USC ID          : 9712700659
/* USC email       : gpillai@usc.edu
/* Submission Date : March 04, 2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#ifndef __COUNTING_GAME_H__
#define __COUNTING_GAME_H__

#include "common.h"
#include "utils.h"
#include "boundary_map.h"
#include "thresholding.h"


typedef struct list_objects {
	tagObject *obj;
	int count;
	int num;
	struct list_objects *next;

} list_objects;

typedef enum tagflip_type {
	FLIP_NONE,
	FLIP_HORIZONTALLY,
	FLIP_VERTICALLY
} tagflip_type;


/*
Function   : fetch_object_data
Input      : tagImageProp *input_img - Input Image
             uint8_t      *workboard - Workboard with multiple objects
             tagObject    *obj       - Current object information
Output     : None
Description: Fetch the current object from the workboard
*/
void fetch_object_data(tagImageProp *input_img, uint8_t *workboard, tagObject *obj);

/*
Function   : remove_obj_from_workboard
Input      : tagImageProp *input_img - Input Image
             uint8_t      *workboard - Workboard with multiple objects
             tagObject    *obj       - Current object information
Output     : None
Description: Remove the current object from workboard to avoid re-hit issues
*/
void remove_obj_from_workboard(tagImageProp *input_img, uint8_t *workboard, tagObject *obj);

/*
Function   : create_obj_list
Input      : tagObject *obj - Current object to be added to linked list
Output     : None
Description: Create the linked list with the input obj.
*/
void create_obj_list(tagObject *obj);

/*
Function   : get_degree_of_closeness
Input      : uint8_t *inp - buffer 1
             uint8_t *out - buffer 2
             int size     - size of buffer 1 and buffer 2
Output     : int - Percentage of closeness
Description: Find how close the buffers 1 and 2 are visually
*/
int get_degree_of_closeness(uint8_t *inp, uint8_t *out, int size);

/*
Function   : rotate_binary_image
Input      : tagObject *obj    - Input object information
             uint8_t   *input  - Input object
             uint8_t   *output - Output object
             int        angle  - Angle of rotation
Output     : None
Description: Rotate the object by 0, 90, 180, or 270 degrees
*/
void rotate_binary_image(tagObject *obj, uint8_t *input, uint8_t *output, int angle);

/*
Function   : flip_binary_image
Input      : tagObject   *obj    - Input object information
             uint8_t     *input  - Input object
             uint8_t     *output - Output object
             tagflip_type type   - Flip type
Output     : None
Description: Flip the object vertically or horizontally depending on type
*/
void flip_binary_image(tagObject *obj, uint8_t *input, uint8_t *output, tagflip_type type);

/*
Function   : compare_and_edit_existing_objs
Input      : tagObject *obj - Input object information
Output     : int - Status: -1: Object already exists in list
                            0: This is new object
Description: Compare the current object with existing objects in the linked lists
*/
int compare_and_edit_existing_objs(tagObject *obj);

/*
Function   : compare_and_add_to_object_list
Input      : tagObject *obj - Input object information
Output     : int - Status: -1: Object already exists in list
                            0: This is new object
Description: Compare the object with other list objects
             or create the list if this is first object.
*/
int compare_and_add_to_object_list(tagObject *obj);

/*
Function   : count_objects_binary
Input      : tagImageProp *input_img       - Input Image
             tagForegroundColor foreground
Output     :
Description:
*/
void count_objects_binary(tagImageProp *input_img, tagForegroundColor foreground);

#endif // #ifndef __COUNTING_GAME_H__
