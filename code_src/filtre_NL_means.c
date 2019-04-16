#include "estimation_bruit.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "pgm.h"
#include "util.h"

double choisi_h(double sigma){
    return 0.4*sigma;
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
            printf("j'ai calcule %i %i\n", gx, gy);
        }
    }
    return imSortie;
}
