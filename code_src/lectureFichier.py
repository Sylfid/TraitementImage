import os                           # os function, i.e. checking file status
from itertools import cycle
import sys

# External, non built-in modules
import numpy as np                  # all matrix manipulations & OpenGL args
import matplotlib.pyplot as plt

def main():
    """ create a window, add scene objects, then run rendering loop """

    mon_fichier = open("resultPSNR.txt", "r")

    contenu = mon_fichier.read()
    sigma = np.zeros(190)
    result = np.zeros(190)
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
            point = 0
            carSpe = 1

        if i == '.' or point != 0:
            point += 1

        if enter < 190:

            if space > enter and point == 0 and carSpe == 0:
                result[enter] = float(i)+ 10*result[enter]

            if space > enter and point > 1 and carSpe == 0:
                result[enter] = result[enter] + float(i)*(10**point)

            if space == enter and point == 0 and carSpe == 0:
                sigma[enter] = float(i)+ 10*sigma[enter]

            if space == enter and point > 1 and carSpe == 0:
                sigma[enter] = sigma[enter] + float(i)*(10**point)
    
    #plot(sigma, result)








if __name__ == '__main__':
    main()                     # main function keeps variables locally scoped


