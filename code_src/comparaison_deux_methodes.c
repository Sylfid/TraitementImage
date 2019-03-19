#include "filtrage_gaussien.h"
#include "conv_spat.h"
#include "pgm.h"
#include <assert.h>



//nom du fichier sur lequel on veut tester
static char* FICHIER = "../imagestp/formes2g.pgm";
//multiplicateur de sigma pour la taie du masque pour la convolution
#define M_MASQUE 6
//valeur de sigma voulu
#define NB_VALEUR_SIGMA 5
static int sigmas[NB_VALEUR_SIGMA] = {1,2, 5, 10, 20};

double EQM_maison(double** r1, double** r2, int nl, int nc){
    double result = 0;
    for(int i=0; i<nl; i++){
        for(int j=0; j<nc; j++){
            result += (r1[i][j]-r2[i][j])*(r1[i][j]-r2[i][j]);
        }
    }
    return result/(nl*nc);
}


double calc_EQM(unsigned char **im_source_char, int nl, int nc, double sigma, int N){
    //calcul de l'image avec filtrage gaussien
    unsigned char ** dest_filtrage_gaussien = filtrageGaussien(im_source_char, nl, nc, sigma);
    double ** dest_filtrage_gaussien_double = imuchar2double(dest_filtrage_gaussien, nl, nc);

    //calcul de l'image avec filtrage par convolution spatiale
    double **im_source_double = imuchar2double(im_source_char, nl, nc);
    double ** dest_convolution_double = filt_separ(im_source_double, nl, nc, sigma, N);

    //calcul EQM
    double eqm = EQM_maison(dest_filtrage_gaussien_double, dest_convolution_double, nl, nc);
    //liberation memoire des images utilises
    libere_image(dest_filtrage_gaussien);
    libere_image_double(dest_filtrage_gaussien_double);
    libere_image_double(im_source_double);
    libere_image_double(dest_convolution_double);

    return eqm;
}


int main (int ac, char **av) {
    int nl,nc;
    unsigned char ** im_source_char=NULL;

    //on recupere l'image
    im_source_char = lectureimagepgm(FICHIER,&nl,&nc);

    double eqm;
    //on teste pour tout les sigmas
    FILE* fichier = fopen("resultEQM.txt", "w+");
    assert(fichier != NULL);
    for(unsigned int i=1; i<=M_MASQUE; i++){ //boucle sur les taille de masques
        fprintf(fichier, "%d ", i);
        for(unsigned int j=0; j<NB_VALEUR_SIGMA; j++){ //boucle sur les sigmas
            eqm = calc_EQM(im_source_char, nl, nc, sigmas[j], sigmas[j]*i);
            fprintf(fichier, "%f ", eqm);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
    libere_image(im_source_char);
    return EXIT_SUCCESS;
}
