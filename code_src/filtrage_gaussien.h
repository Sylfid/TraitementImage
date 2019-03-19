#ifndef CONV_FREQ
#define CONV_FREQ
#include "pgm.h"

unsigned char ** filtrageGaussien(unsigned char** entree, int nl, int nc, double sigma);

void filtrage_write(char* filename_source, char* filename_dest, double sigma);

void multiplication (double** entreReel1, double** entreImag1,
        double** entreReel2, double** entreImag2,
        double** sortieReel, double** sortieImag,
        int nl, int nc);

double psnr_maison(unsigned char** im1, unsigned char** im2, int nl, int nc);
double psnr_gaussien(unsigned char** im1, unsigned char** im2, int nl, int nc, float sigma);
double** convolution(double** image, double** filtre, int nl, int nc, int n);
#endif
