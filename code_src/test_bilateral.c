#include "filtre_bilateral.h"
#include "pgm.h"
#include "util.h"
#include "estimation_bruit.h"

int main(){
    int nl,nc;
    //image bruit faible
    unsigned char **im_source_char1 = lectureimagepgm("../imagestp/formes2p1.pgm",&nl,&nc);
    //sous forme de double
    double **im_source_double1 = imuchar2double(im_source_char1, nl, nc);

    //image bruit moyen
    unsigned char **im_source_char2 = lectureimagepgm("../imagestp/formes2p3.pgm",&nl,&nc);
    //sous forme de double
    double **im_source_double2 = imuchar2double(im_source_char2, nl, nc);

    //image bruit fort
    unsigned char **im_source_char3 = lectureimagepgm("../imagestp/formes2p5.pgm",&nl,&nc);
    //sous forme de double
    double **im_source_double3 = imuchar2double(im_source_char3, nl, nc);

    unsigned char ** im_non_bruite_char = lectureimagepgm("../imagestp/formes2.pgm",&nl,&nc);
    double ** im_non_bruite_double = imuchar2double(im_non_bruite_char, nl, nc);

    //
    double bruit_reel1 = 1;
    double bruit_reel2 = 3;
    double bruit_reel3 = 5;
    //double bruit_estime = estimation_bruit(im_source_double, nl, nc, 10, 0.005);

    //fichier d'ecriture des resultats
    FILE* fichier = fopen("resultBilateralP.txt", "w+");

    double psnrbase1 = psnr_double(im_source_double1, im_non_bruite_double, nl, nc);
    double psnrbase2 = psnr_double(im_source_double2, im_non_bruite_double, nl, nc);
    double psnrbase3 = psnr_double(im_source_double3, im_non_bruite_double, nl, nc);

    fprintf(fichier, "%f %f %f %f \n", 0.0, psnrbase1, psnrbase2, psnrbase3);

    for(float mult = 1; mult<=3; mult+=0.2){
        printf("multiplicateur %f\n", mult);
        //test du filtre sur image 1
        double **im_dest_double1 = filtre_bilateral(im_source_double1, nl, nc, 10, mult * bruit_reel1);
        //psnr
        double psnr1 = psnr_double(im_dest_double1, im_non_bruite_double, nl, nc);

        //test du filtre sur image 2
        double **im_dest_double2 = filtre_bilateral(im_source_double2, nl, nc, 10, mult * bruit_reel2);
        //psnr
        double psnr2 = psnr_double(im_dest_double2, im_non_bruite_double, nl, nc);

        //test du filtre sur image 3
        double **im_dest_double3 = filtre_bilateral(im_source_double3, nl, nc, 10, mult * bruit_reel3);
        //psnr
        double psnr3 = psnr_double(im_dest_double3, im_non_bruite_double, nl, nc);

        fprintf(fichier, "%f %f %f %f \n", mult, psnr1, psnr2, psnr3);

        //liberation
        libere_image_double(im_dest_double1);
        libere_image_double(im_dest_double2);
        libere_image_double(im_dest_double3);
    }


    //liberation
    libere_image(im_source_char1);
    libere_image_double(im_source_double1);
    libere_image(im_source_char2);
    libere_image_double(im_source_double2);
    libere_image(im_source_char3);
    libere_image_double(im_source_double3);

    fclose(fichier);
}
