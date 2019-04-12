#include "estimation_bruit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "pgm.h"
#include "convolution.h"
#include "util.h"

//extern double** convolution(double**, double**, int, int, int);



double ** get_masque_passe_haut(){
    unsigned int N = 1;
    double** masque = calloc(2*N+1, sizeof(double*));
    for(unsigned int i=0; i<2*N+1; i++){
        masque[i] = calloc(2*N+1, sizeof(double));
    }
    //masque 3*3 donne
    masque[0][1] = masque[1][0] = masque[2][1] = masque[1][2] = -1/5;
    masque[1][1] = 1;
    return masque;
}

double calc_ET_local(double **im, int nl, int nc, int t, int x, int y){
    double moyenne = 0;
    for(int i=-t; i<=t; i++){
        for(int j=-t; j<=t; j++){
            moyenne += get_valeur(im, nl, nc, x+i, y+j);
        }
    }
    moyenne /= (2*t+1)*(2*t+1);
    double variance = 0;
    for(int i=-t; i<=t; i++){
        for(int j=-t; j<=t; j++){
            double diff = get_valeur(im, nl, nc, x+i, y+j)-moyenne;
            variance += diff*diff;
        }
    }
    variance /= (2*t+1)*(2*t+1);
    return sqrt(variance);
}

double estimation_bruit(double **im, int nl, int nc, int t, double p){
    //on filtre l'image haut;
    double** masque = get_masque_passe_haut();
    double** image_filtre_haut = convolution(im, masque, nl, nc, 1);
    //on libere le masque tout de suite
    free(masque[0]);
    free(masque[1]);
    free(masque[2]);
    free(masque);
    //on calcule les ecarts types locals pour chaque pixels
    //on fait un tableau ou chaque indice represente le nombre d'ecart type
    //entre [indice*pas, indice*(pas+1)[
    int taille_tableau = 10000;
    double pas = 0.01;
    int ecarts_types[taille_tableau];
    //initialisation de ecarts_types
    for(unsigned int i=0; i<taille_tableau; i++){
        ecarts_types[i] = 0;
    }
    //on compte dans l'histogramme
    for(unsigned int i=0; i<nl; i++){
        for(unsigned int j=0; j<nc; j++){
            double res = calc_ET_local(image_filtre_haut, nl, nc, t, i, j);
            int indice = (double) res/pas;
            if(indice > taille_tableau){
                perror("erreur ecart type trop grand, se reporter a la fonction estimation_bruit");
                exit(0);
            }
            ecarts_types[indice]++;
        }
    }
    //on retrouve l'ecart type correspondant au percentile
    int nombre_percentile = (int) nl*nc*p;
    int courant = 0;
    int indice_courant = 0;
    while(courant < nombre_percentile){
        courant += ecarts_types[indice_courant];
        indice_courant++;
    }
    //on libere l'image alloue par convolution
    libere_image_double(image_filtre_haut);
    return (indice_courant-1)*pas*1.13;
}
