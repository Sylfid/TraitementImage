#include "estimation_bruit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "pgm.h"
#include "util.h"

#define abs(x) (x>=0)?x:-x


double calc_coef_exp_simple(int i, int j, double sigma){
    return exp(-(i*i+j*j)/(2*sigma*sigma));;
}

/*
    precalcul tout les coefficients en exp et le met dans un double** alloue
    ce double** est un tableau de taille [3sigma+1]²
    Si i ou j est negatif, utilise -i/-j pour avoir acces au tableau
*/
double** make_coef_exp_simple(int nl, int nc, double sigma){
    int borne_indice = 3*sigma; //pour avoir en entier les bornes de la somme
    //tableau 2*(borne_indice+1)
    double** tab = calloc(nl, sizeof(double*));

    for(int i=0; i<=borne_indice; i++){
        tab[i] = calloc(nc, sizeof(double));
        for(int j=0; j<=borne_indice; j++){
            tab[i][j] = calc_coef_exp_simple(i, j, sigma);
        }
    }
    return tab;
}

double calc_coef_exp_image(double** im, int nl, int nc, int x, int y, int i, int j, double sigma){
    double dans_carre = get_valeur(im, nl, nc, x+i, y+j) - get_valeur(im, nl, nc, x, y);
    return exp(-(dans_carre*dans_carre)/(2*sigma*sigma));;
}

// /*
//     precalcul tout les coefficients en exp avec l'image et le met dans un double** alloue
//     ce double** est un tableau de taille [6sigma+1]²[nl][nc]
//     ATTENTION acces d'abbord avec i et j puis x et y (pour avoir moins d'allocation)
// */
// double** make_coef_exp_image(double** im, int nl, int nc, double sigma){
//     int borne_indice = 3*sigma; //pour avoir en entier les bornes de la somme
//     //tableau 2*(borne_indice+1)
//     double** tab = calloc(nl, sizeof(double*));
//
//     for(int i=0; i<borne_indice; i++){
//         tab[i] = malloc(nc, sizeof(double));
//         for(int j=0; j<borne_indice; j++){
//             tab[i][j] = calc_coef_exp_image(im, nl, nc, x, y, i, j, sigma);
//         }
//     }
//     return tab;
// }


double** filtre_bilateral(double** imBruite, int nl, int nc, double sigma1, double sigma2){
    double** imSortie = alloue_image_double(nl,nc);

    int borne_indice = 3*sigma1; //pour avoir en entier les bornes de la somme

    //precalcul des coef en exp(-(i²+j²)/2sigma²)
    double ** tab_coef_exp = make_coef_exp_simple(nl, nc, sigma1);

    for(int x=0; x<nl; x++){
        for(int y=0; y<nc; y++){
            double num = 0, den = 0;
            for(int i=-borne_indice; i<=borne_indice; i++){
                for(int j=-borne_indice; j<=borne_indice; j++){
                    double mult_exp = tab_coef_exp[abs(i)][abs(j)]*calc_coef_exp_image(imBruite, nl, nc, x, y, i, j, sigma2);
                    num += mult_exp*get_valeur(imBruite, nl, nc, x+i, y+j);
                    den += mult_exp;
                }
            }
            //printf("j'ai calcule le point %d %d\n",x, y);
            imSortie[x][y] = num/den;
        }
    }
    return imSortie;
}
