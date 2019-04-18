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

static struct option options[] =
{
  {"help",0,NULL,'h'},
  {"version",0,NULL,'v'}
};
int creerSocketTCP(int port);

#endif
