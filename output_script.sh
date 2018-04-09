#!/bin/sh
clear
cp -fr data/* bin/
sync
cd bin/

loc=$PWD
rm -fr Output

cd $loc

rm -fr *.gray
rm -fr *.rgb
rm -fr *.jpg

mkdir Output
mkdir Output/1_warping
echo
echo "Running warp_app for puppy.raw"
cp -fr puppy.raw puppy.rgb
convert -size 512x512 -depth 8 puppy.rgb Output/1_warping/puppy.jpg
./warp_app puppy.raw 512 512 3  
sync
mv -f OUT_warped.raw OUT_warped.rgb
mv -f OUT_dewarped.raw OUT_dewarped.rgb
convert -size 512x512 -depth 8 OUT_warped.rgb Output/1_warping/OUT_warped_puppy.jpg
convert -size 512x512 -depth 8 OUT_dewarped.rgb Output/1_warping/OUT_dewarped_puppy.jpg


echo "Running warp_app for tiger.raw"
cp -fr tiger.raw tiger.rgb
convert -size 512x512 -depth 8 tiger.rgb Output/1_warping/tiger.jpg
./warp_app tiger.raw 512 512 3 
sync
mv -f OUT_warped.raw OUT_warped.rgb
mv -f OUT_dewarped.raw OUT_dewarped.rgb
convert -size 512x512 -depth 8 OUT_warped.rgb Output/1_warping/OUT_warped_tiger.jpg
convert -size 512x512 -depth 8 OUT_dewarped.rgb Output/1_warping/OUT_dewarped_tiger.jpg


echo "Running warp_app for panda.raw"
cp -fr panda.raw panda.rgb
convert -size 512x512 -depth 8 panda.rgb Output/1_warping/panda.jpg
./warp_app panda.raw 512 512 3 
sync
mv -f OUT_warped.raw OUT_warped.rgb
mv -f OUT_dewarped.raw OUT_dewarped.rgb
convert -size 512x512 -depth 8 OUT_warped.rgb Output/1_warping/OUT_warped_panda.jpg
convert -size 512x512 -depth 8 OUT_dewarped.rgb Output/1_warping/OUT_dewarped_panda.jpg
rm -fr *.rgb

cd $loc
mkdir Output/2_stitching
echo "Running stitch_app for left.raw middle.raw right.raw"
cp -fr left.raw left.rgb
convert -size 480x640 -depth 8 left.rgb Output/2_stitching/left.jpg
cp -fr middle.raw middle.rgb
convert -size 480x640 -depth 8 middle.rgb Output/2_stitching/middle.jpg
cp -fr right.raw right.rgb
convert -size 480x640 -depth 8 right.rgb Output/2_stitching/right.jpg
./stitch_app left.raw middle.raw right.raw 480 640 3 
sync
mv -f OUT_stitched.raw OUT_stitched.rgb
convert -size 2000x2000 -depth 8 OUT_stitched.rgb Output/2_stitching/OUT_stitched.jpg
rm -fr *.rgb

cd $loc
mkdir Output/3_Dithering
echo "Running dither_app for colorchecker.raw"
cp -fr colorchecker.raw colorchecker.gray
convert -size 512x512 -depth 8 colorchecker.gray Output/3_Dithering/colorchecker.jpg
./dither_app colorchecker.raw 512 512 1 
sync
mv -f OUT_dither_fixed_binary.raw OUT_dither_fixed_binary.gray
convert -size 512x512 -depth 8 OUT_dither_fixed_binary.gray Output/3_Dithering/OUT_dither_fixed_binary.jpg
mv -f OUT_dither_fixed_quad.raw OUT_dither_fixed_quad.gray
convert -size 512x512 -depth 8 OUT_dither_fixed_quad.gray Output/3_Dithering/OUT_dither_fixed_quad.jpg
mv -f OUT_dither_random_binary.raw OUT_dither_random_binary.gray
convert -size 512x512 -depth 8 OUT_dither_random_binary.gray Output/3_Dithering/OUT_dither_random_binary.jpg
mv -f OUT_dither_random_quad.raw OUT_dither_random_quad.gray
convert -size 512x512 -depth 8 OUT_dither_random_quad.gray Output/3_Dithering/OUT_dither_random_quad.jpg
mv -f OUT_dither_bayer_2_binary.raw OUT_dither_bayer_2_binary.gray
convert -size 512x512 -depth 8 OUT_dither_bayer_2_binary.gray Output/3_Dithering/OUT_dither_bayer_2_binary.jpg
mv -f OUT_dither_bayer_4_binary.raw OUT_dither_bayer_4_binary.gray
convert -size 512x512 -depth 8 OUT_dither_bayer_4_binary.gray Output/3_Dithering/OUT_dither_bayer_4_binary.jpg
mv -f OUT_dither_bayer_8_binary.raw OUT_dither_bayer_8_binary.gray
convert -size 512x512 -depth 8 OUT_dither_bayer_8_binary.gray Output/3_Dithering/OUT_dither_bayer_8_binary.jpg
mv -f OUT_dither_bayer_2_QUAD.raw OUT_dither_bayer_2_QUAD.gray
convert -size 512x512 -depth 8 OUT_dither_bayer_2_QUAD.gray Output/3_Dithering/OUT_dither_bayer_2_QUAD.jpg
mv -f OUT_dither_bayer_4_QUAD.raw OUT_dither_bayer_4_QUAD.gray
convert -size 512x512 -depth 8 OUT_dither_bayer_4_QUAD.gray Output/3_Dithering/OUT_dither_bayer_4_QUAD.jpg
mv -f OUT_dither_bayer_8_QUAD.raw OUT_dither_bayer_8_QUAD.gray
convert -size 512x512 -depth 8 OUT_dither_bayer_8_QUAD.gray Output/3_Dithering/OUT_dither_bayer_8_QUAD.jpg
rm -fr *.gray

cd $loc
mkdir Output/4_ErrorDiffusion
echo "Running error diffusion app for colorchecker.raw"
cp -fr colorchecker.raw colorchecker.gray
convert -size 512x512 -depth 8 colorchecker.gray Output/4_ErrorDiffusion/colorchecker.jpg
./err_diffuse_app colorchecker.raw 512 512 1 
sync
mv -f OUT_error_diffuse_floyd_stein.raw OUT_error_diffuse_floyd_stein.gray
convert -size 512x512 -depth 8 OUT_error_diffuse_floyd_stein.gray Output/4_ErrorDiffusion/OUT_error_diffuse_floyd_stein.jpg
mv -f OUT_error_diffuse_jjn.raw OUT_error_diffuse_jjn.gray
convert -size 512x512 -depth 8 OUT_error_diffuse_jjn.gray Output/4_ErrorDiffusion/OUT_error_diffuse_jjn.jpg
mv -f OUT_error_diffuse_stucki.raw OUT_error_diffuse_stucki.gray
convert -size 512x512 -depth 8 OUT_error_diffuse_stucki.gray Output/4_ErrorDiffusion/OUT_error_diffuse_stucki.jpg
mv -f OUT_error_diffuse_custom.raw OUT_error_diffuse_custom.gray
convert -size 512x512 -depth 8 OUT_error_diffuse_custom.gray Output/4_ErrorDiffusion/OUT_error_diffuse_custom.jpg

cd $loc
mkdir Output/5_ColorHalftoning
echo "Running color halftoning app for flower.raw"
cp -fr flower.raw flower.rgb
convert -size 700x700 -depth 8 flower.rgb Output/5_ColorHalftoning/flower.jpg
./color_halftone_app flower.raw 700 700 3 
sync
mv -f OUT_color_halftone_separable.raw OUT_color_halftone_separable.rgb
convert -size 700x700 -depth 8 OUT_color_halftone_separable.rgb Output/5_ColorHalftoning/OUT_color_halftone_separable.jpg
mv -f OUT_color_halftone_mbvq.raw OUT_color_halftone_mbvq.rgb
convert -size 700x700 -depth 8 OUT_color_halftone_mbvq.rgb Output/5_ColorHalftoning/OUT_color_halftone_mbvq.jpg


cd $loc
mkdir Output/6_Shrinking
echo "Running shrink_app for stars.raw"
cp -fr stars.raw stars.gray
convert -size 640x480 -depth 8 stars.gray Output/6_Shrinking/stars.jpg
./shrink_app stars.raw 640 480 1 0
mv -f OUT_shrinked.raw OUT_shrinked.gray
convert -size 640x480 -depth 8 OUT_shrinked.gray Output/6_Shrinking/OUT_shrinked.jpg
if [ -f "shrinking_13.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_13.gray Output/6_Shrinking/shrinking_13.jpg
fi
if [ -f "shrinking_12.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_12.gray Output/6_Shrinking/shrinking_12.jpg
fi
if [ -f "shrinking_11.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_11.gray Output/6_Shrinking/shrinking_11.jpg
fi
if [ -f "shrinking_10.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_10.gray Output/6_Shrinking/shrinking_10.jpg
fi
if [ -f "shrinking_9.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_9.gray Output/6_Shrinking/shrinking_9.jpg
fi
if [ -f "shrinking_8.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_8.gray Output/6_Shrinking/shrinking_8.jpg
fi
if [ -f "shrinking_7.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_7.gray Output/6_Shrinking/shrinking_7.jpg
fi
if [ -f "shrinking_6.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_6.gray Output/6_Shrinking/shrinking_6.jpg
fi
if [ -f "shrinking_5.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_5.gray Output/6_Shrinking/shrinking_5.jpg
fi
if [ -f "shrinking_4.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_4.gray Output/6_Shrinking/shrinking_4.jpg
fi
if [ -f "shrinking_3.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_3.gray Output/6_Shrinking/shrinking_3.jpg
fi
if [ -f "shrinking_2.gray" ]; then
	convert -size 640x480 -depth 8 shrinking_2.gray Output/6_Shrinking/shrinking_2.jpg
fi
cd $loc
mkdir Output/7_Thinning
echo "Running thinning_app for jigsaw_1.raw"
cp -fr jigsaw_1.raw jigsaw_1.gray
convert -size 100x100 -depth 8 jigsaw_1.gray Output/7_Thinning/jigsaw_1.jpg
./thinning_app jigsaw_1.raw 100 100 1 1 
mv -f OUT_thinned.raw OUT_thinned.gray
convert -size 100x100 -depth 8 OUT_thinned.gray Output/7_Thinning/OUT_thinned.jpg
if [ -f "thinning_20.gray" ]; then
	convert -size 100x100 -depth 8 thinning_20.gray Output/7_Thinning/thinning_20.jpg
fi
if [ -f "thinning_19.gray" ]; then
	convert -size 100x100 -depth 8 thinning_19.gray Output/7_Thinning/thinning_19.jpg
fi
if [ -f "thinning_18.gray" ]; then
	convert -size 100x100 -depth 8 thinning_18.gray Output/7_Thinning/thinning_18.jpg
fi
if [ -f "thinning_17.gray" ]; then
	convert -size 100x100 -depth 8 thinning_17.gray Output/7_Thinning/thinning_17.jpg
fi
if [ -f "thinning_16.gray" ]; then
	convert -size 100x100 -depth 8 thinning_16.gray Output/7_Thinning/thinning_16.jpg
fi
if [ -f "thinning_15.gray" ]; then
	convert -size 100x100 -depth 8 thinning_15.gray Output/7_Thinning/thinning_15.jpg
fi
if [ -f "thinning_14.gray" ]; then
	convert -size 100x100 -depth 8 thinning_14.gray Output/7_Thinning/thinning_14.jpg
fi
if [ -f "thinning_13.gray" ]; then
	convert -size 100x100 -depth 8 thinning_13.gray Output/7_Thinning/thinning_13.jpg
fi
if [ -f "thinning_12.gray" ]; then
	convert -size 100x100 -depth 8 thinning_12.gray Output/7_Thinning/thinning_12.jpg
fi
if [ -f "thinning_11.gray" ]; then
	convert -size 100x100 -depth 8 thinning_11.gray Output/7_Thinning/thinning_11.jpg
fi
if [ -f "thinning_10.gray" ]; then
	convert -size 100x100 -depth 8 thinning_10.gray Output/7_Thinning/thinning_10.jpg
fi
if [ -f "thinning_9.gray" ]; then
	convert -size 100x100 -depth 8 thinning_9.gray Output/7_Thinning/thinning_9.jpg
fi
if [ -f "thinning_8.gray" ]; then
	convert -size 100x100 -depth 8 thinning_8.gray Output/7_Thinning/thinning_8.jpg
fi
if [ -f "thinning_7.gray" ]; then
	convert -size 100x100 -depth 8 thinning_7.gray Output/7_Thinning/thinning_7.jpg
fi
if [ -f "thinning_6.gray" ]; then
	convert -size 100x100 -depth 8 thinning_6.gray Output/7_Thinning/thinning_6.jpg
fi
if [ -f "thinning_5.gray" ]; then
	convert -size 100x100 -depth 8 thinning_5.gray Output/7_Thinning/thinning_5.jpg
fi
if [ -f "thinning_4.gray" ]; then
	convert -size 100x100 -depth 8 thinning_4.gray Output/7_Thinning/thinning_4.jpg
fi
if [ -f "thinning_3.gray" ]; then
	convert -size 100x100 -depth 8 thinning_3.gray Output/7_Thinning/thinning_3.jpg
fi
if [ -f "thinning_2.gray" ]; then
	convert -size 100x100 -depth 8 thinning_2.gray Output/7_Thinning/thinning_2.jpg
fi
cd $loc
mkdir Output/8_Skeletonization
echo "Running shrink_app for jigsaw_2.raw"
cp -fr jigsaw_2.raw jigsaw_2.gray
convert -size 100x100 -depth 8 jigsaw_2.gray Output/8_Skeletonization/jigsaw_2.jpg
./skeletonize_app jigsaw_2.raw 100 100 1 1 
mv -f OUT_skeltonized.raw OUT_skeltonized.gray
convert -size 100x100 -depth 8 OUT_skeltonized.gray Output/8_Skeletonization/OUT_skeltonized.jpg
if [ -f "skeletonizing_15.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_15.gray Output/8_Skeletonization/skeletonizing_15.jpg
fi
if [ -f "skeletonizing_14.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_14.gray Output/8_Skeletonization/skeletonizing_14.jpg
fi
if [ -f "skeletonizing_13.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_13.gray Output/8_Skeletonization/skeletonizing_13.jpg
fi
if [ -f "skeletonizing_12.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_12.gray Output/8_Skeletonization/skeletonizing_12.jpg
fi
if [ -f "skeletonizing_11.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_11.gray Output/8_Skeletonization/skeletonizing_11.jpg
fi
if [ -f "skeletonizing_10.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_10.gray Output/8_Skeletonization/skeletonizing_10.jpg
fi
if [ -f "skeletonizing_9.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_9.gray Output/8_Skeletonization/skeletonizing_9.jpg
fi
if [ -f "skeletonizing_8.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_8.gray Output/8_Skeletonization/skeletonizing_8.jpg
fi
if [ -f "skeletonizing_7.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_7.gray Output/8_Skeletonization/skeletonizing_7.jpg
fi
if [ -f "skeletonizing_6.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_6.gray Output/8_Skeletonization/skeletonizing_6.jpg
fi
if [ -f "skeletonizing_5.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_5.gray Output/8_Skeletonization/skeletonizing_5.jpg
fi
if [ -f "skeletonizing_4.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_4.gray Output/8_Skeletonization/skeletonizing_4.jpg
fi
if [ -f "skeletonizing_3.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_3.gray Output/8_Skeletonization/skeletonizing_3.jpg
fi
if [ -f "skeletonizing_2.gray" ]; then
	convert -size 100x100 -depth 8 skeletonizing_2.gray Output/8_Skeletonization/skeletonizing_2.jpg
fi
cd $loc
mkdir Output/9_CountGame
echo "Running counting_game_app for board.raw"
cp -fr board.raw board.gray
convert -size 372x372 -depth 8 board.gray Output/9_CountGame/board.jpg
./counting_game_app board.raw 372 372 1 1
sync
if [ -f "subobject_1_46_62.gray" ]; then
	convert -size 46x62 -depth 8 subobject_1_46_62.gray Output/9_CountGame/subobject_1_46_62.jpg
fi
if [ -f "subobject_2_46_62.gray" ]; then
	convert -size 46x62 -depth 8 subobject_2_46_62.gray Output/9_CountGame/subobject_2_46_62.jpg
fi
if [ -f "subobject_3_46_78.gray" ]; then
	convert -size 46x78 -depth 8 subobject_3_46_78.gray Output/9_CountGame/subobject_3_46_78.jpg
fi
if [ -f "subobject_4_62_62.gray" ]; then
	convert -size 62x62 -depth 8 subobject_4_62_62.gray Output/9_CountGame/subobject_4_62_62.jpg
fi
if [ -f "subobject_5_46_46.gray" ]; then
	convert -size 46x46 -depth 8 subobject_5_46_46.gray Output/9_CountGame/subobject_5_46_46.jpg
fi
if [ -f "subobject_6_46_62.gray" ]; then
	convert -size 46x62 -depth 8 subobject_6_46_62.gray Output/9_CountGame/subobject_6_46_62.jpg
fi
if [ -f "subobject_7_62_62.gray" ]; then
	convert -size 62x62 -depth 8 subobject_7_62_62.gray Output/9_CountGame/subobject_7_62_62.jpg
fi
if [ -f "subobject_8_46_46.gray" ]; then
	convert -size 46x46 -depth 8 subobject_8_46_46.gray Output/9_CountGame/subobject_8_46_46.jpg
fi
if [ -f "subobject_9_46_62.gray" ]; then
	convert -size 46x62 -depth 8 subobject_9_46_62.gray Output/9_CountGame/subobject_9_46_62.jpg
fi
if [ -f "subobject_10_46_78.gray" ]; then
	convert -size 46x78 -depth 8 subobject_10_46_78.gray Output/9_CountGame/subobject_10_46_78.jpg
fi

echo "Running oil_effect_app for Barn.raw"
cd $loc
mkdir Output/10_OilEffect
cp -fr Barn.raw Barn.rgb
convert -size 380x275 -depth 8 Barn.rgb Output/10_OilEffect/Barn.jpg
./oil_effect_app Barn.raw 380 275 3 5 64
mv -f OUT_oil_effect.raw OUT_oil_effect.rgb
convert -size 380x275 -depth 8 OUT_oil_effect.rgb Output/10_OilEffect/OUT_oil_effect.jpg


echo "Enhancing contrast of Desk.raw"
cd $loc
mkdir Output/11_ContrastEnhancement
cp -fr Desk.raw Desk.rgb
convert -size 400x300 -depth 8 Desk.rgb Output/11_ContrastEnhancement/Desk.jpg
./contrast_manip_app Desk.raw 400 300 3 transfer
mv -f OUT_Contrast_Enhanced.raw OUT_Contrast_Enhanced.rgb
convert -size 400x300 -depth 8 OUT_Contrast_Enhanced.rgb Output/11_ContrastEnhancement/OUT_Contrast_Enhanced_transfer.jpg
./contrast_manip_app Desk.raw 400 300 3 uniform
mv -f OUT_Contrast_Enhanced.raw OUT_Contrast_Enhanced.rgb
convert -size 400x300 -depth 8 OUT_Contrast_Enhanced.rgb Output/11_ContrastEnhancement/OUT_Contrast_Enhanced_uniform.jpg



echo "Denoising Lena_mixed.raw"
cd $loc
mkdir Output/12_Denoising
cp -fr Lena.raw Lena.rgb
convert -size 512x512 -depth 8 Lena.rgb Output/12_Denoising/Lena.jpg
cp -fr Lena_mixed.raw Lena_mixed.rgb
convert -size 512x512 -depth 8 Lena_mixed.rgb Output/12_Denoising/Lena_mixed.jpg

./denoise_app Lena_mixed.raw Lena.raw 512 512 3 5
mv -f OUT_DENOISED_Median_Gaussian.raw OUT_DENOISED_Median_Gaussian.rgb
convert -size 512x512 -depth 8 OUT_DENOISED_Median_Gaussian.rgb Output/12_Denoising/OUT_DENOISED_Median_Gaussian.jpg
mv -f OUT_DENOISED_Median_Linear.raw OUT_DENOISED_Median_Linear.rgb
convert -size 512x512 -depth 8 OUT_DENOISED_Median_Linear.rgb Output/12_Denoising/OUT_DENOISED_Median_Linear.jpg
mv -f OUT_DENOISED_Median.raw OUT_DENOISED_Median.rgb
convert -size 512x512 -depth 8 OUT_DENOISED_Median.rgb Output/12_Denoising/OUT_DENOISED_Median.jpg

echo "Resizing Airplane.raw"
cd $loc
mkdir Output/13_Resize
cp -fr Airplane.raw Airplane.rgb
convert -size 512x512 -depth 8 Airplane.rgb Output/13_Resize/Airplane.jpg
./resize_app Airplane.raw 512 512 3 650 650
mv -f OUT_resize.raw OUT_resize.rgb
convert -size 650x650 -depth 8 OUT_resize.rgb Output/13_Resize/OUT_resize.jpg


echo "Getting film effect on Original.raw"
cd $loc
mkdir Output/14_FilmEffect
cp -fr Original.raw Original.rgb
convert -size 1000x750 -depth 8 Original.rgb Output/14_FilmEffect/Original.jpg
./film_effect_app Original.raw 1000 750 3
mv -f OUT_film_effect.raw OUT_film_effect.rgb
convert -size 1000x750 -depth 8 OUT_film_effect.rgb Output/14_FilmEffect/OUT_film_effect.jpg



rm -fr *.gray
rm -fr *.rgb
rm -fr *.raw

echo