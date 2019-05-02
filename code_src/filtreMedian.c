#include <math.h>
#include "pgm.h"

int calculMedian2(double** image, int nl, int nc, int tailleMasque, int i, int j){
    if(i+tailleMasque >= nl || j+tailleMasque >= nc){
        printf("Problème utilisation calculMedian");
        exit(0);
    }
    if(i-tailleMasque < 0 || j-tailleMasque < 0){
        printf("Problème utilisation calculMedian");
        exit(0);
    }
    int somme =0;
    int n = 2*tailleMasque +1;
    int resultat=0;
    while(somme < n*n/2){
        for(int p=0; p<n; p++){
            for(int k=0; k<n; k++){
                if((int) image[i-tailleMasque+p][j-tailleMasque+k]== resultat){
                    somme +=1;
                }
            }
        }
        resultat += 1;
    }
    return resultat;
}

int maxImage(double** image, int nl, int nc){
    int max = (int) image[0][0];
    for(int i=0; i<nl; i++){
        for(int j=0; j<nc; j++){
            if(max < (int) image[i][j]){max =(int) image[i][j];}
        }
    }
    return max;
}

int calculMedian(int* tableau, int nbElem){
    int somme = 0;
    int i=0;
    while(somme < nbElem/2 + 1){
        somme += tableau[i];
        i++;
    }
    i--;
    return i;
}

void ajoutValeurHist(int* hist, int taille, int valeur){
    if(taille <= valeur || valeur < 0){
        printf("Erreur ajoutValeurHist");
        exit(0);
    }
    hist[valeur] = hist[valeur]+1;
}

void enleveValeurHist(int* hist, int taille, int valeur){
    if(taille <= valeur || valeur < 0){
        printf("Erreur ajoutValeurHist");
        exit(0);
    }
    if(hist[valeur] == 0){
        printf("Imposible d'enlever la valeur");
        exit(0);
    }
    hist[valeur] = hist[valeur]-1;
}

void reinitialisationHist(int* hist, int taille, double** image, int nl, int nc, int tailleMasque, int i){
    for(int j=0; j<taille; j++){
        hist[j]=0;
    }
    if(i-tailleMasque < 0 || i+tailleMasque >= nl){
        printf("Erreur reinitialisationHist");
        exit(0);
    }
    int n = 2*tailleMasque + 1;
    for(int p=0; p<n; p++){
        for(int k=0; k<n; k++){
            hist[(int) image[i-tailleMasque+p][k]] += 1;
        }
    }
}

void etapeSuivanteMedian(int* hist, int taille, double** image, int nl, int nc, int i, int j, int tailleMasque){
    int n = 2*tailleMasque +1;
    if(i-tailleMasque < 0 || i + tailleMasque >=nl || j-tailleMasque-1<0 || j+tailleMasque>=nc){
        printf("Erreur etapeSuivanteMedian");
        exit(0);
    }
    for(int k=0; k<n; k++){
        enleveValeurHist(hist, taille, (int)image[i-tailleMasque+k][j-tailleMasque-1]);
        ajoutValeurHist(hist, taille, (int)image[i-tailleMasque+k][j+tailleMasque]);
    }
}

double** filtreMedian(double** imageBruite, int nl, int nc, int tailleMasque){
    int n = 2*tailleMasque +1;
    int max = maxImage(imageBruite,nl,nc);
    max ++;
    int *hist = malloc(max*sizeof(int));
    for(int i=0; i<max; i++){
        hist[i]=0;
    }
    double** imageResult = alloue_image_double(nl,nc);
    for(int i= tailleMasque; i< nl - tailleMasque; i++){
        reinitialisationHist(hist, max, imageBruite, nl, nc, tailleMasque, i);
        imageResult[i][tailleMasque] = calculMedian(hist, n*n);
        for(int j=tailleMasque+1; j< nc-tailleMasque; j++){
            etapeSuivanteMedian(hist, max, imageBruite, nl, nc, i, j, tailleMasque);
            imageResult[i][j]=calculMedian(hist,n*n);
        }
    }
    free(hist);
    return imageResult;
}

double calcul_psnr_median(double **image1, double **image2, int nl, int nc, int taille){
    double** image_filtre = filtreMedian(image2, nl, nc, taille);
    double result = psnr_double(image1, image_filtre, nl, nc);
    libere_image_double(image_filtre);
    return result;
}
//
// int main (int ac, char **av) {
//     int nl, nc;
//     unsigned char **im1, **im4, **im3;
//     im1=lectureimagepgm(av[1],&nl,&nc);
//     im4 = lectureimagepgm(av[2],&nl,&nc);
//     im3 = lectureimagepgm(av[3],&nl,&nc);
//     double**im2=imuchar2double(im1,nl,nc);
//     double**im5=imuchar2double(im4,nl,nc);
//     double**im6=imuchar2double(im3,nl,nc);
//     printf("%f\n", calcul_psnr_median(im2, im5, nl, nc, 0));
//     printf("%f\n", psnr_double(im2, im5, nl, nc));
//     printf("%f\n", psnr_double(im2, im6, nl, nc));
//     libere_image(im1);
//     libere_image(im4);
//     libere_image(im3);
//     libere_image_double(im2);
//     libere_image_double(im5);
//     libere_image_double(im6);
//     return 0;
// }
