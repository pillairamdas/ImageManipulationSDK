CC = ${CROSS_COMPILE}gcc

CONV_GRAY_APP   	= ./bin/conv_gray_app
CONV_CMY_APP    	= ./bin/conv_cmy_app
RESIZE_APP      	= ./bin/resize_app
COLOR_CONV_APP  	= ./bin/color_conv_app
OIL_EFFECT_APP  	= ./bin/oil_effect_app
CONTRAST_MANIP_APP  = ./bin/contrast_manip_app
DENOISE_APP     	= ./bin/denoise_app
FILM_EFFECT_APP 	= ./bin/film_effect_app
REFLECT_APP         = ./bin/reflect_app
WARPING_APP         = ./bin/warp_app
STITCH_APP          = ./bin/stitch_app
DITHER_APP			= ./bin/dither_app
ERR_DIFFUSE_APP     = ./bin/err_diffuse_app
COLOR_HT_APP        = ./bin/color_halftone_app
SHRINKING_APP       = ./bin/shrink_app
THINNING_APP        = ./bin/thinning_app
SKELETONIZE_APP     = ./bin/skeletonize_app
COUNT_GAME_APP      = ./bin/counting_game_app

CFLAGS=-g 

COMMON_OBJS = 	      utils.o \
					  data.o \
				      interface.o \
				      warping.o \
				      format_conversion.o \
				      resize.o \
				      color_conversion.o \
				      oil_effect.o \
				      histogram_modification_transfer.o \
				      filtering.o \
				      histogram_equalization_uniform.o \
				      reflect.o \
				      stitching.o \
				      thresholding.o \
				      dithering.o \
				      error_diffusion.o \
				      color_halftoning.o \
				      boundary_map.o \
				      morphing.o \
				      counting_game.o				      
				
CONV_GRAY_OBJS   	= testapp_convert_gray.o
CONV_CMY_OBJS    	= testapp_convert_cmy.o
RESIZE_OBJS      	= testapp_resize.o
COLOR_CONV_OBJS  	= testapp_color_palette_conv.o
OIL_EFFECT_OBJS  	= testapp_oil_effect.o
CONTRAST_MANIP_OBJS = testapp_contrast_manipulation.o
DENOISE_OBJS    	= testapp_denoise.o
FILM_EFFECT_OBJS 	= testapp_film_effect.o
REFLECT_OBJS        = testapp_reflect.o
WARPING_OBJS        = testapp_warping.o
STITCH_OBJS         = testapp_stitching.o    
DITHER_OBJS         = testapp_dithering.o
ERR_DIFFUSE_OBJS    = testapp_error_diffusion.o                   
COLOR_HT_OBJS       = testapp_color_halftoning.o
SHRINKING_OBJS      = testapp_shrinking.o
THINNING_OBJS       = testapp_thinning.o
SKELETONIZE_OBJS    = testapp_skeletonizing.o
COUNT_GAME_OBJS     = testapp_counting_game.o

INCLUDE=-I ./inc -I ./interface/inc

LIB=

LDFLAGS= -lm

vpath %.c ./src ./interface/src ./test 


all: ${CONV_GRAY_APP} ${CONV_CMY_APP} ${RESIZE_APP} ${COLOR_CONV_APP} ${OIL_EFFECT_APP} \
	 ${CONTRAST_MANIP_APP} ${DENOISE_APP} ${FILM_EFFECT_APP} ${REFLECT_APP} ${WARPING_APP} \
	 ${DITHER_APP} ${ERR_DIFFUSE_APP} ${COLOR_HT_APP} ${SHRINKING_APP} ${THINNING_APP} \
	 ${SKELETONIZE_APP} ${COUNT_GAME_APP} ${STITCH_APP}

conv_gray:   		   ${CONV_GRAY_APP}
conv_cmy:    		   ${CONV_CMY_APP}
resize:      		   ${RESIZE_APP}
color_conv:  		   ${COLOR_CONV_APP}
oil_effect:  		   ${OIL_EFFECT_APP}
contrast_manipulation: ${CONTRAST_MANIP_APP}
denoise:               ${DENOISE_APP}
film_effect:           ${FILM_EFFECT_APP}
reflect:			   ${REFLECT_APP}
warp:                  ${WARPING_APP}

.c.o:
	@${CC} ${CFLAGS} ${INCLUDE} -c $< ${LIB} ${LDFLAGS} -o $@
	@echo [CC] $@

${CONV_GRAY_APP}:${COMMON_OBJS} ${CONV_GRAY_OBJS} 
	@${CC} ${COMMON_OBJS} ${CONV_GRAY_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${CONV_CMY_APP}:${COMMON_OBJS} ${CONV_CMY_OBJS}
	@${CC} ${COMMON_OBJS} ${CONV_CMY_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${RESIZE_APP}:${COMMON_OBJS} ${RESIZE_OBJS}
	@${CC} ${COMMON_OBJS} ${RESIZE_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${COLOR_CONV_APP}:${COMMON_OBJS} ${COLOR_CONV_OBJS}
	@${CC} ${COMMON_OBJS} ${COLOR_CONV_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${OIL_EFFECT_APP}:${COMMON_OBJS} ${OIL_EFFECT_OBJS}
	@${CC} ${COMMON_OBJS} ${OIL_EFFECT_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${CONTRAST_MANIP_APP}:${COMMON_OBJS} ${CONTRAST_MANIP_OBJS}
	@${CC} ${COMMON_OBJS} ${CONTRAST_MANIP_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${DENOISE_APP}:${COMMON_OBJS} ${DENOISE_OBJS}
	@${CC} ${COMMON_OBJS} ${DENOISE_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${FILM_EFFECT_APP}:${COMMON_OBJS} ${FILM_EFFECT_OBJS}
	@${CC} ${COMMON_OBJS} ${FILM_EFFECT_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${REFLECT_APP}:${COMMON_OBJS} ${REFLECT_OBJS}
	@${CC} ${COMMON_OBJS} ${REFLECT_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${WARPING_APP}:${COMMON_OBJS} ${WARPING_OBJS}
	@${CC} ${COMMON_OBJS} ${WARPING_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${STITCH_APP}:${COMMON_OBJS} ${STITCH_OBJS}
	@${CC} ${COMMON_OBJS} ${STITCH_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${DITHER_APP}:${COMMON_OBJS} ${DITHER_OBJS}
	@${CC} ${COMMON_OBJS} ${DITHER_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${ERR_DIFFUSE_APP}:${COMMON_OBJS} ${ERR_DIFFUSE_OBJS}
	@${CC} ${COMMON_OBJS} ${ERR_DIFFUSE_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@	

${COLOR_HT_APP}:${COMMON_OBJS} ${COLOR_HT_OBJS}
	@${CC} ${COMMON_OBJS} ${COLOR_HT_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${SHRINKING_APP}:${COMMON_OBJS} ${SHRINKING_OBJS}
	@${CC} ${COMMON_OBJS} ${SHRINKING_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${THINNING_APP}:${COMMON_OBJS} ${THINNING_OBJS}
	@${CC} ${COMMON_OBJS} ${THINNING_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${SKELETONIZE_APP}:${COMMON_OBJS} ${SKELETONIZE_OBJS}
	@${CC} ${COMMON_OBJS} ${SKELETONIZE_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

${COUNT_GAME_APP}:${COMMON_OBJS} ${COUNT_GAME_OBJS}
	@${CC} ${COMMON_OBJS} ${COUNT_GAME_OBJS} ${CFLAGS} ${INCLUDE} -o $@ $(LIB) ${LDFLAGS}
	@echo [CC] $@

clean:
	find . -type f | xargs touch
	@rm -rf *.o
	@rm -rf ${CONV_GRAY_APP} 
	@echo [RM] ${CONV_GRAY_APP} 
	@rm -rf ${CONV_CMY_APP} 
	@echo [RM] ${CONV_CMY_APP} 
	@rm -rf ${RESIZE_APP}
	@echo [RM] ${RESIZE_APP}
	@rm -rf ${COLOR_CONV_APP}
	@echo [RM] ${COLOR_CONV_APP}
	@rm -rf ${OIL_EFFECT_APP}
	@echo [RM] ${OIL_EFFECT_APP}
	@rm -rf ${CONTRAST_MANIP_APP}
	@echo [RM] ${CONTRAST_MANIP_APP}
	@rm -rf ${DENOISE_APP}
	@echo [RM] ${DENOISE_APP}
	@rm -rf ${FILM_EFFECT_APP}
	@echo [RM] ${FILM_EFFECT_APP}
	@rm -rf ${REFLECT_APP}
	@echo [RM] ${REFLECT_APP}
	@rm -rf ${WARPING_APP}
	@echo [RM] ${WARPING_APP}	
	@rm -rf ${STITCH_APP}
	@echo [RM] ${STITCH_APP}
	@rm -rf ${DITHER_APP}
	@echo [RM] ${DITHER_APP}	
	@rm -rf ${ERR_DIFFUSE_APP}
	@echo [RM] ${ERR_DIFFUSE_APP}
	@rm -rf ${COLOR_HT_APP}
	@echo [RM] ${COLOR_HT_APP}	
	@rm -rf ${SHRINKING_APP}
	@echo [RM] ${SHRINKING_APP}
	@rm -rf ${THINNING_APP}
	@echo [RM] ${THINNING_APP}
	@rm -rf ${SKELETONIZE_APP}
	@echo [RM] ${SKELETONIZE_APP}
	@rm -rf ${COUNT_GAME_APP}
	@echo [RM] ${COUNT_GAME_APP}	

