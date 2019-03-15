#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

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



#endif
