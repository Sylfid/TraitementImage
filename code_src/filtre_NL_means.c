#include "estimation_bruit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "pgm.h"
#include "util.h"

double choisi_h(double sigma){
    double h;
    if(sigma<=15){
        h = 0.4*sigma;
    } else if(sigma<=30){
        h = 0.4*sigma;
    } else if(sigma<=45){
        h = 0.35*sigma;
    } else if(sigma<=45){
        h = 0.35*sigma;
    } else {//if(sigma<=45){
        h = 0.35*sigma;
    }
    return h;
}

int choisir_patch(double sigma){
    int patch;
    if(sigma<=15){
        patch = 3;
    } else if(sigma<=30){
        patch = 5;
    } else if(sigma<=45){
        patch = 7;
    } else if(sigma<=45){
        patch = 9;
    } else {//if(sigma<=45){
        patch = 11;
    }
    return patch;
}

int choisir_region(double sigma){
    int region;
    if(sigma<=15){
        region = 21;
    } else if(sigma<=30){
        region = 21;
    } else if(sigma<=45){
        region = 35;
    } else if(sigma<=45){
        region = 35;
    } else {//if(sigma<=45){
        region = 35;
    }
    return region;
}

double** filtre_NL_means(double** imageBruite,
                        int nl,
                        int nc,
                        int t,
                        int r,
                        double sigma)
{
    double** imSortie = alloue_image_double(nl,nc);

    double deux_sigma2 = 2*sigma*sigma;

    double h = choisi_h(sigma);

    //pour tout les pixels de l'image
    for(int gx=0; gx<nl; gx++){
        for(int gy=0; gy<nc; gy++){
            double num = 0;
            double den = 0;
            //pour tout les pixels de la region Rt(p)
            for(int x=gx-t; x<=gx+t; x++){
                for(int y=gy-t; y<=gy+t; y++){
                    //d²(P,Q)
                    double d2 = 0;
                    //somme sur le patch
                    for(int i=-r; i<=r; i++){
                        for(int j=-r; j<=r; j++){
                            double in_carre = get_valeur(imageBruite, nl, nc, x+i, y+j)
                                              -get_valeur(imageBruite, nl, nc, gx+i, gy+j);
                            d2 += in_carre*in_carre;
                        }
                    }
                    d2 /= (2*r+1);
                    //
                    //w(p,q)
                    double w = (d2>deux_sigma2)?exp(-(d2-deux_sigma2)/(h*h)):1;
                    //calcul numerateur
                    num += w*get_valeur(imageBruite, nl, nc, x, y);
                    //calcul denominateur
                    den += w;
                }
            }
            imSortie[gx][gy] = num/den;
            //printf("j'ai calcule %i %i\n", gx, gy);
        }
    }
    return imSortie;
}

double **filtre_NL_means_auto(double** imageBruite,
                        int nl,
                        int nc,
                        double sigma)
{
    return filtre_NL_means(imageBruite, nl, nc, choisir_region(sigma), choisir_patch(sigma), sigma);
}
