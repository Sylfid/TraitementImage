#include <math.h>
#include "filtrage_gaussien.h"
#include "pgm.h"


unsigned char ** filtrageGaussien(unsigned char** entree, int nl, int nc, double sigma) {
    int i,j;
    int oldnl, oldnc;
    unsigned char** result;
    double** im7=NULL, ** im4=NULL, ** im5=NULL, ** im6=NULL, **im8=NULL, **im9=NULL, **im10=NULL, **im11=NULL;
    double** im12=NULL, **im13=NULL, **im14=NULL, **im15=NULL;
    double a=0., b=0.;


    oldnl = nl; oldnc = nc;
    double** im3 = imuchar2double(entree, nl, nc);
    im7 = padimdforfft(im3, &nl, &nc);

    double** filtreGaussienReel = alloue_image_double(nl,nc);
    double** filtreGaussienImag = alloue_image_double(nl,nc);

    for(i=0; i<nl; i++){
        for(j=0; j<nc; j++){
            a = ((double) i - (((double) nl)/ 2.))/((double) nl);
            b = ((double) j - (((double) nc)/ 2.))/((double) nc);
            filtreGaussienReel[i][j]=exp(-2.*M_PI*M_PI*sigma*sigma*(a*a + b*b));
            filtreGaussienImag[i][j]=0;
        }
    }
    

    im4 = alloue_image_double(nl,nc); im5 = alloue_image_double(nl,nc);
    im6 = alloue_image_double(nl,nc); im8 = alloue_image_double(nl,nc);
    im9 = alloue_image_double(nl,nc);
    im10 = alloue_image_double(nl,nc);
    im11 = alloue_image_double(nl,nc);
    im12 = alloue_image_double(nl,nc);
    im13 = alloue_image_double(nl,nc);
    im14 = alloue_image_double(nl,nc);
    im15 = alloue_image_double(nl,nc);


    fft(im7, im4, im5, im6, nl, nc);
    fftshift(im5, im6, im8, im9, nl, nc);
    multiplication(im8, im9, filtreGaussienReel, filtreGaussienImag, im10, im11, nl, nc);
    fftshift(im10, im11, im12, im13, nl, nc);
    ifft(im12, im13, im14, im15, nl, nc);


    //libere_image_double(im3);
    libere_image_double(im4);
    libere_image_double(im5);
    libere_image_double(im6);
    libere_image_double(im7);
    libere_image_double(im8);
    libere_image_double(im9);
    libere_image_double(im10);
    libere_image_double(im11);
    libere_image_double(im12);
    libere_image_double(im13);
    libere_image_double(im15);
    libere_image_double(filtreGaussienReel);
    libere_image_double(filtreGaussienImag);
    result = imdouble2char(im14, nl, nc);
    libere_image_double(im14);
    return result;
}

void multiplication (double** entreReel1, double** entreImag1,
        double** entreReel2, double** entreImag2,
        double** sortieReel, double** sortieImag,
        int nl, int nc){
    int i,j;
    if (sortieReel==NULL) sortieReel=alloue_image_double(nl,nc);
    if (sortieImag == NULL) sortieImag = alloue_image_double(nl,nc);
    for(i=0;i<nl;i++){
        for(j=0;j<nc;j++){
           sortieReel[i][j] = entreReel1[i][j]*entreReel2[i][j] - entreImag1[i][j]*entreImag2[i][j];
           sortieImag[i][j] = entreReel1[i][j]*entreImag2[i][j] + entreReel2[i][j]*entreImag1[i][j];
        }
    }
}

double psnr_maison(unsigned char** im1, unsigned char** im2, int nl, int nc){ //Calcul du PSNR de deux images
    double result = 0.0;
    double** im1d=imuchar2double(im1,nl,nc), **im2d=imuchar2double(im2, nl, nc);
    for(int i=0; i<nl; i++){
        for(int j=0; j<nc; j++){
            result += (im1d[i][j]-im2d[i][j]) *(im1d[i][j]-im2d[i][j]);
        }
    }
    result = 10*(log(255*255)+log(nc*nl)-log(result));
    libere_image_double(im1d);
    libere_image_double(im2d);
    return result;

}
//Calcul le PSNR entre im2 et im1 filtré par filtre gaussien
double psnr_gaussien(unsigned char** im1,unsigned char** im2, int nl, int nc, float sigma){
    unsigned char ** tampon = filtrageGaussien(im1, nl, nc, sigma);
    double result = psnr_maison(im2, tampon, nl, nc);
    libere_image(tampon);
    return result;
}

void filtrage_write(char* filename_source, char* filename_dest, double sigma){
    int nl,nc;
    unsigned char ** im_source_char=NULL;
    double **im_source_double=NULL;
    double **im_dest_double=NULL;
    unsigned char ** im_dest_char=NULL;

    im_source_char = lectureimagepgm(filename_source,&nl,&nc);

    im_dest_char = filtrageGaussien(im_source_char, nl, nc, sigma);

    ecritureimagepgm(filename_dest, im_dest_char, nl, nc);

    //liberation
    libere_image(im_source_char);
    //libere_image_double(im_source_double);
    //libere_image_double(im_dest_double);
    libere_image(im_dest_char);
}

double** convolution(double** image, double** filtre, int nl, int nc, int n){ //Calcul la convolution entre deux images

    double** tampon = alloue_image_double(nl,nc);
    double somme=0;
    double valeurIm = 0;
    if((int) (n/2) == (int) ((n+1)/2)){printf("Convolution n pair");exit(0);}
    int a = (n+1)/2;
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
                    somme += image[valeurx][valeury]*filtre[k][p]; 
                }
            }
            tampon[i][j]=somme;
        }
    }
    return tampon;
}

