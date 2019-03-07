#include <math.h>
#include "filtrage_gaussien.h"
#include "pgm.h"


unsigned char** filtrageGaussien(double** entree, int nl, int nc) { 
    printf("Entree filtrageGaussien\n");
    int i,j;
    int oldnl, oldnc;
    int sigma = 5;
    unsigned char** result;
    double** im7, ** im4, ** im5, ** im6, **im8, **im9, **im10=NULL, **im11=NULL;
    double** im12, **im13, **im14, **im15;
    double** filtreGaussienReel = alloue_image_double(nl,nc);
    double** filtreGaussienImag = alloue_image_double(nl,nc);
    for(i=0; i<nl; i++){
        for(j=0; j<nc; j++){
            filtreGaussienReel[i][j]=exp(-2*M_PI*M_PI*sigma*sigma*(pow((i-(nl/2))/nl,2)+pow((j-(nc/2))/nc,2)));
            filtreGaussienImag[i][j]=0;
        }
    }
    oldnl = nl; oldnc = nc;
    im7 = padimdforfft(entree, &nl, &nc); 
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
    printf("Presque sortie\n");
    return result; 
}

void multiplication (double** entreReel1, double** entreImag1,
        double** entreReel2, double** entreImag2, 
        double** sortieReel, double** sortieImag, 
        int nl, int nc){
    printf("Entree Multiplication\n");
    int i,j;
    if (sortieReel==NULL) sortieReel=alloue_image_double(nl,nc);
    if (sortieImag == NULL) sortieImag = alloue_image_double(nl,nc);
    for(i=0;i<nl;i++){
        for(j=0;j<nc;j++){
           sortieReel[i][j] = entreReel1[i][j]*entreReel2[i][j] - entreImag1[i][j]*entreImag2[i][j];
           sortieImag[i][j] = entreReel1[i][j]*entreImag2[i][j] + entreReel2[i][j]*entreImag1[i][j];
        }
    }
    printf("Sortie Multiplication\n");
}

main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  int nb,nl,nc, oldnl,oldnc;
  unsigned char **im2=NULL,** im1=NULL;
  //double** im4,** im5, ** im6, ** im7, **im8, **im9,**im10;

  if (ac < 2) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }
	/* Lecture d'une image pgm dont le nom est passe sur la ligne de commande */
  im1=lectureimagepgm(av[1],&nl,&nc);
  if (im1==NULL)  { puts("Lecture image impossible"); exit(1); }
	/* Calcul de son inverse video */
  double**im3=imuchar2double(im1,nl,nc);
  im2=filtrageGaussien(im3, nl, nc);
	/* Sauvegarde dans un fichier dont le nom est passe sur la ligne de commande */
  ecritureimagepgm(av[2],im2,nl,nc);
  libere_image(im1);
  libere_image(im2);

}
