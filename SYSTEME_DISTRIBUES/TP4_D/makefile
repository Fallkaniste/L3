all: client.out server.out

client.out: client.c common.o
	gcc -Wall -g -o client.out client.c common.o

server.out: serveur.c calculs.o common.o
	gcc -Wall -g -o serveur.out serveur.c calculs.o common.o

calculs.o: calculs.c
	gcc -Wall -g -c -o calculs.o calculs.c

common.o: common.c
	gcc -Wall -g -c -o common.o common.c

clean:
	rm -rf *.o

mrproper:
	rm -rf *.o *.out
