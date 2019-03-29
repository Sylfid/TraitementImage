import os                           # os function, i.e. checking file status
from itertools import cycle
import sys

# External, non built-in modules
import numpy as np                  # all matrix manipulations & OpenGL args
import matplotlib.pyplot as plt

def main():
    """ create a window, add scene objects, then run rendering loop """

    mon_fichier = open("resultTime.txt", "r")

    contenu = mon_fichier.read()
    nbNombre = 198
    sigma = np.zeros(nbNombre)
    result = np.zeros(nbNombre)
    result2 = np.zeros(nbNombre)
    result3 = np.zeros(nbNombre)
    point = 0
    space = 0
    enter = 0
    carSpe = 0
    for i in contenu:
        carSpe = 0
        if i == ' ':
            space += 1
            point = 0
            carSpe = 1

        if i == '\n':
            enter += 1
            space=0
            point = 0
            carSpe = 1

        if i == '.' or point != 0:
            point += 1

        if enter < nbNombre:

            if space == 1 and point == 0 and carSpe == 0:
                result[enter] = float(i)+ 10.*result[enter]

            if space == 1 and point > 1 and carSpe == 0:
                result[enter] = result[enter]+float(i)*(10.**(-point))

            if space == 2 and point == 0 and carSpe == 0:
                result2[enter] = float(i)+ 10.*result2[enter]

            if space == 2 and point > 1 and carSpe == 0:
                result2[enter] = result2[enter]+float(i)*(10.**(-point))

            if space == 3 and point == 0 and carSpe == 0:
                result3[enter] = float(i)+ 10.*result3[enter]

            if space == 3 and point > 1 and carSpe == 0:
                result3[enter] = result3[enter]+float(i)*(10.**(-point))

            if space == 0 and point == 0 and carSpe == 0:
                sigma[enter] = float(i)+ 10.*sigma[enter]

            if space == 0 and point > 1 and carSpe == 0:
                sigma[enter] = sigma[enter]+ float(i)*(10.**(-point))

    plt.plot(sigma, result, sigma, result2)#, sigma, result3)
    plt.show()








if __name__ == '__main__':
    main()                     # main function keeps variables locally scoped
