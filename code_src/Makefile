CFLAGS=-c -g -std=gnu99 -O4
LDFLAGS=-lm

essai1: essai1.o pgm.o
	gcc $(LDFLAGS)  $^  -o $@

essai2: essai2.o pgm.o fft.o
	gcc $(LDFLAGS)  $^  -o $@

clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello

%.o:%.c
	gcc $(CFLAGS) $<
