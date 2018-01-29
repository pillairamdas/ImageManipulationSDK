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


#ifndef __COMMON_H__
#define __COMMON_H__

#define DEBUG_LEVEL						1


// typedef enum LOG_STATE {
//     ERROR,
//     OUTPUT,
//     DEBUG
// } LOG_STATE;


// char LOG_STR[3][80] = {"ERROR", "OUTPUT", "DEBUG"};


// #define LOG(X, ...)                 if(X == OUTPUT) { \
//                                         printf(__VA_ARGS__); \
//                                     } else { \
//                                         if(X <= DEBUG_LEVEL) { \
//                                             printf("\n[%s] %s: %s(%d): ", LOG_STR[(int)X], __FILE__, __func__, __LINE__); \
//                                             printf(__VA_ARGS__); \
//                                         } \
//                                     }


#define RGB_BITS_PER_PIXEL				3
#define GRAY_BITS_PER_PIXEL             1
#define CMY_BITS_PER_PIXEL				3
#define MAX_PIXEL_LEVEL 				255
#define RGB_PIXEL_LEVELS				256
#define RED_CHANNEL                     0
#define GREEN_CHANNEL                   1
#define BLUE_CHANNEL                    2


typedef enum tagColorspace {
	COLORSPACE_NONE = -1,
	COLORSPACE_RGB = 0,
	COLORSPACE_GRAY_LIGHTNESS,
	COLORSPACE_GRAY_AVERAGE,
	COLORSPACE_GRAY_LUMINOSITY,
	COLORSPACE_CMY,
	COLORSPACE_CYAN,
	COLORSPACE_MAGENTA,
	COLORSPACE_YELLOW,
	COLORSPACE_NEGATIVE_RGB
} tagColorspace;

typedef enum tagEffects {
	EFFECT_NONE = -1,
	EFFECT_OIL_EFFECT = 0,
	EFFECT_FILM_EFFECT
} tagEffects;

typedef enum tagNoiseRemovalMethods {
	NOISE_REMOVE_MEDIAN_FILTER,
	NOISE_REMOVE_GAUSSIAN_FILTER,
	NOISE_REMOVE_LINEAR_FILTER
} tagNoiseRemovalMethods;

typedef enum tagHistModifications {
	HIST_EQUALIZE_TRANSFER_FUNCTION = 1,
	HIST_EQUALIZE_EQUAL_PROBABILITY,
	HIST_CUSTOM_TRANSFER_FUNCTION	
} tagHistModifications;

typedef enum tagStatus {
	ERROR_NONE = 0,
	ERROR_INCORRECT_COLOR_LEVEL = -1,
	ERROR_WINDOW_SIZE = -2,
	ERROR_INCORECT_OPTION
} tagStatus;


typedef enum tagFilterType {
	FILTER_GAUSSIAN,
	FILTER_LINEAR,
	FILTER_MEDIAN
} tagFilterType;



/* Image Properties */
typedef struct tagImageProp {
	int        height;
	int        width;
	uint8_t   *buffer;
	int 	   bpp;
    int        size;
    int        pixel_count;
} tagImageProp;




#endif // #ifndef __COMMON_H__