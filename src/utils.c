#include "utils.h"

/*
Function   : max_of_3
Input      : int32_t a, b, c - Three integers for comparison
Output     : int32_t - Max of three input integers
Description: Find maximum of 3 integers
*/
int32_t max_of_3(int32_t a, int32_t b, int32_t c)
{
	if(a >= b && a >= c)
		return a;
	if(b >= a && b >= c)
		return b;
	return c;
}


/*
Function   : min_of_3
Input      : int32_t a, b, c - Three integers for comparison
Output     : int32_t - Min of three input integers
Description: Find minimum of 3 integers
*/
int32_t min_of_3(int32_t a, int32_t b, int32_t c)
{
	if(a <= b && a <= c)
		return a;
	if(b <= a && b <= c)
		return b;
	return c;
}


/*
Function   : dump_to_file 
Input      : uint8_t *buf - Buffer to write
			 int32_t size     - Size of buffer
			 char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Dump contents of buffer to file
*/
int32_t dump_to_file(uint8_t* buf, int32_t size, char *str)
{
	int32_t out_fd = -1;
	int32_t status = 0;

	/* Open the file for writing */
	out_fd = open(str, O_CREAT|O_WRONLY, 0666);
	if(out_fd < 0) {
		LOG(ERROR, "Unable to open output file %s with error: %s\n", str, strerror(errno));
		exit(1);
	}	

	/* Write to file */
	if((status = write(out_fd, buf, size)) >= 0) {
		return 0;
	} else {
		LOG(ERROR, "Write Error for %s with error: %s\n", str, strerror(errno));
		return -1;
	}

	close(out_fd);
}


/*
Function   : read_from_file
Input      : uint8_t *buf - Buffer to read to 
		     int32_t size     - Size of buffer
		     char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Read contents of file to the buffer
*/
int32_t read_from_file(uint8_t *buf, int32_t size, char *str)
{
	int32_t in_fd  = -1;
	int32_t status = 0;

	/* Open the file for reading */ 
	in_fd = open(str, O_RDONLY);
	if(in_fd < 0) {
		LOG(ERROR, "Unable to open input file %s with error: %s\n", str,strerror(errno));
		exit(1);
	}

	/* Read file to buffer */
	if((status = read(in_fd, buf, size)) >= 0) {
		return 0;
	} else {
		LOG(ERROR, "Read Error for %s with error: %s\n", str, strerror(errno));
		return -1;	
	}

	close(in_fd);
}


/*
Function   : get_input_pixel
Input      : tagImageProp *input_img - Input Image 
             uint8_t *buffer 	- Output buffer to fill in pixel
             int32_t i 				- veritical axis location
             int32_t j 				- horizontal axis location
             int32_t width 			- width of the input image
             int32_t height 		- height of the input image
             int32_t bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
void get_input_pixel(tagImageProp *input_img, uint8_t *buffer, int32_t i, int32_t j) {
	uint8_t *temp = input_img->buffer + ((i * input_img->width) + j)*input_img->bpp;
	memcpy(buffer, temp, input_img->bpp * sizeof(uint8_t));
}


/*
Function   : get_input_pixel_ptr
Input      : tagImageProp *input_img - Input Image 
             int32_t i 				- veritical axis location
             int32_t j 				- horizontal axis location
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
uint8_t* get_input_pixel_ptr(tagImageProp *input_img, int32_t i, int32_t j) {
	uint8_t *temp = input_img->buffer + ((i * input_img->width) + j)*input_img->bpp;
	return temp;
}


/*
Function   : int_cmp
Input      : const void *a, *b - Comparison parameters
Output     : Comparison value
Description: qsort int32_t comparison function
*/
int32_t int_cmp(const void *a, const void *b) 
{ 
    const int32_t *ia = (const int32_t *)a; // casting pointer types 
    const int32_t *ib = (const int32_t *)b;

    return *ia  - *ib; 
	/* integer comparison: returns negative if b > a 
	and positive if a > b */ 
} 


/*
Function   : index_max
Input      : int32_t *arr - Input array
			 int32_t size - Input array size
Output     : Index of max value
Description: Find the index of max element in the array
*/
int32_t index_max(int32_t *arr, int32_t size) {
	
	int32_t i       = 0;
	int32_t max_pos = 0;
	int32_t max     = 0;

	for(i = 0; i<size; i++) {
		if(max < arr[i]) {
			max = arr[i];
			max_pos = i;
		}
	}

	return max_pos;
}


/*
Function   : get_median
Input      : int32_t *arr - Input array
			 int32_t size - Input array size
Output     : Median of the array
Description: Find the median of array
*/
int32_t get_median(int32_t *arr, int32_t size)
{
	qsort(arr, size, sizeof(int32_t), int_cmp);
	return arr[size/2];
}


/*
Function   : get_PSNR
Input      : tagImageProp *input_img - Input image
			 tagImageProp *filtered_img - Filtered image
Output     : None
Description: Print the PSNR statistics
*/
void get_PSNR(tagImageProp *input_img, tagImageProp *filtered_img)
{
	int32_t        channel_count  = 0;
	int32_t        iterator       = 0;
	long long int  noise          = 0;
	float          mse            = 0.0;
	long long int  signal         = 0;
	float          snr            = 0.0;
	float          psnr_255       = 0.0;
	float          psnr_max       = 0.0;
	uint8_t       *inbuffer       = NULL;
	uint8_t       *outbuffer      = NULL;
	int32_t        max_intensity  = 0;


	while(channel_count < input_img->bpp) {

		max_intensity = 0;
		noise = 0;
		signal = 0;


		iterator = 0;
		while(iterator < input_img->pixel_count) {

			inbuffer  = input_img->buffer + iterator * input_img->bpp + channel_count;
			outbuffer = filtered_img->buffer + iterator * filtered_img->bpp + channel_count;
			
			signal += pow(*(inbuffer), 2);
			noise  += pow((*outbuffer - *inbuffer), 2);

			if(max_intensity < *inbuffer) {
				max_intensity = *inbuffer;
			}

			++iterator;
		}

		mse = noise / input_img->pixel_count;
		snr = 10.0 * log10(signal / (float)noise);
		psnr_255 = 10.0 * log10(pow(255,2) / mse);
		psnr_max = 10.0 * log10(pow(max_intensity,2) / mse);

		LOG(OUTPUT, "\n\n*************************************************\n");
		switch(channel_count) {
			case 0: { LOG(OUTPUT, "The statistics for RED channel is: \n"); } break;
			case 1: { LOG(OUTPUT, "The statistics for GREEN channel is: \n"); } break;
			case 2: { LOG(OUTPUT, "The statistics for BLUE channel is: \n"); } break;
			default: { LOG(OUTPUT, "Incorrect Channel count \n"); } break;
		}



		LOG(OUTPUT, "\n");
		LOG(OUTPUT, "Noise                                  : %lld\n", noise);
		LOG(OUTPUT, "Signal to Noise Ratio                  : %0.2f dB\n", snr);
		LOG(OUTPUT, "Mean Square Error                      : %0.2f\n", mse);
		LOG(OUTPUT, "Peak-Signal to Noise Ratio (Max = 255) : %0.2f dB\n", psnr_255);
		LOG(OUTPUT, "Peak-Signal to Noise Ratio (Max)       : %0.2f dB\n", psnr_max);
		LOG(OUTPUT, "\n*************************************************\n");

		++channel_count;
	}
}