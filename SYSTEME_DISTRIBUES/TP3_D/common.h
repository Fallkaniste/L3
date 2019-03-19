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

typedef enum
{
  FIN,
  FACTORIEL,
  PUISSANCE,
  STATS
} requete_t;

typedef struct
{
  requete_t type;
  int taille;
} requete;

typedef struct
{
  float moy;
  int max;
  int min;
} res_analyse_donnees;

int creerSocketTCP(int port);

#endif
