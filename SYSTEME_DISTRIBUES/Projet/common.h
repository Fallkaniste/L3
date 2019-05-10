#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <arpa/inet.h>

#define L_PSEUDO_MAX 32
#define MULTICAST_ADR "224.0.0.1"
#define MULTICAST_PORT 7777
#define NB_CLIENT_MAX 4

typedef struct sockaddr_in sockaddr_in;
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
} info_client;

int creerSocketTCP(int port);

#endif
