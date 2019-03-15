all: client.out server.out

client.out: client.c calculs.o common.h
	gcc -Wall -g -o client.out client.c calculs.o common.h

server.out: serveur.c calculs.o common.h
	gcc -Wall -g -o serveur.out serveur.c calculs.o common.h

calculs.o: calculs.c
	gcc -Wall -g -c -o calculs.o calculs.c

clean:
	rm -rf *.o
