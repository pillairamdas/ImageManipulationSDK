CC = ${CROSS_COMPILE}gcc

CONV_GRAY_APP   =./bin/conv_gray_app
CONV_CMY_APP    =./bin/conv_cmy_app
RESIZE_APP      =./bin/resize_app
COLOR_CONV_APP  =./bin/color_conv_app
OIL_EFFECT_APP  = ./bin/oil_effect_app
CONTRAST_MANIP_APP = ./bin/contrast_manip_app
DENOISE_APP     = ./bin/denoise_app
FILM_EFFECT_APP = ./bin/film_effect_app


#Add -DPRINT_MATRIX to CFLAGS to print input file matrix on stdout
CFLAGS=-g 

COMMON_OBJS = 	utils.o \
				interface.o \
				format_conversion.o \
				resize.o \
				color_conversion.o \
				oil_effect.o \
				histogram_modification_transfer.o \
				filtering.o \
				histogram_equalization_uniform.o \
				reflect.o
				
CONV_GRAY_OBJS   = testapp_convert_gray.o
CONV_CMY_OBJS    = testapp_convert_cmy.o
RESIZE_OBJS      = testapp_resize.o
COLOR_CONV_OBJS  = testapp_color_palette_conv.o
OIL_EFFECT_OBJS  = testapp_oil_effect.o
CONTRAST_MANIP_OBJS = testapp_contrast_manipulation.o
DENOISE_OBJS    = testapp_denoise.o
FILM_EFFECT_OBJS = testapp_film_effect.o

INCLUDE=-I ./inc -I ./interface/inc

LIB=

LDFLAGS= -lm

vpath %.c ./src ./interface/src ./test 


all: ${CONV_GRAY_APP} ${CONV_CMY_APP} ${RESIZE_APP} ${COLOR_CONV_APP} ${OIL_EFFECT_APP} ${CONTRAST_MANIP_APP} ${DENOISE_APP} ${FILM_EFFECT_APP}
conv_gray:   ${CONV_GRAY_APP}
conv_cmy:    ${CONV_CMY_APP}
resize:      ${RESIZE_APP}
color_conv:  ${COLOR_CONV_APP}
oil_effect:  ${OIL_EFFECT_APP}
contrast_manipulation: ${CONTRAST_MANIP_APP}
denoise:     ${DENOISE_APP}
film_effect: ${FILM_EFFECT_APP}

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


