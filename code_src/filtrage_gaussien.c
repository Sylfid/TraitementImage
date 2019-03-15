#include <math.h>
#include "filtrage_gaussien.h"
#include "pgm.h"


unsigned char ** filtrageGaussien(unsigned char** entree, int nl, int nc, double sigma) { 
    int i,j;
    int oldnl, oldnc;
    unsigned char** result;
    double** im7=NULL, ** im4=NULL, ** im5=NULL, ** im6=NULL, **im8=NULL, **im9=NULL, **im10=NULL, **im11=NULL;
    double** im12=NULL, **im13=NULL, **im14=NULL, **im15=NULL;
    double** filtreGaussienReel = alloue_image_double(nl,nc);
    double** filtreGaussienImag = alloue_image_double(nl,nc);
    double a=0., b=0.;
    for(i=0; i<nl; i++){
        for(j=0; j<nc; j++){
            a = ((double) i - (((double) nl)/ 2.))/((double) nl);
            b = ((double) j - (((double) nc)/ 2.))/((double) nc);
            filtreGaussienReel[i][j]=exp(-2.*M_PI*M_PI*sigma*sigma*(a*a + b*b));
            filtreGaussienImag[i][j]=0;
        }
    }

    oldnl = nl; oldnc = nc;
    double** im3 = imuchar2double(entree, nl, nc);
    im7 = padimdforfft(im3, &nl, &nc); 

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

double psnr_maison(unsigned char** im1, unsigned char** im2, int nl, int nc){
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

double psnr_gaussien(unsigned char** im1,unsigned char** im2, int nl, int nc, float sigma){
    unsigned char ** tampon = filtrageGaussien(im1, nl, nc, sigma);
    double result = psnr_maison(im2, tampon, nl, nc);
    libere_image(tampon);
    return result;
}

