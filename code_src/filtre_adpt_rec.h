#ifndef FILTRE_ADPT_RECH
#define FILTRE_ADPT_RECH

double calcul_w(double** image, int nl, int nc, int x, int y, double seuil);
double** copie_image(double** image, int nl, int nc);
double calcul_pixel_filtre(double** image, int nl, int nc, int x, int y, double seuil);
void filtre_adaptatif_recursif_nomodif(double** image, int nl, int nc, int nbIter, double seuil);
double** filtre_adaptatif_recursif(double** image, int nl, int nc, int nbIter, double seuil);
double calcul_PSNR_filtre_rec(double** imageIni, double** image, int nl, int nc, int nbIter, double seuil);

#endif
