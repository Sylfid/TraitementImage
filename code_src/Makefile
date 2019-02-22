CFLAGS=-c -g -std=gnu99 -O4
LDFLAGS=-lm

main_conv_spat: main_conv_spat.o conv_spat.o pgm.o
	gcc $(LDFLAGS)  $^  -o $@

essai1: essai1.o pgm.o
	gcc $(LDFLAGS)  $^  -o $@

essai2: essai2.o pgm.o fft.o
	gcc $(LDFLAGS)  $^  -o $@

%.o:%.c
	gcc $(CFLAGS) $<
