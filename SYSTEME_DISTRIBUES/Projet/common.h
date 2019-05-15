#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#define L_PSEUDO_MAX    32
#define MULTICAST_ADR   "224.0.0.1"
#define MULTICAST_PORT  10001
#define NB_CLIENT_MAX   4

//CANSI (terminal)
#define BOLD          "\033[1m"
#define UNDERLINE     "\033[4m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"
#define COLOR_BLUE    "\033[34m"
#define RESET         "\033[0m"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct ip_mreq ip_mreq;
typedef struct ifaddrs ifaddrs;
typedef struct in_addr in_addr;

/*static struct option options[] =
{
  {"help",0,NULL,'h'},
  {"version",0,NULL,'v'}
};*/

typedef enum type_message
{
  AUCUN,
  DES,
  ACK,
  CANCEL,
  STARTGAME,
  INFOHOST,
  UNLOG,
  ANSWER,
  YETANSWERED,
  SCORE,
  TOURSUIVANT
} type_message;

typedef struct
{
  sockaddr_in adr;
  char pseudo[L_PSEUDO_MAX];
  int id;
} info_client;

#define T_CONTENU_MESSAGE 2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client)

typedef struct
{
  type_message type;
  char contenu[T_CONTENU_MESSAGE];
} message;

int creerSocketTCP(int port);
int calculerID(char* pseudo, int nbClients);
int indexOf(int id, info_client infoClients[], int nbClients);
void removeClient(info_client infoClients[], int nbClients, int id);
in_addr getIP();

#endif
