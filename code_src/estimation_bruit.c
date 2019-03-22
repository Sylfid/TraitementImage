#include "estimation_bruit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "pgm.h"

extern double** convolution(double**, double**, int, int, int);

/*
    obtient la valeur de l'image en x et y
    Si ces derniers depassent de l'image alors : (IMPLEMENTATION COURANTE)
        -on prolonge par continuite l'image
*/
double get_valeur(double** image, int nl, int nc, int x, int y){
    int x_f;
    int y_f;
    //x final de reference
    if(x >= nl){
        x_f = nl-1;
    } else if (x < 0){
        x_f = 0;
    } else {
        x_f = x;
    }
    //y final de reference
    if(y >= nc){
        y_f = nl-1;
    } else if (y < 0){
        y_f = 0;
    } else {
        y_f = y;
    }
    //on retourne le point, prolonge ou non
    return image[x_f][y_f];
}

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
    free(masque);
    //on calcule les ecarts types locals pour chaque pixels
    //on fait un tableau ou chaque indice represente le nombre d'ecart type
    //entre [indice*pas, indice*(pas+1)[
    int taille_tableau = 100;
    double pas = 1.0;
    int ecarts_types[taille_tableau];
    for(unsigned int i=0; i<nl; i++){
        for(unsigned int j=0; j<nc; j++){
            double res = calc_ET_local(image_filtre_haut, nl, nc, t, i, j);
            int indice = (double) res/pas;
            if(indice > 100){
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
    return (indice_courant-1)*pas;
}
