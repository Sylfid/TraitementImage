#ifndef CONV_SPAT
#define CONV_SPAT
#include "pgm.h"

/*
    Fait la convolution spatiale du fichier filename avec comme paramettre sigma
*/
void conv_spatial(char* filename_source, char* filename_dest, double sigma);

#endif
