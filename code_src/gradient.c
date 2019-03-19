#include <math.h>
#include "filtrage_gaussien.h"
#include "pgm.h"
#define M_PI 3.14159265358979323846


double ** filtre_prewit_x(){
    
    double** filtre = alloue_image_double(3,3);
    for(int i=0;i<3;i++){
        filtre[i][0]=-1.0;
    }
    for(int i=0;i<3;i++){
        filtre[i][1]=0.0;
    }
    for(int i=0;i<3;i++){
        filtre[i][2]=1.0;
    }
    return filtre;
}

double ** filtre_prewit_y(){
    
    double** filtre = alloue_image_double(3,3);
    for(int i=0;i<3;i++){
        filtre[0][i]=-1.0;
    }
    for(int i=0;i<3;i++){
        filtre[1][i]=0.0;
    }
    for(int i=0;i<3;i++){
        filtre[2][i]=1.0;
    }
    return filtre;
}




int main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
    int nb,nl,nc, oldnl,oldnc;
    unsigned char ** im1=NULL;

    if (ac < 2) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
    /* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
    im1=lectureimagepgm(av[1],&nl,&nc);
    if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
    double** im2 = imuchar2double(im1, nl, nc);
    double** im3 = imuchar2double(im1, nl, nc);
    double** filtre_x = filtre_prewit_x();
    double** filtre_y = filtre_prewit_y();
    double** Gx = convolution(im3, filtre_x, nl, nc, 3); 
    double** Gy = convolution(im3, filtre_y, nl, nc, 3); 
    double pi = -M_PI/4;
    double result_pi = 0.;
    double min =0;
    for(int i=1; i<nl-1;i++){
        for(int j=1; j<nc-1; j++){

            if(Gx[i][j]==0){
               if(im2[i][j] < im2[i][j+1] || im2[i][j] < im2[i][j-1]){
                    im3[i][j]=0.0;
               } 
            }
            else{
                arctan = atan(Gy[i][j]/Gx[i][j]);
                for(int p=0; p<5; p++){
                    if(min > abs(arctan - pi)){
                        min = abs(artan-pi);
                        result_pi = pi;
                        pi = pi + M_PI/4;
                    }
                }
            }
        }
    }
    libere_image_double(Gx);
    libere_image_double(Gy);
    libere_image_double(filtre_x);
    libere_image_double(filtre_y);
    libere_image_double(im3);
    libere_image(im1);

}


