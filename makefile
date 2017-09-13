CC = gcc
CFLAGS = -w -Wall -std=c99

all: cplx_graph_desert

cplx_graph_desert.o: cplx_graph_desert.c
	$(CC) -o cplx_graph_desert.o -c cplx_graph_desert.c $(CFLAGS)

cplx_graph_desert:cplx_graph_desert.o
	$(CC) -o cplx_graph_desert cplx_graph_desert.o
