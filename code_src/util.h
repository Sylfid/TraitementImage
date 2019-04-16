#ifndef UTILH
#define UTILH

/*
    obtient la valeur de l'image en x et y
    Si ces derniers depassent de l'image alors : (IMPLEMENTATION COURANTE)
        -on prolonge par continuite l'image
*/
double get_valeur(double** image, int nl, int nc, int x, int y);

double EQM_maison(double** r1, double** r2, int nl, int nc);

#endif
