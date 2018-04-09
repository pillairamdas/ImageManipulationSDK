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


typedef enum LOG_STATE {
    ERROR,
    OUTPUT,
    DEBUG
} LOG_STATE;


#define LOG(X, ...)						if((X == ERROR) && (DEBUG_LEVEL >= ERROR)) { \
											printf("\n[ERROR] %s: %s(%d): ", __FILE__, __func__, __LINE__); \
										} else if((X == DEBUG) && (DEBUG_LEVEL >= DEBUG)) { \
											printf("\n[DEBUG] %s: %s(%d): ", __FILE__, __func__, __LINE__); \
										} \
										if(X <= DEBUG_LEVEL) \
											printf(__VA_ARGS__);


#define RGB_BITS_PER_PIXEL				3
#define GRAY_BITS_PER_PIXEL             1
#define CMY_BITS_PER_PIXEL				3
#define MAX_PIXEL_LEVEL 				255
#define RGB_PIXEL_LEVELS				256
#define R_CHANNEL                       0
#define G_CHANNEL                       1
#define B_CHANNEL                       2
#define C_CHANNEL                       0
#define M_CHANNEL                       1
#define Y_CHANNEL                       2


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

/* Supported morphing types */
typedef enum type_morphing {
      SHRINKING     = 0x1,
      THINNING      = 0x2,
      SKELETONIZING = 0x4
} type_morphing;

/* Foreground color information */
typedef enum tagForegroundColor {
      FG_WHITE,
      FG_BLACK
} tagForegroundColor;

/* Supported methods to perform image dithering */
typedef enum tagDithertingMethod {
	DITHERING_FIXED_BINARY,
	DITHERING_FIXED_QUAD,
	DITHERING_RANDOM_BINARY,
	DITHERING_RANDOM_QUAD,
	DITHERING_BAYER_2_BINARY,
	DITHERING_BAYER_4_BINARY,
	DITHERING_BAYER_8_BINARY,
	DITHERING_BAYER_2_QUAD,
	DITHERING_BAYER_4_QUAD,
	DITHERING_BAYER_8_QUAD
} tagDithertingMethod;

/* Supported error diffusion methods */
typedef enum tagErrorDiffusionMethod {
	ERROR_DIFFUSE_FLYD_STEIN,
	ERROR_DIFFUSE_JJN,
	ERROR_DIFFUSE_STUCKI,
	ERROR_DIFFUSE_CUSTOM
} tagErrorDiffusionMethod;

/* Supported color halftoning methods */
typedef enum tagColorHalftoneMethod {
	COLOR_HALFTONE_SEPARABLE,
	COLOR_HALFTONE_MBVQ
} tagColorHalftoneMethod;


/* Image Properties */
typedef struct tagImageProp {
	int        height;
	int        width;
	uint8_t   *buffer;
	int 	   bpp;
    int        size;
    int        pixel_count;
} tagImageProp;

/* Object Properties */
typedef struct tagObject {
	int32_t 	top_index;
	int32_t 	left_index;
	int32_t 	right_index;
	int32_t 	bottom_index;
	int32_t 	pixel_count;
	int32_t 	height;
	int32_t 	width;
	int32_t 	bpp;
	int32_t 	size;
	uint8_t    *buffer;
} tagObject;


#endif // #ifndef __COMMON_H__