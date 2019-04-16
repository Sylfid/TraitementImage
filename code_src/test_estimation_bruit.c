#include "estimation_bruit.h"
#include "pgm.h"

//les fichiers voulu
//bruit blanc
#define NB_IMAGE_BB 15
static const int ecart_type_real_BB[NB_IMAGE_BB] = {
    0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70
};
static char* IMAGE_BB[NB_IMAGE_BB] = {
    "../imagestp/formes2.pgm",
    "../imagestp/formes2bb5.pgm",
    "../imagestp/formes2bb10.pgm",
    "../imagestp/formes2bb15.pgm",
    "../imagestp/formes2bb20.pgm",
    "../imagestp/formes2bb25.pgm",
    "../imagestp/formes2bb30.pgm",
    "../imagestp/formes2bb35.pgm",
    "../imagestp/formes2bb40.pgm",
    "../imagestp/formes2bb45.pgm",
    "../imagestp/formes2bb50.pgm",
    "../imagestp/formes2bb55.pgm",
    "../imagestp/formes2bb60.pgm",
    "../imagestp/formes2bb65.pgm",
    "../imagestp/formes2bb70.pgm"
};
//bruit speckle
#define NB_IMAGE_S 15
static const int ecart_type_real_S[NB_IMAGE_S] = {
    0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70
};
static char* IMAGE_S[NB_IMAGE_S] = {
    "../imagestp/formes2.pgm",
    "../imagestp/formes2s5.pgm",
    "../imagestp/formes2s10.pgm",
    "../imagestp/formes2s15.pgm",
    "../imagestp/formes2s20.pgm",
    "../imagestp/formes2s25.pgm",
    "../imagestp/formes2s30.pgm",
    "../imagestp/formes2s35.pgm",
    "../imagestp/formes2s40.pgm",
    "../imagestp/formes2s45.pgm",
    "../imagestp/formes2s50.pgm",
    "../imagestp/formes2s55.pgm",
    "../imagestp/formes2s60.pgm",
    "../imagestp/formes2s65.pgm",
    "../imagestp/formes2s70.pgm"
};
//bruit poisson
#define NB_IMAGE_P 6
static const int ecart_type_real_P[NB_IMAGE_P] = {
    0, 1, 2, 3, 4, 5
};
static char* IMAGE_P[NB_IMAGE_P] = {
    "../imagestp/formes2.pgm",
    "../imagestp/formes2p1.pgm",
    "../imagestp/formes2p2.pgm",
    "../imagestp/formes2p3.pgm",
    "../imagestp/formes2p4.pgm",
    "../imagestp/formes2p5.pgm"
};
//bruit proivre et sel
#define NB_IMAGE_SP 14
static const int ecart_type_real_SP[NB_IMAGE_SP] = {
    0, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29
};
static char* IMAGE_SP[NB_IMAGE_SP] = {
    "../imagestp/formes2.pgm",
    "../imagestp/formes2sp5.pgm",
    "../imagestp/formes2sp7.pgm",
    "../imagestp/formes2sp9.pgm",
    "../imagestp/formes2sp11.pgm",
    "../imagestp/formes2sp13.pgm",
    "../imagestp/formes2sp15.pgm",
    "../imagestp/formes2sp17.pgm",
    "../imagestp/formes2sp19.pgm",
    "../imagestp/formes2sp21.pgm",
    "../imagestp/formes2sp23.pgm",
    "../imagestp/formes2sp25.pgm",
    "../imagestp/formes2sp27.pgm",
    "../imagestp/formes2sp29.pgm"
};

/*
    donne un tableau sur les images voulu
*/
int main(){
    //fichier d'ecriture des resultats
    FILE* fichier = fopen("resultTestEstimationBruit.txt", "w+");

    fprintf(fichier, "Image bruit blanc\n\n");
    fprintf(fichier, "ecart type attendu \tecart type trouve \t(t=10 p=0.5%%) \t(t=15 p=50%%) \n");
    //on itere sur tout les images bruit blanc
    for(unsigned int i=0; i<NB_IMAGE_BB; i++){
        int nl,nc;
        //on recupere l'image
        printf("on analyse l'image %s\n", IMAGE_BB[i]);
        unsigned char **im_source_char = lectureimagepgm(IMAGE_BB[i],&nl,&nc);
        //sous forme de double
        double **im_source_double = imuchar2double(im_source_char, nl, nc);
        //on recupere l'ecart pour t=10 p=0.005
        double et1 = estimation_bruit(im_source_double, nl, nc, 10, 0.005);
        //on recupere l'ecart pour t=15 p=0.5
        double et2 = estimation_bruit(im_source_double, nl, nc, 15, 0.5);
        //choix : et2 est meilleur pour un ecart type de 60+
        double et = (et2>60) ? et2 : et1;
        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \n", ecart_type_real_BB[i], et, et1, et2);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
    }

    fprintf(fichier, "\nImage speckle\n\n");
    fprintf(fichier, "ecart type attendu \tecart type trouve \t(t=10 p=0.5%%) \t(t=15 p=50%%) \n");
    //on itere sur tout les images speckle
    for(unsigned int i=0; i<NB_IMAGE_S; i++){
        int nl,nc;
        //on recupere l'image
        printf("on analyse l'image %s\n", IMAGE_S[i]);
        unsigned char **im_source_char = lectureimagepgm(IMAGE_S[i],&nl,&nc);
        //sous forme de double
        double **im_source_double = imuchar2double(im_source_char, nl, nc);
        //on recupere l'ecart pour t=10 p=0.005
        double et1 = estimation_bruit(im_source_double, nl, nc, 10, 0.005);
        //on recupere l'ecart pour t=15 p=0.5
        double et2 = estimation_bruit(im_source_double, nl, nc, 15, 0.5);
        //choix : et2 est meilleur pour un ecart type de 60+
        double et = (et2>60) ? et2 : et1;
        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \n", ecart_type_real_S[i], et, et1, et2);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
    }


    fprintf(fichier, "\nImage poisson\n\n");
    fprintf(fichier, "ecart type attendu \tecart type trouve \t(t=10 p=0.5%%) \t(t=15 p=50%%) \n");
    //on itere sur tout les images poisson
    for(unsigned int i=0; i<NB_IMAGE_P; i++){
        int nl,nc;
        //on recupere l'image
        printf("on analyse l'image %s\n", IMAGE_P[i]);
        unsigned char **im_source_char = lectureimagepgm(IMAGE_P[i],&nl,&nc);
        //sous forme de double
        double **im_source_double = imuchar2double(im_source_char, nl, nc);
        //on recupere l'ecart pour t=10 p=0.005
        double et1 = estimation_bruit(im_source_double, nl, nc, 10, 0.005);
        //on recupere l'ecart pour t=15 p=0.5
        double et2 = estimation_bruit(im_source_double, nl, nc, 15, 0.5);
        //choix : et2 est meilleur pour un ecart type de 60+
        double et = (et2>60) ? et2 : et1;
        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \n", ecart_type_real_P[i], et, et1, et2);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
    }

    fprintf(fichier, "\nImage proivre et sel\n\n");
    fprintf(fichier, "ecart type attendu \tecart type trouve \t(t=10 p=0.5%%) \t(t=15 p=50%%) \n");
    //on itere sur tout les images poisson
    for(unsigned int i=0; i<NB_IMAGE_SP; i++){
        int nl,nc;
        //on recupere l'image
        printf("on analyse l'image %s\n", IMAGE_SP[i]);
        unsigned char **im_source_char = lectureimagepgm(IMAGE_SP[i],&nl,&nc);
        //sous forme de double
        double **im_source_double = imuchar2double(im_source_char, nl, nc);
        //on recupere l'ecart pour t=10 p=0.005
        double et1 = estimation_bruit(im_source_double, nl, nc, 10, 0.005);
        //on recupere l'ecart pour t=15 p=0.5
        double et2 = estimation_bruit(im_source_double, nl, nc, 15, 0.5);
        //choix : et2 est meilleur pour un ecart type de 60+
        double et = (et2>60) ? et2 : et1;
        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \n", ecart_type_real_SP[i], et, et1, et2);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
    }

    fclose(fichier);
    return EXIT_SUCCESS;
}
