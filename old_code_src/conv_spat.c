#include "pgm.h"
#include <math.h>



void liber_coef(double**tab){
    free(tab[0]);
    free(tab[1]);
    free(tab);
}

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

/*
    filtrage separe
*/
double** filt_separ(double** im_source, int nl, int nc, double sigma, int N){
    //printf("calcul du filtrage separe\n");
    double **im_inter = alloue_image_double(nl, nc);
    double **tab_coef = make_coef(nl, nc, sigma);
    //iteration sur tout les points de l'image
    //printf("debut du calcul pour chaque point\n");
    for(int x = 0; x<nl; x++){
        //printf("ligne [%i]\n", x);;
        for(int y = 0; y<nc; y++){
            //printf("calcul du point [%i, %i]\n", x, y);
            //on fait la somme pour la convolution
            double res2 = 0;//la somme exterieur sur j
            for(int j = -N; j<=N; j++){
                res2 += tab_coef[1][abs(j)]*get_valeur(im_source, nl, nc, x, y+j);
            }
            //fin de la somme, on sauvegarde dans l'image
            im_inter[x][y] = res2;
        }
    }
    //filtrage sur colonne
    double **im_dest = alloue_image_double(nl, nc);
    for(int x = 0; x<nl; x++){
        //printf("ligne [%i]\n", x);;
        for(int y = 0; y<nc; y++){
            //printf("calcul du point [%i, %i]\n", x, y);
            //on fait la somme pour la convolution
            double res1 = 0;//la somme exterieur sur i
            for(int i = -N; i<=N; i++){
                res1 += tab_coef[0][abs(i)]*get_valeur(im_inter, nl, nc, x+i, y);
            }
            //fin de la somme, on sauvegarde dans l'image
            im_dest[x][y] = res1/(2*M_PI*sigma*sigma);
        }
    }
    //liberation de tab_coef
    liber_coef(tab_coef);
    //liberation de l'image intermediaire
    libere_image_double(im_inter);
    //return
    return im_dest;
}

int get_N_v1(double sigma){
    return sigma;
}

void conv_spatial(char* filename_source, char* filename_dest, double sigma){
    int nl,nc;
    unsigned char ** im_source_char=NULL;
    double **im_source_double=NULL;
    double **im_dest_double=NULL;
    unsigned char ** im_dest_char=NULL;

    //printf("Debut des lecture de fichier\n");
    im_source_char = lectureimagepgm(filename_source,&nl,&nc);
    im_source_double = imuchar2double(im_source_char, nl, nc);


    int N = get_N_v1(sigma);

    im_dest_double = filt_separ(im_source_double, nl, nc, sigma, N);
    im_dest_char = imdouble2uchar(im_dest_double, nl, nc);

    ecritureimagepgm(filename_dest, im_dest_char, nl, nc);

    //liberation
    libere_image(im_source_char);
    libere_image_double(im_source_double);
    libere_image_double(im_dest_double);
    libere_image(im_dest_char);

}
