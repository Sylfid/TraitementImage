#include <time.h>
#include "filtrage_gaussien.h"
#include "conv_spat.h"
#include "pgm.h"


int main (int ac, char **av)  {
    clock_t  debut ,  fin, debut2, fin2;
    int nbIter = 200;
    double incrementation = 0.05;
    int nl,nc;
    unsigned char ** im1=NULL;
    unsigned char ** resultchar=NULL;
    double **resultdouble = NULL;
    double sigma = 0.1;
    FILE* fichier = NULL;

    fichier = fopen("resultTime.txt", "w+");

    im1=lectureimagepgm(av[1],&nl,&nc);
    double **im1double = imuchar2double(im1,nl,nc);

    if (fichier != NULL){

    }
    else{

        printf("Impossible d'ouvrir le fichier test.txt");
        exit(0);

    }
    double temps1 = 0.0;
    double temps2 = 0.0;

    for(int i=0; i<nbIter;i++){
        printf("debut boucle %d\n", i);
        debut=clock (); 
        resultchar = filtrageGaussien(im1, nl, nc, sigma);
        printf(" ");
        fin=clock ();

        libere_image(resultchar);

        debut2=clock ();
        resultdouble = filt_separ(im1double, nl, nc, sigma, (int) 4*sigma);
        fin2=clock ();

        libere_image_double(resultdouble);

        temps1 = ((double) fin - (double) debut)/CLOCKS_PER_SEC;
        temps2 = ((double) fin2 - (double) debut2)/CLOCKS_PER_SEC;
        fprintf(fichier, "%f %f %f\n", sigma, temps1, temps2);
        //double t = (double) fin - debut;
        sigma = sigma + incrementation;
    }
    fclose(fichier);
    return 0;
}
