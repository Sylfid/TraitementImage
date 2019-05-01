#ifndef FILTREMEDIAN
#define FILTREMEDIAN
#include "pgm.h"

int calculMedian2(double** image, int nl, int nc, int tailleMasque, int i, int j);

int maxImage(double** image, int nl, int nc);

int calculMedian(int* tableau, int nbElem);

void ajoutValeurHist(int* hist, int taille, int valeur);

void enleveValeurHist(int* hist, int taille, int valeur);

void reinitialisationHist(int* hist, int taille, double** image, int nl, int nc, int tailleMasque, int i);

void etapeSuivanteMedian(int* hist, int taille, double** image, int nl, int nc, int i, int j, int tailleMasque);

double calcul_psnr_median(double **image1, double **image2, int nl, int nc, int taille);
double** filtreMedian(double** imageBruite, int nl, int nc, int tailleMasque);

#endif
