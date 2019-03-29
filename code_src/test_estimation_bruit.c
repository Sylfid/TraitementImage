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

/*
    donne un tableau sur les images voulu
*/
int main(){
    //fichier d'ecriture des resultats
    FILE* fichier = fopen("resultMethod1.txt", "w+");

    fprintf(fichier, "ecart type attendu \tecart type trouve \t(t=10 p=0.5%%) \t(t=15 p=50%%) \n");
    //on itere sur tout les images
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
    fclose(fichier);
    return EXIT_SUCCESS;
}
