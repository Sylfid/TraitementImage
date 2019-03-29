#ifndef EST_BRUITH
#define EST_BRUITH

/*
    Algorithme 1 : estimationd du bruit
    renvoie la valeur de l’écart type du bruit
    valeur usuel :
        -taille_bloc=10, pourcentile = 0.5 si faible ecart type
        -taille_bloc=15, pourcentile = 0.5 si grand ecart type
*/

double estimation_bruit(double **image, int nl, int nc, int taille_bloc, double pourcentile);

#endif
