#include <math.h>
#include "filtrage_gaussien.h"
#include "pgm.h"




main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
    int nb,nl,nc, oldnl,oldnc;
    int nbIter = 400;
    float sigma[400];
    double result[400];
    double result1[400];
    double result2[400];

    for(int i=0; i<nbIter; i++){
      sigma[i] = 0.1 + 0.01*(double)i;
    }
    unsigned char **im2 = NULL, ** im1=NULL, ** im1bis=NULL, ** im2bis=NULL;
    //double** im4,** im5, ** im6, ** im7, **im8, **im9,**im10;

    if (ac < 2) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
    /* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
    im1=lectureimagepgm(av[1],&nl,&nc);
    im1bis = lectureimagepgm(av[2],&nl,&nc);
    im2bis = lectureimagepgm(av[3],&nl,&nc);
    if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
    if (im2bis==NULL)  { puts("Lecture image impossible"); exit(1); }
    if (im1bis==NULL)  { puts("Lecture image impossible"); exit(1); }
    /* Calcul de son inverse video */
    for(int i=0; i<nbIter; i++){
      result[i]=psnr_gaussien(im1, im1, nl, nc, sigma[i]); 
      result1[i]=psnr_gaussien(im1bis, im1, nl, nc, sigma[i]); 
      result2[i]=psnr_gaussien(im2bis, im1, nl, nc, sigma[i]); 
    }
    /*for(int i=0; i<100; i++){
      printf("sigma : %f psnr : %f\n", sigma[i], result[i]);
    } */
    FILE* fichier = NULL;

    fichier = fopen("resultPSNR.txt", "r+");

    if (fichier != NULL)

    {

        for(int i=0; i<nbIter ; i++){
            fprintf(fichier, "%f %f %f %f\n", sigma[i], result[i], result1[i], result2[i]);
        }
        fclose(fichier);

    }

    else

    {

        // On affiche un message d'erreur si on veut

        printf("Impossible d'ouvrir le fichier test.txt");

    }


    /* Sauvegarde dans un fichier dont le nom est passe sur la ligne de commande */
    //double** im3 = imuchar2double(im1, nl, nc);
    //im2 = filtrageGaussien(im1, nl, nc, 0.1);
    //ecritureimagepgm(av[2],im2,nl,nc);
    libere_image(im1);
    libere_image(im1bis);
    libere_image(im2bis);

}

