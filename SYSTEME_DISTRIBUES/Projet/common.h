#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <time.h>


#define L_PSEUDO_MAX    32
#define MULTICAST_ADR   "224.0.0.0"
#define MULTICAST_PORT  10001
#define NB_CLIENT_MAX   4

//CANSI (terminal)
#define BOLD          "\033[1m"
#define UNDERLINE     "\033[4m"
#define FONT_RESET    "\033[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_RESET   "\x1b[0m"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct ip_mreq ip_mreq;

/*static struct option options[] =
{
  {"help",0,NULL,'h'},
  {"version",0,NULL,'v'}
};*/

typedef struct
{
  sockaddr_in adr;
  char pseudo[L_PSEUDO_MAX];
  int id;
} info_client;

int creerSocketTCP(int port);
int calculerID(char* pseudo, int nbClients);
void afficherClients(info_client infoClients[], int nbClients, int id);
void removeClient(info_client infoClients[], int nbClients, int id);

#endif
