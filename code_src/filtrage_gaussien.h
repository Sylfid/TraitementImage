#ifndef CONV_FREQ
#define CONV_FREQ
#include "pgm.h"

unsigned char ** filtrageGaussien(double** entree, int nl, int nc, float sigma);

void multiplication (double** entreReel1, double** entreImag1,
        double** entreReel2, double** entreImag2, 
        double** sortieReel, double** sortieImag, 
        int nl, int nc);

#endif

