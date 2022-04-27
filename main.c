#include <stdio.h>
#include <complex.h>
#include <math.h>
#include "libattopng.h"
#include "libattopng.c"

#define RGB(r, g, b) ((r) | ((g) << 8) | ((b) << 16))

int main(int argc, char *argv[]) {
	if (argc < 6) {
		printf("Mandelbrot generator made in C.\nUsage: <image size (uint)> <iterations (uint)> <x offset (double)> <y offset (double> <domain (double)> <save path (char[])>");
		return 0;
	}
	const unsigned int dataSize = atoi(argv[1]);
	const unsigned int iterCount = atoi(argv[2]);
	const double xOffset = atof(argv[3]);
	const double yOffset = atof(argv[4]);
	const double domainSize = atof(argv[5]);
	libattopng_t* image = libattopng_new(dataSize, dataSize, PNG_RGB);
	double complex* dataArray[dataSize];
	double complex* addendArray[dataSize];
	unsigned int* iterArray[dataSize];
	for (unsigned int i = 0; i < dataSize; i++) {
		dataArray[i] = (double complex*)malloc(dataSize * sizeof(double complex));
		addendArray[i] = (double complex*)malloc(dataSize * sizeof(double complex));
		iterArray[i] = (unsigned int*)malloc(dataSize * sizeof(unsigned int));
	}
	for(unsigned int y=0; y<dataSize; y++) {
		for(unsigned int x=0; x<dataSize; x++) {
			dataArray[y][x] = 0 + 0 * I;
			addendArray[y][x] = (((2*x*domainSize)-(dataSize*domainSize)+(xOffset*dataSize))/dataSize) + (((2*y*domainSize)-(dataSize*domainSize)+(yOffset*dataSize))/dataSize) * I;
			iterArray[y][x] = 0;
		}
	}
	for(unsigned int i=0; i<iterCount; i++) {
		for(unsigned int y=0; y<dataSize; y++) {
			for(unsigned int x=0; x<dataSize; x++) {
				if (cabs(dataArray[y][x]) <= 2){
					dataArray[y][x] = (creal(cpow(dataArray[y][x],2))+creal(addendArray[y][x])) + (cimag(cpow(dataArray[y][x],2))+cimag(addendArray[y][x])) * I;
					iterArray[y][x]++;
				}
			}
		}
		printf("\rIterating: %d/%d",(i+1),iterCount);
	}
	for(unsigned int y=0; y<dataSize; y++) {
		for(unsigned int x=0; x<dataSize; x++) {
			if (cabs(dataArray[y][x]) > 2){
				libattopng_set_pixel(image,x,y,RGB((unsigned char)round(255*((cos(iterArray[y][x])+1)/2)),(unsigned char)round(255*((cos(iterArray[y][x]/2)+1)/2)),(unsigned char)round(255*((cos(iterArray[y][x]/4)+1)/2))));
			} else {
				libattopng_set_pixel(image,x,y,0);
			}
		}
	}
	libattopng_save(image, argv[6]);
	libattopng_destroy(image);
	for (unsigned int i = 0; i < dataSize; i++) {
		free(dataArray[i]);
		free(addendArray[i]);
		free(iterArray[i]);
	}
	return 0;
}
