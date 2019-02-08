#include "conv_spat.h"

int main (int ac, char **av) {  /* av[1] contient le nom de l'image, av[2] le nom du resultat . */
  if (ac < 3) {printf("Usage : %s entree sortie \n",av[0]); exit(1); }

  return EXIT_SUCCESS;
}
