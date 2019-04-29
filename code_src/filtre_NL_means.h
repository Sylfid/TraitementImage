#ifndef FNLMEANH
#define FNLMEANH

double** filtre_NL_means(double** imageBruite,
                        int nl,
                        int nc,
                        int taille_region,
                        int taille_patch,
                        double sigma);

double **filtre_NL_means_auto(double** imageBruite,
                        int nl,
                        int nc,
                        double sigma);

#endif
