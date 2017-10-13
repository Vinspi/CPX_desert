CC = gcc
CFLAGS = -w -Wall -std=c99

all: verif_desert verif_maximal calcul_maximum_exact calcul_maximum_inexact calcul_maximal


verif_desert: verif_desert.o util.o
	$(CC) -o verif_desert verif_desert.o util.o

verif_maximal: verif_maximal.o util.o
	$(CC) -o verif_maximal verif_maximal.o util.o

calcul_maximum_exact: calcul_maximum_exact.o util.o
	$(CC) -o calcul_maximum_exact calcul_maximum_exact.o util.o

calcul_maximum_inexact: calcul_maximum_inexact.o util.o
	$(CC) -o calcul_maximum_inexact calcul_maximum_inexact.o util.o

calcul_maximal: calcul_maximal.o util.o
	$(CC) -o calcul_maximal calcul_maximal.o util.o

util.o : util.c
	$(CC) -o util.o -c util.c $(CFLAGS)

verif_desert.o: verif_desert.c
	$(CC) -o verif_desert.o -c verif_desert.c $(CFLAGS)

verif_maximal.o: verif_maximal.c
	$(CC) -o verif_maximal.o -c verif_maximal.c $(CFLAGS)

calcul_maximal.o: calcul_maximal.c
	$(CC) -o calcul_maximal.o -c calcul_maximal.c $(CFLAGS)

calcul_maximum_exact.o: calcul_maximum_exact.c
	$(CC) -o calcul_maximum_exact.o -c calcul_maximum_exact.c $(CFLAGS)

calcul_maximum_inexact.o: calcul_maximum_inexact.c
	$(CC) -o calcul_maximum_inexact.o -c calcul_maximum_inexact.c $(CFLAGS)




clean:
	rm *.o

mr-proper:
	clean && rm cplx_graph_desert
