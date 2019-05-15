#ifndef CUL_DE_CHOUETTE
#define CUL_DE_CHOUETTE

#include "common.h"

typedef enum combinaison
{
  VELUTE,
  CHOUETTE,
  CDC,
  SUITE,
  CHOUETTEVELUTE,
  AUCUNE
} combinaison;

void lancerDe(int* ch1, int* ch2, int* cul);
combinaison trouverCombinaison(int ch1, int ch2, int cul);
int verifierSuite(int ch1, int ch2, int cul);
void afficherDes(int ch1, int ch2, int cul, char* pseudo, int idJoueur, int id);
void afficherReponses();
int calculPoints(int ch1, int ch2, int cul, combinaison c);

#endif