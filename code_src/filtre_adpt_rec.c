#include <math.h>
#include "util.h"
#include "pgm.h"
#include "util.h"

double calcul_w(double** image, int nl, int nc, int x, int y){
    double resultat = get_valeur(image, nl, nc, x+1, y) - get_valeur(image, 
            nl, nc, x-1, y); 
    resultat = resultat * resultat;
    double resultat2 = get_valeur(image, nl, nc, x, y+1) - get_valeur(image, 
            nl, nc, x, y-1);
    double seuil = 30.;
    resultat2 = resultat2 * resultat2;
    resultat = (resultat + resultat2)/(2*seuil*seuil);
    resultat = exp(-resultat);
    return resultat;
}
    

double** copie_image(double** image, int nl, int nc){
    double** result = alloue_image_double(nl,nc);
    for(int i=0; i<nl; i++){
        for(int j=0; j<nc; j++){
            result[i][j] = image[i][j];
        }
    }
    return result;
}

double calcul_pixel_filtre(double** image, int nl, int nc, int x, int y){
        double somme =0;
        double somme2 =0;
        for(int i=-1; i<2; i++){
            for(int j=-1; j<2; j++){
                somme2 += calcul_w(image, nl, nc, x+i, y+j);
                somme += calcul_w(image, nl, nc, x+i, y+j) * 
                    get_valeur(image, nl, nc, x+i, y+j);
            }
        }
        return somme/somme2;
}


/*Calcule le filtre adaptatif recursif en changeant l'image entree*/

void filtre_adaptatif_recursif_nomodif(double** image, int nl, int nc, int nbIter){
    if(nbIter > 0){
        double** image_tampon = copie_image(image, nl, nc);
        for(int i=0; i<nl; i++){
            for(int j=0; j<nc; j++){
                image[i][j] = calcul_pixel_filtre(image_tampon, nl, nc, i, j);
            }
        }
        libere_image_double(image_tampon);
        filtre_adaptatif_recursif_nomodif(image, nl, nc, nbIter-1);
    }
}

/*Calcule le filtre adaptatif recursif sans changer l'image entree*/

double** filtre_adaptatif_recursif(double** image, int nl, int nc, int nbIter){
    if(nbIter == 0){
        return image;
    }
    double** image_result = copie_image(image, nl, nc);
    filtre_adaptatif_recursif_nomodif(image_result, nl, nc, nbIter);
    return image_result;
}



int main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
    int nl, nc;
    unsigned char **im1;
    im1=lectureimagepgm(av[1],&nl,&nc);
    double**im2=imuchar2double(im1,nl,nc);
    double** im3 = filtre_adaptatif_recursif(im2, nl, nc, 1000);
    unsigned char **im4 = imdouble2char(im3, nl, nc);
    ecritureimagepgm(av[2],im4,nl,nc);
    libere_image(im1);
    libere_image_double(im2);
    libere_image_double(im3);
    return 0;
}

