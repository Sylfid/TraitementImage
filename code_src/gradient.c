#include <math.h>
#include "filtrage_gaussien.h"
#include "pgm.h"
#define M_PI 3.14159265358979323846


double ** filtre_prewit_x(){ //Création du masque prewit selon x
    
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

double ** filtre_prewit_y(){ //Création du masque prewit selon y
    
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


double module(double ** a, double **b, int i, int j){ //Calcul du module
    return sqrt(a[i][j]*a[i][j] + b[i][j]*b[i][j]);
}


int main (int ac, char **av) { //Extraction des contours de l'image av[1] 
    int nb,nl,nc, oldnl,oldnc;
    unsigned char ** im1=NULL;
    if (ac < 2) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
    /* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
    im1=lectureimagepgm(av[1],&nl,&nc);
//On filtre l'image par filtre gaussien
    unsigned char **filtre = filtrageGaussien(im1,nl,nc,2.5);
    if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
    double** im2 = imuchar2double(filtre, nl, nc);
    double** im3 = imuchar2double(filtre, nl, nc);
    double** filtre_x = filtre_prewit_x();
    double** filtre_y = filtre_prewit_y();
    double** Gx = convolution(im3, filtre_x, nl, nc, 3); //On applique le masque pour avoir le gradient
    double** Gy = convolution(im3, filtre_y, nl, nc, 3); 
    double pi = -M_PI/2;
    int result_pi = 0;
    double min =0.;
    double arctan = 0.;
    double moduledouble=0.;
    int module1=0;
    int module2=0;
    double seuilbas = 60.;
    double seuilhaut=100.;
    for(int i=1; i<nl-1;i++){
        for(int j=1; j<nc-1; j++){
            moduledouble = module(Gx,Gy,i,j);
            module1=(int)moduledouble;
            module2=(int)moduledouble;
            /*Pour éviter les débordements*/
            if(i-module1<0){module1=i;}
            if(i+module1 >= nl){module1=nl-i-1;}
            if(j-module2<0){module2=j;}
            if(j+module2 >= nc){module2=nc-j-1;}

            if(Gx[i][j]==0){
                if(module(Gx,Gy,i,j) < module(Gx,Gy,i,j+module2) || module(Gx,Gy,i,j)<module(Gx,Gy,i,j-module2)){
                    im3[i][j]=0.0;
                } 
            }
            else{
                /*Calcul de la direction la plus proche*/
                arctan = atan(Gy[i][j]/Gx[i][j]);
                for(int p=0; p<5; p++){
                    if(min > abs(arctan - pi)){
                        min = abs(arctan-pi);
                        result_pi = p;
                        pi = pi + M_PI/4;
                    }
                }
                /*Recherche des points maxima du gradient, mise à 0 si
                 * ce n'est pas le cas*/
                if(result_pi == 0 || result_pi == 4){
                    if(module(Gx,Gy,i,j) < module(Gx,Gy,i,j+module2) || module(Gx,Gy,i,j)<module(Gx,Gy,i,j-module2)){
                        im3[i][j]=0.0;
                    } 
                }
                if(result_pi == 1){
                    if(module(Gx,Gy,i,j) < module(Gx,Gy,i+module1,j+module2) || module(Gx,Gy,i,j)<module(Gx,Gy,i-module1,j-module2)){
                        im3[i][j]=0.0;
                    } 
                }
                if(result_pi == 2){
                    if(module(Gx,Gy,i,j) < module(Gx,Gy,i+module1,j) || module(Gx,Gy,i,j)<module(Gx,Gy,i-module1,j)){
                        im3[i][j]=0.0;
                    } 
                }
                if(result_pi == 3){
                    if(module(Gx,Gy,i,j) < module(Gx,Gy,i-module1,j+module2) || module(Gx,Gy,i,j)<module(Gx,Gy,i+module1,j-module2)){
                        im3[i][j]=0.0;
                    } 
                }
            }
            if(module(Gx,Gy,i,j) < seuilbas){
                im3[i][j]=0.;
            }
        }
    }
    int flag =0;
    /*Vérification des points restants*/
    for(int i=1; i<nl-1;i++){
        for(int j=1; j<nc-1; j++){
            if(im3[i][j]!=0.){
                if(module(Gx,Gy,i,j) > seuilbas && module(Gx,Gy,i,j)<seuilhaut){
                    for(int k=0;k<3;k++){
                        for(int p=0;p<3;p++){
                            if(im3[i][j] != 0.){flag=1;}
                        }
                    }
                    if(flag==0){im3[i][j]=0.;}
                    flag = 0;
                }
            }
        }
    }
    libere_image_double(Gx);
    libere_image_double(Gy);
    libere_image_double(filtre_x);
    libere_image_double(filtre_y);
    ecritureimagepgm(av[2],crop(imdouble2uchar(im3,nl,nc),0,0,nl,nc),nl,nc);
    libere_image_double(im2);
    libere_image_double(im3);
    libere_image(im1);

}


