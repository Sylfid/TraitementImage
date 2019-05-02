CFLAGS=-c -g -std=gnu99 -O4
LDFLAGS=-lm

essai1: essai1.o pgm.o
	gcc $(LDFLAGS)  $^  -o $@

essai2: essai2.o pgm.o fft.o
	gcc $(LDFLAGS)  $^  -o $@

test_estimation_bruit: test_estimation_bruit.o estimation_bruit.o pgm.o convolution.o util.o
	gcc $(LDFLAGS)  $^  -o $@

test_bilateral:test_bilateral.o filtre_bilateral.o pgm.o convolution.o util.o divers.o estimation_bruit.o
	gcc $(LDFLAGS)  $^  -o $@

test_NL_means:test_NL_means.o filtre_NL_means.o pgm.o convolution.o util.o divers.o estimation_bruit.o
	gcc $(LDFLAGS)  $^  -o $@

test_comparaison_psnr:test_comparaison_psnr.o filtreMedian.o filtre_adpt_rec.o filtre_bilateral.o filtre_NL_means.o pgm.o convolution.o util.o divers.o estimation_bruit.o
	gcc $(LDFLAGS)  $^  -o $@


clean:
	rm -rf *.o

mrproper: clean
	rm -rf hello

%.o:%.c
	gcc $(CFLAGS) $<
