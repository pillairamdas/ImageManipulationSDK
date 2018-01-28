#include "utils.h"

/*
Function   : max_of_3
Input      : int a, b, c - Three integers for comparison
Output     : int - Max of three input integers
Description: Find maximum of 3 integers
*/
int max_of_3(int a, int b, int c)
{
	if(a >= b && a >= c)
		return a;
	if(b >= a && b >= c)
		return b;
	return c;
}


/*
Function   : min_of_3
Input      : int a, b, c - Three integers for comparison
Output     : int - Min of three input integers
Description: Find minimum of 3 integers
*/
int min_of_3(int a, int b, int c)
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
			 int size     - Size of buffer
			 char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Dump contents of buffer to file
*/
int dump_to_file(uint8_t* buf, int size, char *str)
{
	int out_fd = -1;
	int status = 0;

	/* Open the file for writing */
	out_fd = open(str, O_CREAT|O_WRONLY, 0666);
	if(out_fd < 0) {
		printf("[ERROR]: Unable to open output file %s with error: %s\n", str, strerror(errno));
		exit(1);
	}	

	/* Write to file */
	if((status = write(out_fd, buf, size)) >= 0) {
		return 0;
	} else {
		printf("[ERROR]: Write Error for %s with error: %s\n", str, strerror(errno));
		return -1;
	}

	close(out_fd);
}


/*
Function   : read_from_file
Input      : uint8_t *buf - Buffer to read to 
		     int size     - Size of buffer
		     char *str    - filename
Output     : 0 : on success
			 -1: on failure
Description: Read contents of file to the buffer
*/
int read_from_file(uint8_t *buf, int size, char *str)
{
	int in_fd = -1;
	int status = 0;

	/* Open the file for reading */ 
	in_fd = open(str, O_RDONLY);
	if(in_fd < 0) {
		printf("[Error]: Unable to open input file %s with error: %s\n", str,strerror(errno));
		exit(1);
	}

	/* Read file to buffer */
	if((status = read(in_fd, buf, size)) >= 0) {
		return 0;
	} else {
		printf("[ERROR]: Read Error for %s with error: %s\n", str, strerror(errno));
		return -1;	
	}

	close(in_fd);
}


/*
Function   : get_input_pixel
Input      : tagImageProp *input_img - Input Image 
             uint8_t *buffer 	- Output buffer to fill in pixel
             int i 				- veritical axis location
             int j 				- horizontal axis location
             int width 			- width of the input image
             int height 		- height of the input image
             int bpp			- bytes per pixel of the input image
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
void get_input_pixel(tagImageProp *input_img, uint8_t *buffer, int i, int j) {
	uint8_t *temp = input_img->buffer + ((i * input_img->width) + j)*input_img->bpp;
	memcpy(buffer, temp, input_img->bpp * sizeof(uint8_t));
}


/*
Function   : get_input_pixel_ptr
Input      : tagImageProp *input_img - Input Image 
             int i 				- veritical axis location
             int j 				- horizontal axis location
Output     : None
Description: Copy the pixel of the input image pointed by [i,j]
*/
uint8_t* get_input_pixel_ptr(tagImageProp *input_img, int i, int j) {
	uint8_t *temp = input_img->buffer + ((i * input_img->width) + j)*input_img->bpp;
	return temp;
}


/*
Function   : int_cmp
Input      : const void *a, *b - Comparison parameters
Output     : Comparison value
Description: qsort int comparison function
*/
int int_cmp(const void *a, const void *b) 
{ 
    const int *ia = (const int *)a; // casting pointer types 
    const int *ib = (const int *)b;
    return *ia  - *ib; 
	/* integer comparison: returns negative if b > a 
	and positive if a > b */ 
} 


/*
Function   : index_max
Input      : int *arr - Input array
			 int size - Input array size
Output     : Index of max value
Description: Find the index of max element in the array
*/
int index_max(int *arr, int size) {
	int i = 0;
	int max_pos = 0;
	int max = 0;
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
Input      : int *arr - Input array
			 int size - Input array size
Output     : Median of the array
Description: Find the median of array
*/
int get_median(int *arr, int size)
{
	qsort(arr, size, sizeof(int), int_cmp);
	return arr[size/2];
}

void get_PSNR(tagImageProp *input_img, tagImageProp *filtered_img)
{
	int channel_count = 0;
	int iterator      = 0;
	long long int noise         = 0;
	float mse         = 0.0;
	long long int   signal      = 0;
	float snr         = 0.0;
	float psnr_255    = 0.0;
	float psnr_max    = 0.0;
	uint8_t* inbuffer   = NULL;
	uint8_t* outbuffer   = NULL;
	int   max_intensity = 0;

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

		printf("\n\n*************************************************\n");
		switch(channel_count) {
			case 0: { printf("The statistics for RED channel is: \n"); } break;
			case 1: { printf("The statistics for GREEN channel is: \n"); } break;
			case 2: { printf("The statistics for BLUE channel is: \n"); } break;
			default: { printf("Incorrect Channel count \n"); } break;
		}


		printf("\n");
		printf("Signal to Noise Ratio                  : %0.2f dB\n", snr);
		printf("Mean Square Error                      : %0.2f\n", mse);
		printf("Peak-Signal to Noise Ratio (Max = 255) : %0.2f dB\n", psnr_255);
		printf("Peak-Signal to Noise Ratio (Max)       : %0.2f dB\n", psnr_max);
		printf("\n*************************************************\n");

		++channel_count;
	}
}