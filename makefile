CC = gcc
CFLAGS = -w -Wall -std=c99

all: cplx_graph_desert



cplx_graph_desert.o: cplx_graph_desert_r2.c
	$(CC) -o cplx_graph_desert.o -c cplx_graph_desert_r2.c $(CFLAGS)

cplx_graph_desert:cplx_graph_desert.o
	$(CC) -o cplx_graph_desert cplx_graph_desert.o


verif_desert: verif_desert.o
	$(CC) -o verif_desert verif_desert.o

util.o : util.c
	$(CC) -o util.o -c util.c $(CFLAGS)

verif_desert.o: verif_desert.c
	$(CC) -o verif_desert.o -c verif_desert.c $(CFLAGS)




clean:
	rm *.o

mr-proper:
	clean && rm cplx_graph_desert
