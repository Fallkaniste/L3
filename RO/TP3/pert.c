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

int main()
{
  //Déclaration des variables
  int chemin[100];
  int associe [100];
  int sommeChemin = 0;
  int nbTaches = 16 ;
  int i,j,k;
  int somme = 0;
  int *pile;
  tache *projet;
  int **matrice;

  //Instanciation dynamique & initialisation
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
  matrice[0][1]=0;
  pile = (int *)malloc((nbTaches +2) * sizeof(int));

  //Remplissage de la matrice
  matrice[1][2]=0;
  matrice[1][3]=0;

  matrice[2][4]=3;
  matrice[2][5]=3;
  matrice[2][7]=3;
  matrice[2][8]=3;
  matrice[2][9]=3;
  matrice[2][10]=3;
  matrice[2][11]=3;
  matrice[2][12]=3;
  matrice[2][13]=3;
  matrice[2][14]=3;
  matrice[2][15]=3;

  matrice[3][4]=4;
  matrice[3][5]=4;
  matrice[3][6]=4;
  matrice[3][11]=4;
  matrice[3][12]=4;
  matrice[3][13]=4;
  matrice[3][14]=4;
  matrice[3][15]=4;

  matrice[4][15]=7;

  matrice[5][12]=1;
  matrice[5][13]=1;
  matrice[5][14]=1;
  matrice[5][15]=1;

  matrice[6][8]=8;
  matrice[6][11]=8;
  matrice[6][15]=8;

  matrice[7][9]=5;
  matrice[7][10]=5;
  matrice[7][11]=5;
  matrice[7][15]=5;

  matrice[8][11]=8;
  matrice[8][15]=8;

  matrice[9][10]=8;
  matrice[9][11]=8;
  matrice[9][12]=8;
  matrice[9][15]=8;

  matrice[10][11]=5;
  matrice[10][15]=5;

  matrice[11][15]=7;

  matrice[12][13]=6;
  matrice[12][15]=6;

  matrice[13][15]=2;

  matrice[14][15]=7;

  matrice[15][16]=4;

  matrice[nbTaches][nbTaches+1] = 0;

  //Dates au plus tôt
  projet[0].datePlusTot = 0;

  for(j = 0; j < nbTaches+2; j++)
  {
    projet[j].datePlusTot = 0;
    for(i = 0; i < j; i++)
    {
      if(matrice[i][j] != -1)
      {
        projet[j].datePlusTot = max(projet[j].datePlusTot, projet[i].datePlusTot + matrice[i][j]);
      }
    }
  }

  projet[nbTaches+1].datePlusTard = projet[nbTaches+1].datePlusTot;

  //Dates au plus tard
  for(i = nbTaches ; i >= 0; i--)
  {
    projet[i].datePlusTard = INT_MAX;
    for(j = i + 1; j <= nbTaches + 1; j++)
    {
      if(matrice[i][j] != -1)
      {
         projet[i].datePlusTard  = min(projet[j].datePlusTard  -  matrice[i][j], projet[i].datePlusTard);
      }
    }
  }

  //Marges
  for(i=0; i < nbTaches +2; i++)
  {
    projet[i].marge = projet[i].datePlusTard - projet[i].datePlusTot;
  }

  //Chemin critique
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

  //Affichage
  printf("\n===Tableau des marges===\n\n\t+Tot\t+Tard\tMarge\n");
  for(i=1; i<nbTaches+1; i++)
  {
    if(i==1)
    {
      printf("Deb\t%d\t%d\t%d\n",projet[i].datePlusTot,projet[i].datePlusTard,projet[i].marge);
    }
    else if(i<nbTaches)
    {
      printf("%c\t%d\t%d\t%d\n",'A'+i-2,projet[i].datePlusTot,projet[i].datePlusTard,projet[i].marge);
    }
    else
    {
      printf("Fin\t%d\t%d\t%d\n",projet[i].datePlusTot,projet[i].datePlusTard,projet[i].marge);
    }

  }

  for (k = 1; k < sommeChemin; k++)
  {
    if(chemin[k]==1)
    {
      printf("\n\n===Chemin critique===\n\nDeb ");
    }
    else if(chemin[k]<nbTaches)
    {
      printf( " %c ",'A'+chemin[k]-2);
    }
    else
    {
      printf(" Fin\n");
    }
  }

  //Libération de la mémoire
  free(pile);
  free(projet);
  for(i=0; i<nbTaches+2;i++)
  {
    free(matrice[i]);
  }
  free(matrice);

  return 0;
}