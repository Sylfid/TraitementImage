#include "pgm.h"
#include <math.h>


double calc_coef(int i, double sigma){
    return exp(-(i*i)/(2*sigma*sigma));;
}

/*
    precalcul tout les coefficients en exp et le met dans un double** alloue
    ce double** comprend deux tableaux, un de taille nl+1 et l'autre de taille nc+1
    Si i ou j est negatif, utilise -i/-j pour avoir acces au tableau
*/
double** make_coef(int nl, int nc, double sigma){
    //on va faire simple et creer une image de double
    double** tab = malloc(2*sizeof(double*));
    //calcul des elements pour i
    tab[0] = malloc((nl+1)*sizeof(double));
    for(int i=0; i<=nl; i++){
        tab[0][i] = calc_coef(i, sigma);
    }
    //calcul des elements pour j
    tab[1] = malloc((nc+1)*sizeof(double));
    for(int j=0; j<=nc; j++){
        tab[1][j] = calc_coef(j, sigma);
    }
    return tab;
}

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
    printf("calcul du filtrage separe\n");
    double **im_dest = alloue_image_double(nl, nc);
    double **tab_coef = make_coef(nl, nc, sigma);
    //iteration sur tout les points de l'image
    printf("debut du calcul pour chaque point\n");
    for(int x = 0; x<nl; x++){
        //printf("ligne [%i]\n", x);;
        for(int y = 0; y<nl; y++){
            //printf("calcul du point [%i, %i]\n", x, y);
            //on fait la somme pour la convolution
            double res1 = 0;//la somme exterieur sur i
            for(int i = -N; i<=N; i++){
                double res2 = 0;//la somme interieur sur j
                for(int j = -N; j<=N; j++){
                    res2 += tab_coef[1][(j<0)?(-j):j]*get_valeur(im_source, nl, nc, x+i, y+j);
                }
                res1 += res2*tab_coef[0][(i<0)?(-i):i];
            }
            //fin de la somme, on sauvegarde dans l'image
            im_dest[x][y] = res1/(2*M_PI*sigma*sigma);
        }
    }
    //liberation de tab_coef
    liber_coef(tab_coef);
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
