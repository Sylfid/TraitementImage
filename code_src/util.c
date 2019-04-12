#include "util.h"

/*
    obtient la valeur de l'image en x et y
    Si ces derniers depassent de l'image alors : (IMPLEMENTATION COURANTE)
        -on prolonge par continuite l'image
*/
double get_valeur(double** image, int nl, int nc, int x, int y){
    int x_f;
    int y_f;
    //x final de reference
    if(x >= nl){
        x_f = nl-1;
    } else if (x < 0){
        x_f = 0;
    } else {
        x_f = x;
    }
    //y final de reference
    if(y >= nc){
        y_f = nl-1;
    } else if (y < 0){
        y_f = 0;
    } else {
        y_f = y;
    }
    //on retourne le point, prolonge ou non
    return image[x_f][y_f];
}
