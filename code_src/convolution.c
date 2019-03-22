#include <math.h>
#include "pgm.h"


double** convolution(double** image, double** filtre, int nl, int nc, int tailleMasque){

    double** tampon = alloue_image_double(nl,nc);
    double somme=0;
    double valeurIm = 0;
    int a = tailleMasque;
    int n = 2*tailleMasque+1;
    int valeurx = 0, valeury = 0;
    for(int i=0; i<nl; i++){
        for(int j=0; j<nc ; j++){
            somme=0;
            for(int k=0; k<n; k++){
                for(int p=0; p<n; p++){
                    valeurIm=0.0;
                    valeurx=0;
                    valeury=0;
                    if(i-a+k<0){
                        valeurx = a-i-k-1;
                    }
                    else if(i-a+k >nl-1){
                        valeurx = i-a+k-1-2*(i-a+k-nl);
                    }
                    else{valeurx = i-a+k;}

                    if(j-a+p<0){
                        valeury = a-j-p-1;
                    }
                    else if(j-a+p >nc-1){
                        valeury = j-a+p-1-2*(j-a+p-nc);
                    }
                    else{valeury = j-a+p;}
                    somme += image[valeurx][valeury]*filtre[n-1-k][n-1-p];
                }
            }
            tampon[i][j]=somme;
        }
    }
    return tampon;
}
