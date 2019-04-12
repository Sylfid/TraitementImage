#ifndef FILTRE_ADPT_RECH
#define FILTRE_ADPT_RECH

double calcul_w(double** image, int nl, int nc, int x, int y);
double** copie_image(double** image, int nl, int nc);
double calcul_pixel_filtre(double** image, int nl, int nc, int x, int y);
void filtre_adaptatif_recursif_nomodif(double** image, int nl, int nc, int nbIter);
double** filtre_adaptatif_recursif(double** image, int nl, int nc, int nbIter);

#endif
