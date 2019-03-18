CFLAGS=-c -g -std=gnu99 -O4
LDFLAGS=-lm

main_conv_spat: main_conv_spat.o conv_spat.o pgm.o
	gcc $(LDFLAGS)  $^  -o $@

comparaison_deux_methodes: comparaison_deux_methodes.o filtrage_gaussien.o conv_spat.o pgm.o fft.o
	gcc $(LDFLAGS)  $^  -o $@

essai1: essai1.o pgm.o
	gcc $(LDFLAGS)  $^  -o $@

essai2: essai2.o pgm.o fft.o
	gcc $(LDFLAGS)  $^  -o $@

filtrageGaussien: filtrage_gaussien.o pgm.o fft.o
	gcc $(LDFLAGS)  $^  -o $@

filtrageGaussien.o: filtrage_gaussien.c pgm.h fft.h math.h
	gcc -o filtrage_gaussien.o -c filtrage_gaussien.c -W -Wall -ansi -pedantic

mesureTemps: mesureTemps.o filtrage_gaussien.o conv_spat.o pgm.o fft.o
	gcc $(LDFLAGS)  $^  -o $@
	
clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello

%.o:%.c
	gcc $(CFLAGS) $<
