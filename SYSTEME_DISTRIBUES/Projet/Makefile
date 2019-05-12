all: client.out server.out

client.out: client.c common.o
	gcc -Wall -g -o client.out client.c common.o -lm

server.out: serveur.c common.o
	gcc -Wall -g -o serveur.out serveur.c common.o -lm

common.o: common.c
	gcc -Wall -g -c -o common.o common.c -lm

clean:
	rm -rf *.o

mrproper:
	rm -rf *.o *.out
