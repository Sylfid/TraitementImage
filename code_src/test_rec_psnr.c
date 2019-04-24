#include <math.h>
#include "filtre_adpt_rec.h"
#include "util.h"
#include "pgm.h"
#include "util.h"


int main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
    int nl, nc;
    unsigned char **im1, **im2, **im3, **im4;
    im1=lectureimagepgm(av[1],&nl,&nc);
    double**imref = imuchar2double(im1,nl,nc);

    im2=lectureimagepgm(av[2],&nl,&nc);
    double** im_faible_bruit = imuchar2double(im2,nl,nc);

    im3=lectureimagepgm(av[3],&nl,&nc);
    double** im_moyen_bruit = imuchar2double(im3,nl,nc);

    im4=lectureimagepgm(av[4],&nl,&nc);
    double** im_fort_bruit = imuchar2double(im4,nl,nc);

    double step = 0.5;
    double seuilMax = 100.;

    FILE* fichier = NULL;
    fichier = fopen("resultPSNR.txt", "r+");

    for(double i = 5.0; i<seuilMax; i+=step){
        double result_faible = calcul_PSNR_filtre_rec(imref, im_faible_bruit, nl, nc, 500, i);
        double result_moyen = calcul_PSNR_filtre_rec(imref, im_moyen_bruit, nl, nc, 500, i);
        double result_fort = calcul_PSNR_filtre_rec(imref, im_fort_bruit, nl, nc, 500, i);
        fprintf(fichier, "%f %f %f %f\n", i, result_faible, result_moyen, result_fort);
    }


    libere_image(im1);
    libere_image(im2);
    libere_image(im3);
    libere_image(im4);
    libere_image_double(imref);
    libere_image_double(im_faible_bruit);
    libere_image_double(im_moyen_bruit);
    libere_image_double(im_fort_bruit);
    return 0;
}

