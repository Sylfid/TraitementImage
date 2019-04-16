#include "filtre_NL_means.h"
#include "pgm.h"
#include "util.h"
#include "estimation_bruit.h"

int main(){
    int nl,nc;
    unsigned char **im_source_char = lectureimagepgm("../imagestp/formes2bb20.pgm",&nl,&nc);
    //sous forme de double
    double **im_source_double = imuchar2double(im_source_char, nl, nc);

    unsigned char ** im_non_bruite_char = lectureimagepgm("../imagestp/formes2.pgm",&nl,&nc);
    double ** im_non_bruite_double = imuchar2double(im_non_bruite_char, nl, nc);

    //
    double bruit_reel = 20;
    double bruit_estime = estimation_bruit(im_source_double, nl, nc, 10, 0.005);

    //test du filtre
    double **im_dest_double = filtre_NL_means(im_source_double, nl, nc, 21, 5, 20);
    //on recupere l'image en char
    unsigned char **im_dest_char = imdouble2uchar(im_dest_double, nl, nc);
    //on l'affiche dans cette image test
    ecritureimagepgm("image_test.pgm", im_dest_char, nl, nc);

    // for(int mult = 1; mult<=10; mult++){
    //     //test du filtre
    //     double **im_dest_double = filtre_bilateral(im_source_double, nl, nc, 10, mult * bruit_reel);
    //     //on recupere l'image en char
    //     unsigned char **im_dest_char = imdouble2uchar(im_dest_double, nl, nc);
    //
    //     double psnr = psnr_double(im_dest_double, im_non_bruite_double, nl, nc);
    //
    //     printf("pour un multiplicateur %d, on obtient un psnr de %f\n", mult, psnr);
    //
    //     //liberation
    //     libere_image_double(im_dest_double);
    //     libere_image(im_dest_char);
    // }

    // double eqm = EQM_maison(im_source_double, im_dest_double, nl, nc);
    //
    // printf("EQM : %f\n", eqm);

    //liberation
    libere_image(im_source_char);
    libere_image_double(im_source_double);
    libere_image_double(im_dest_double);
    libere_image(im_dest_char);
}
