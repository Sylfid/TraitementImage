#include <math.h>
#include "filtrage_gaussien.h"
#include "pgm.h"
#define M_PI 3.14159265358979323846

double ** filtre_laplacien(){ //Création du masque permettant d'obtenir le laplacien
    
    double** filtre = alloue_image_double(3,3);
    filtre[1][0]=1.;
    filtre[0][1]=1.;
    filtre[1][2]=1.;
    filtre[2][1]=1.;
    filtre[0][0]=0.;
    filtre[0][2]=0.;
    filtre[2][2]=0.;
    filtre[2][0]=0.;
    filtre[1][1]=-4.;
    return filtre;
}

int main (int ac, char **av) {  

    int nb,nl,nc, oldnl,oldnc;
    unsigned char ** im1=NULL;
    if (ac < 2) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
    /* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
    im1=lectureimagepgm(av[1],&nl,&nc);
    if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
    /*Application d'un filtre gaussien*/
    unsigned char **imfiltre = filtrageGaussien(im1,nl,nc,3.2);
    double** im3 = imuchar2double(imfiltre, nl, nc);
    double** filtre_lap = filtre_laplacien();
    //Obtention du laplacien
    double** laplace = convolution(im3, filtre_lap, nl, nc, 3); 
    //Recherche des x,y pour lesquel le laplacien s'annule
    for(int i=0; i<nl-1; i++){
        for(int j=0; j<nc-1; j++){
            if(laplace[i][j]*laplace[i][j+1]<=0 || 
                    laplace[i][j]*laplace[i+1][j]<=0){
                im3[i][j]=0;
            }
        }
    }
    ecritureimagepgm(av[2],crop(imdouble2uchar(im3,nl,nc),0,0,nl,nc),nl,nc);
    libere_image_double(im3);
    libere_image_double(filtre_lap);
    libere_image_double(laplace);
    libere_image(im1);
    libere_image(imfiltre);

}
