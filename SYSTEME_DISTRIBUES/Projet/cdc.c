#include "cdc.h"

void lancerDe(int* ch1, int* ch2, int* cul)
{
  srand(time(NULL));

  *ch1=rand()%(6)+1;
  *ch2=rand()%(6)+1;
  *cul=rand()%(6)+1;
}

void afficherDes(int ch1, int ch2, int cul, char* pseudo, int idJoueur, int id)
{
  if(idJoueur!=id)
  {
    printf(BOLD "RESULTAT DU LANCÉ DE:" RESET COLOR_GREEN "%s#%d\n" RESET, pseudo, idJoueur);
  }
  else
  {
    printf(BOLD "RESULTAT DE VOTRE LANCÉ:\n" RESET);
  }

  printf(UNDERLINE "Chouette 1:" RESET COLOR_RED "%d\n" RESET, ch1);
  printf(UNDERLINE "Chouette 2:" RESET COLOR_RED "%d\n" RESET , ch2);
  printf(UNDERLINE "Cul de chouette:" RESET COLOR_RED "%d\n\n" RESET, cul);
}

void afficherReponses()
{
  printf(BOLD "CHOISISSEZ VOTRE RÉPONSE:\n" RESET COLOR_RED "[0]" RESET "Grelotte ça picote !\n" COLOR_RED "[1]" RESET "Pas mou le caillou !\n\n");
}

combinaison trouverCombinaison(int ch1, int ch2, int cul)
{
  int score=0;

  printf(BOLD "COMBINAISON: " RESET);
  if (verifierSuite(ch1, ch2, cul))
  {
    printf(COLOR_RED "Suite!\n\n" RESET);
    return SUITE;
  }
  else if((ch1+ch2)==cul)
  {
    printf(COLOR_GREEN "Velute!\n\n" RESET);
    return VELUTE;
  }
  else if(ch1==ch2 && (ch1+ch2)!=cul)
  {
    printf(COLOR_GREEN "Chouette!\n\n" RESET);
    return CHOUETTE;
  }
  else if(ch1==ch2 && ch2==cul)
  {
    printf(COLOR_YELLOW "Cul de chouette!\n\n" RESET);
    return CDC;
  }
  else if(ch1==ch2 && (ch1+ch2)==cul)
  {
    printf(COLOR_GREEN "Chouette velute!\n\n" RESET);
    return CHOUETTEVELUTE;
  }
  else
  {
    printf(COLOR_BLUE "Aucune...\n\n" RESET);
    return AUCUNE;
  }
  //TODO move ci dessous
  printf(BOLD "POINTS MARQUÉS: " RESET COLOR_RED "%d\n\n" RESET, score);
}

int calculPoints(int ch1, int ch2, int cul, combinaison c)
{
  switch(c)
  {
    case VELUTE:
      return pow(cul,2);
      break;
    case CHOUETTE:
      return pow(ch1,2);
      break;
    case CDC:
      return 40+ch1*10;
      break;
    case SUITE:
      return -10;
      break;
    case CHOUETTEVELUTE:
      return pow(ch1,2);
      break;
    default:
      return 0;
      break;
  }
}

int verifierSuite(int ch1, int ch2, int cul)
{
  int tab[3]={ch1, ch2, cul};
  int tmp;

  for (int i=0; i<3; i++)
  {
    for(int j=i; j<3; j++)
    {
      if(tab[j]<tab[i])
      {
        tmp=tab[i];
        tab[i] = tab[j];
        tab[j] = tmp;
      }
    }
  }

  return ((tab[0]+1)==tab[1] && (tab[1]+1)==tab[2]);
}
