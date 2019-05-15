all: client.out server.out

client.out: client.c common.o cdc.o
	gcc -Wall -g -o client.out client.c common.o cdc.o -lm -lpthread

server.out: serveur.c common.o
	gcc -Wall -g -o serveur.out serveur.c common.o -lm -lpthread

cdc.o: cdc.c
	gcc -Wall -g -c -o cdc.o cdc.c -lm -lpthread

common.o: common.c
	gcc -Wall -g -c -o common.o common.c -lm -lpthread

clean:
	rm -rf *.o

mrproper:
	rm -rf *.o *.out
