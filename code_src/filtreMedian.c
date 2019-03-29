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
            if(max < (int) image[i][j]){max = image[i][j];}
        }
    }
    return max;
}

int calculMedian(int* tableau, int tailleTableau, int nbElem){
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
    if(valeur == 0){
        printf("Imposible d'enlever la valeur");
        exit(0);
    }
    hist[valeur] = hist[valeur]-1;
}


double** filtreMedian(double** imageBruite, int nl, int nc, int tailleMasque){
    int n = 2*tailleMasque +1;
    int max = maxImage(imageBruite,nl,nc);
    int *hist = malloc(max*sizeof(int));
    for(int i=0; i<max; i++){
        hist[i]=0;
    }
    double** imageResult = alloue_image_double(nl,nc);
    for(int i= tailleMasque; i< nl - tailleMasque; i++){
        mediane = calculMedian(imageBruite, nl, nc, tailleMasque, i, tailleMasque);
        imageResult[i][tailleMasque] = mediane;
        for(int j=tailleMasque+1; j< nc-tailleMasque; j++){
            imageResult[i][j] = calculMedian(imageBruite, nl, nc, tailleMasque, i, j);
        }
    } 
}


int main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
    int* test = malloc(5*sizeof(int));
    for(int i=0; i<15; i++){
        test[i]=1;
    }
    printf("%d", calculMedian(test, 5, 15));
    return 0;
}



Si t'as oublié : faire une méthode créer hist et l'appeler dans la premier boucle de filtre Median
