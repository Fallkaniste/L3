#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<limits.h>

#define max(x,y) x>y?x:y
#define min(x,y) x<y?x:y

typedef struct
{
  int datePlusTot;
  int datePlusTard;
  int marge;
  int num_tache;
} tache;

tache *projet;
int **matrice;

int main()
{
  int chemin[100];
  int associe [100];
  int sommeChemin = 0;
  int nbTaches = 16 ;
  int i,j,k;
  int somme = 0;
  int *pile;

  projet = (tache *)malloc((nbTaches + 2) * sizeof(tache));
  matrice = (int **)malloc((nbTaches + 2) * sizeof(int *));

  for(i = 0; i < nbTaches + 2; i++)
  {
    matrice[i] = (int *)malloc((nbTaches + 2) * sizeof(int));
  }

  for(i = 0; i < nbTaches + 2; i++)
  {
    for(j = 0; j < nbTaches + 2; j++)
    {
      matrice[i][j] = -1;
    }
  }

  matrice[0][1] = 0;

  pile = (int *)malloc((nbTaches +2) * sizeof(int));

  matrice[0][2]=0;

  matrice[1][3]=3;
  matrice[1][4]=3;
  matrice[1][6]=3;
  matrice[1][7]=3;
  matrice[1][8]=3;
  matrice[1][10]=3;
  matrice[1][11]=3;
  matrice[1][12]=3;
  matrice[1][13]=3;
  matrice[1][14]=3;

  matrice[2][3]=4;
  matrice[2][4]=4;
  matrice[2][5]=4;
  matrice[2][10]=4;
  matrice[2][11]=4;
  matrice[2][12]=4;
  matrice[2][13]=4;
  matrice[2][14]=4;

  matrice[3][14]=7;

  matrice[4][11]=1;
  matrice[4][12]=1;
  matrice[4][13]=1;
  matrice[4][13]=1;

  matrice[5][7]=8;
  matrice[5][10]=8;
  matrice[5][14]=8;

  matrice[6][8]=5;
  matrice[6][9]=5;
  matrice[6][10]=5;
  matrice[6][14]=5;

  matrice[7][10]=8;
  matrice[7][14]=8;

  matrice[8][9]=8;
  matrice[8][10]=8;
  matrice[8][11]=8;
  matrice[8][14]=8;

  matrice[9][10]=5;
  matrice[9][14]=5;

  matrice[10][14]=7;

  matrice[11][12]=6;
  matrice[11][14]=6;

  matrice[12][14]=2;

  matrice[13][14]=7;

  matrice[14][15]=4;

  matrice[nbTaches][nbTaches+1] = 1;

  //Dates au plus tôt
  printf("\n===DATES AU PLUS TOT===\n");
  projet[0].datePlusTot = 0;

  for(j = 1; j < nbTaches + 2; j++)
  {
    projet[j].datePlusTot = 0;
    for(i = 0; i < j; i++)
    {
      if(matrice[i][j] != -1)
      {
        projet[j].datePlusTot = max(projet[j].datePlusTot, projet[i].datePlusTot + matrice[i][j]);
      }
    }
    printf(" \nLa date au plus tôt de %d est %d", j, projet[j].datePlusTot);
  }

  projet[nbTaches+1].datePlusTard = projet[nbTaches+1].datePlusTot;

  //Dates au plus tard
  printf("\n\n===DATES AU PLUS TARD===\n");
  for(i = nbTaches ; i >= 0; i--)
  {
    projet[i].datePlusTard = INT_MAX;
    for(j = i + 1; j <= nbTaches + 1; j++)
    {
      if(matrice[i][j] != -1)
      {
         projet[i].datePlusTard  = min (projet[j].datePlusTard  -  matrice[i][j], projet[i].datePlusTard);
      }
    }
    printf("\nLa date au plus tard de %d est %d", i, projet[i].datePlusTot);
  }

  //Marges
  printf("\n\n===MARGES===\n");
  for(i=0; i < nbTaches +2; i++)
  {
    projet[i].marge = projet[i].datePlusTard - projet[i].datePlusTot;
    printf("\nLa marge de %d est de %d", i, projet[i].marge);
  }

  associe[sommeChemin] = -1;
  pile[somme] = 1;
  somme ++;
  sommeChemin ++;

  while(somme > 0)
  {
  	somme --;
  	i = pile[somme];
  	chemin[sommeChemin] = i;
  	sommeChemin ++;
  	for(j = i+1; j < nbTaches + 2; j++)
    {
      if(matrice[i][j] != -1 && projet[i].marge == 0 && projet[j].marge == 0 )
      {
        if(j != nbTaches + 1)
        {
          pile[somme] = j;
          associe[somme] = i;
          somme++;
        }
        else
        {
          printf("\n ==> ");
          for (k = 1; k < sommeChemin; k++)
          {
            printf(" %d ", chemin[k]);
          }
          for (k = 0; k < sommeChemin; k++)
          {
            if(somme > 0 && chemin[k] ==  associe[somme - 1])
            {
              sommeChemin = k + 1;
              break;
            }
          }
        }
      }
    }
  }

  printf("\n");
  return 0;
}