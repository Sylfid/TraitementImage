#include "estimation_bruit.h"
#include "filtreMedian.h"
#include "filtre_adpt_rec.h"
#include "filtre_bilateral.h"
#include "filtre_NL_means.h"
#include "pgm.h"
#include "util.h"

//les fichiers voulu
//bruit blanc
#define NB_IMAGE_BB 4 //15
static const int ecart_type_real_BB[NB_IMAGE_BB] = {
    //0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70
    5, 20, 40, 60
};
static char* IMAGE_BB[NB_IMAGE_BB] = {
    //"../imagestp/formes2.pgm",
    "../imagestp/formes2bb5.pgm",
    //"../imagestp/formes2bb10.pgm",
    //"../imagestp/formes2bb15.pgm",
    "../imagestp/formes2bb20.pgm",
    //"../imagestp/formes2bb25.pgm",
    //"../imagestp/formes2bb30.pgm",
    //"../imagestp/formes2bb35.pgm",
    "../imagestp/formes2bb40.pgm",
    //"../imagestp/formes2bb45.pgm",
    //"../imagestp/formes2bb50.pgm",
    //"../imagestp/formes2bb55.pgm",
    "../imagestp/formes2bb60.pgm",
    //"../imagestp/formes2bb65.pgm",
    //"../imagestp/formes2bb70.pgm"
};
//bruit speckle
#define NB_IMAGE_S 4 //15
static const int ecart_type_real_S[NB_IMAGE_S] = {
    //0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70
    5, 20, 40, 60
};
static char* IMAGE_S[NB_IMAGE_S] = {
    //"../imagestp/formes2.pgm",
    "../imagestp/formes2s5.pgm",
    //"../imagestp/formes2s10.pgm",
    //"../imagestp/formes2s15.pgm",
    "../imagestp/formes2s20.pgm",
    //"../imagestp/formes2s25.pgm",
    //"../imagestp/formes2s30.pgm",
    //"../imagestp/formes2s35.pgm",
    "../imagestp/formes2s40.pgm",
    //"../imagestp/formes2s45.pgm",
    //"../imagestp/formes2s50.pgm",
    //"../imagestp/formes2s55.pgm",
    "../imagestp/formes2s60.pgm",
    //"../imagestp/formes2s65.pgm",
    //"../imagestp/formes2s70.pgm"
};
//bruit poisson
#define NB_IMAGE_P 3 //6
static const int ecart_type_real_P[NB_IMAGE_P] = {
    //0, 1, 2, 3, 4, 5
    1, 3, 5
};
static char* IMAGE_P[NB_IMAGE_P] = {
    //"../imagestp/formes2.pgm",
    "../imagestp/formes2p1.pgm",
    //"../imagestp/formes2p2.pgm",
    "../imagestp/formes2p3.pgm",
    //"../imagestp/formes2p4.pgm",
    "../imagestp/formes2p5.pgm"
};
//bruit proivre et sel
#define NB_IMAGE_SP 5 //14
static const int ecart_type_real_SP[NB_IMAGE_SP] = {
    //0, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29
    5, 11, 17, 23, 29
};
static char* IMAGE_SP[NB_IMAGE_SP] = {
    //"../imagestp/formes2.pgm",
    "../imagestp/formes2sp5.pgm",
    //"../imagestp/formes2sp7.pgm",
    //"../imagestp/formes2sp9.pgm",
    "../imagestp/formes2sp11.pgm",
    //"../imagestp/formes2sp13.pgm",
    //"../imagestp/formes2sp15.pgm",
    "../imagestp/formes2sp17.pgm",
    //"../imagestp/formes2sp19.pgm",
    //"../imagestp/formes2sp21.pgm",
    "../imagestp/formes2sp23.pgm",
    //"../imagestp/formes2sp25.pgm",
    //"../imagestp/formes2sp27.pgm",
    "../imagestp/formes2sp29.pgm"
};

////////////////////////////////////////
//choix des parametres des filtres
//filtre median
#define tailleMasqueMedian 3
//filtre adaptif recursif
#define nbIterAdptRecur 100
#define seuilAdptRecur 10
//filtre bilateral
#define sigma1Bilateral 10
#define sigma2(bruit) 2*bruit
//filtre NLmeans
#define sigmaNL(bruit) bruit

/*
    donne un tableau sur les images voulu
    ATTENTION PROGRAMME TRES LONG
    ne pas hesiter a commenter/limiter le nombre
*/
int main(){
    //fichier d'ecriture des resultats
    FILE* fichier = fopen("resultTestComparaisonPSNR.txt", "w+");

    int nl,nc;

    unsigned char ** im_non_bruite_char = lectureimagepgm("../imagestp/formes2.pgm",&nl,&nc);
    double ** im_non_bruite_double = imuchar2double(im_non_bruite_char, nl, nc);

    fprintf(fichier, "Image bruit blanc\n\n");
    fprintf(fichier, "ecart type \tfiltre median \tfiltre adaptif recursif \tfiltre bilateral \tfiltre NLmeans \n");
    //on itere sur tout les images bruit blanc
    for(unsigned int i=0; i<NB_IMAGE_BB; i++){
        int nl,nc;
        //on recupere l'image
        printf("on analyse l'image %s\n", IMAGE_BB[i]);
        unsigned char **im_source_char = lectureimagepgm(IMAGE_BB[i],&nl,&nc);
        //sous forme de double
        double **im_source_double = imuchar2double(im_source_char, nl, nc);

        //filtre median
        double **im_res_median = filtreMedian(im_source_double, nl ,nc, tailleMasqueMedian);
        //filtre adaptif recursif
        double **im_res_adpt_rec = filtre_adaptatif_recursif(im_source_double, nl, nc, nbIterAdptRecur, seuilAdptRecur);
        //filtre bilateral
        double **im_res_bilateral = filtre_bilateral(im_source_double, nl, nc, sigma1Bilateral, sigma2(ecart_type_real_BB[i]));
        //filtre NLmeans
        double **im_res_NLmeans = filtre_NL_means_auto(im_source_double, nl, nc, sigmaNL(ecart_type_real_BB[i]));

        //on calcule tout les PSNR
        //filtre median
        double psnr_median = psnr_double(im_res_median, im_non_bruite_double, nl, nc);
        //filtre adaptif recursif
        double psnr_adpt_rec = psnr_double(im_res_adpt_rec, im_non_bruite_double, nl, nc);
        //filtre bilateral
        double psnr_bilateral = psnr_double(im_res_bilateral, im_non_bruite_double, nl, nc);
        //filtre NLmeans
        double psnr_NLmeans = psnr_double(im_res_NLmeans, im_non_bruite_double, nl, nc);

        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \t%f \n", ecart_type_real_BB[i], psnr_median, psnr_adpt_rec, psnr_bilateral, psnr_NLmeans);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
        libere_image_double(im_res_median);
        libere_image_double(im_res_adpt_rec);
        libere_image_double(im_res_bilateral);
        libere_image_double(im_res_NLmeans);
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

        //filtre median
        double **im_res_median = filtreMedian(im_source_double, nl ,nc, tailleMasqueMedian);
        //filtre adaptif recursif
        double **im_res_adpt_rec = filtre_adaptatif_recursif(im_source_double, nl, nc, nbIterAdptRecur, seuilAdptRecur);
        //filtre bilateral
        double **im_res_bilateral = filtre_bilateral(im_source_double, nl, nc, sigma1Bilateral, sigma2(ecart_type_real_S[i]));
        //filtre NLmeans
        double **im_res_NLmeans = filtre_NL_means_auto(im_source_double, nl, nc, sigmaNL(ecart_type_real_S[i]));

        //on calcule tout les PSNR
        //filtre median
        double psnr_median = psnr_double(im_res_median, im_non_bruite_double, nl, nc);
        //filtre adaptif recursif
        double psnr_adpt_rec = psnr_double(im_res_adpt_rec, im_non_bruite_double, nl, nc);
        //filtre bilateral
        double psnr_bilateral = psnr_double(im_res_bilateral, im_non_bruite_double, nl, nc);
        //filtre NLmeans
        double psnr_NLmeans = psnr_double(im_res_NLmeans, im_non_bruite_double, nl, nc);

        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \t%f \n", ecart_type_real_S[i], psnr_median, psnr_adpt_rec, psnr_bilateral, psnr_NLmeans);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
        libere_image_double(im_res_median);
        libere_image_double(im_res_adpt_rec);
        libere_image_double(im_res_bilateral);
        libere_image_double(im_res_NLmeans);
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

        //filtre median
        double **im_res_median = filtreMedian(im_source_double, nl ,nc, tailleMasqueMedian);
        //filtre adaptif recursif
        double **im_res_adpt_rec = filtre_adaptatif_recursif(im_source_double, nl, nc, nbIterAdptRecur, seuilAdptRecur);
        //filtre bilateral
        double **im_res_bilateral = filtre_bilateral(im_source_double, nl, nc, sigma1Bilateral, sigma2(ecart_type_real_P[i]));
        //filtre NLmeans
        double **im_res_NLmeans = filtre_NL_means_auto(im_source_double, nl, nc, sigmaNL(ecart_type_real_P[i]));

        //on calcule tout les PSNR
        //filtre median
        double psnr_median = psnr_double(im_res_median, im_non_bruite_double, nl, nc);
        //filtre adaptif recursif
        double psnr_adpt_rec = psnr_double(im_res_adpt_rec, im_non_bruite_double, nl, nc);
        //filtre bilateral
        double psnr_bilateral = psnr_double(im_res_bilateral, im_non_bruite_double, nl, nc);
        //filtre NLmeans
        double psnr_NLmeans = psnr_double(im_res_NLmeans, im_non_bruite_double, nl, nc);

        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \t%f \n", ecart_type_real_P[i], psnr_median, psnr_adpt_rec, psnr_bilateral, psnr_NLmeans);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
        libere_image_double(im_res_median);
        libere_image_double(im_res_adpt_rec);
        libere_image_double(im_res_bilateral);
        libere_image_double(im_res_NLmeans);
    }

    fprintf(fichier, "\nImage proivre et sel\n\n");
    fprintf(fichier, "ecart type attendu \tecart type trouve \t(t=10 p=0.5%%) \t(t=15 p=50%%) \n");
    //on itere sur tout les images poivre et sel
    for(unsigned int i=0; i<NB_IMAGE_SP; i++){
        int nl,nc;
        //on recupere l'image
        printf("on analyse l'image %s\n", IMAGE_SP[i]);
        unsigned char **im_source_char = lectureimagepgm(IMAGE_SP[i],&nl,&nc);
        //sous forme de double
        double **im_source_double = imuchar2double(im_source_char, nl, nc);

        //filtre median
        double **im_res_median = filtreMedian(im_source_double, nl ,nc, tailleMasqueMedian);
        //filtre adaptif recursif
        double **im_res_adpt_rec = filtre_adaptatif_recursif(im_source_double, nl, nc, nbIterAdptRecur, seuilAdptRecur);
        //filtre bilateral
        double **im_res_bilateral = filtre_bilateral(im_source_double, nl, nc, sigma1Bilateral, sigma2(ecart_type_real_SP[i]));
        //filtre NLmeans
        double **im_res_NLmeans = filtre_NL_means_auto(im_source_double, nl, nc, sigmaNL(ecart_type_real_SP[i]));

        //on calcule tout les PSNR
        //filtre median
        double psnr_median = psnr_double(im_res_median, im_non_bruite_double, nl, nc);
        //filtre adaptif recursif
        double psnr_adpt_rec = psnr_double(im_res_adpt_rec, im_non_bruite_double, nl, nc);
        //filtre bilateral
        double psnr_bilateral = psnr_double(im_res_bilateral, im_non_bruite_double, nl, nc);
        //filtre NLmeans
        double psnr_NLmeans = psnr_double(im_res_NLmeans, im_non_bruite_double, nl, nc);

        //on ecrit dans le fichier
        fprintf(fichier, "%d \t%f \t%f \t%f \t%f \n", ecart_type_real_SP[i], psnr_median, psnr_adpt_rec, psnr_bilateral, psnr_NLmeans);
        //on libere les images
        libere_image(im_source_char);
        libere_image_double(im_source_double);
        libere_image_double(im_res_median);
        libere_image_double(im_res_adpt_rec);
        libere_image_double(im_res_bilateral);
        libere_image_double(im_res_NLmeans);
    }

    fclose(fichier);
    return EXIT_SUCCESS;
}
