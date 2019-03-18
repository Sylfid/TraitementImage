#include <time.h>
#include "filtrage_gaussien.h"
#include "conv_spat.h"
#include "pgm.h"


int main (int ac, char **av)  {  
    clock_t  debut ,  fin, debut2, fin2;
    int nbIter = 400;
    int nl,nc;
    unsigned char ** im1=NULL;
    unsigned char ** resultchar=NULL;
    double **resultdouble = NULL;
    double sigma = 0.1;
    FILE* fichier = NULL;

    fichier = fopen("resultTime.txt", "r+");

    im1=lectureimagepgm(av[1],&nl,&nc);
    double **im1double = imuchar2double(im1,nl,nc);

    if (fichier != NULL){

    }
    else{

        printf("Impossible d'ouvrir le fichier test.txt");
        exit(0);

    }
    for(int i=0; i<nbIter;i++){
        sigma = sigma + 0.01;

        debut=clock (); 
        resultchar = filtrageGaussien(im1, nl, nc, sigma);
        fin=clock ();

        libere_image(resultchar);

        debut2=clock (); 
        resultdouble = filt_separ(im1double, nl, nc, sigma, (int) 2*sigma);
        fin2=clock ();

        libere_image_double(resultdouble);
        //double t = (double) fin - debut;
        if (fichier != NULL){

            for(int i=0; i<nbIter ; i++){
                fprintf(fichier, "%f %f %f\n", sigma, ((double) fin - debut)/
                        CLOCKS_PER_SEC, ((double) fin2 - debut2)/CLOCKS_PER_SEC);
            }
            fclose(fichier);
        }
    }
    return 0;
}
