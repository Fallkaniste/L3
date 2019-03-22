#include "common.h"
#include "client.h"


static int sock;

int main()
{
  sock=ouvrirConnexion("localhost", 7777);

  int* tab;
  int taille;
  int nb;
  int exp;
  res_analyse_donnees res;
  int choix=-1;

  while(choix!=3)
  {
    choix=-1;

    do
    {
      printf("Choisissez votre requête :\n\t0:Factorielle\n\t1:Puissance\n\t2:Statistiques\n\t3:Fin\n");
      scanf("%d", &choix);
    } while(choix<0 || choix>3);

    switch(choix)
    {
      case 0:
        do
        {
          printf("Entrez un nombre (nombre positif):\n");
          scanf("%d", &nb);
        } while(taille<nb);
        printf("Factorielle de %d : %ld\n\n", nb, factoriel(nb));
        break;
      case 1:
        printf("Entrez un nombre:\n");
        scanf("%d", &nb);
        printf("Entrez l'exposant:\n");
        scanf("%d", &exp);
        printf("%d puissance %d: %ld\n\n", nb, exp, puissance(nb, exp));
        break;
      case 2:
        do
        {
          printf("Entrez le nombre de notes à saisir (nombre positif):\n");
          scanf("%d", &taille);
        } while(taille<0);
        tab=malloc(taille*sizeof(int));
        for(int i=0; i<taille; i++)
        {
          do
          {
            printf("Entrez la note n° %d (nombre positif):\n",i);
            scanf("%d", &tab[i]);
          } while(tab[i]<0);
        }
        if(analyserDonnees(tab, taille,&res)==-1)
        {
          perror("erreur FATALE\n");
          exit(1);
        }
        free(tab);
        printf("Moyenne :%f\nMin:%d\nMax:%d\n\n", res.moy, res.min, res.max);
        break;
      case 3:
        fermerConnexion();
        break;
    }
  }

  return 0;
}

int ouvrirConnexion(char* host, int port)
{
  static struct sockaddr_in addr_serveur;
  struct hostent* host_serveur;

  //création de la socket locale
  int sock=creerSocketTCP(0);

  // récupération identifiants de la machine serveur
  host_serveur = gethostbyname(host);
  if (host_serveur==NULL)
  {
    perror("erreur récupération adresse serveur\n");
    exit(1);
  }

  // création de l'identifiant de la socket d'écoute du serveur
  bzero((char *) &addr_serveur, sizeof(addr_serveur));
  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(port);
  memcpy(&addr_serveur.sin_addr.s_addr, host_serveur->h_addr, host_serveur->h_length);

  // connexion de la socket client locale à la socket coté serveur
  if (connect(sock, (struct sockaddr *)&addr_serveur, sizeof(struct sockaddr_in)) == -1)
  {
    perror("erreur connexion serveur");
    exit(1);
  }

  return sock;
}

void fermerConnexion()
{
  char* message;
  int nb_octets=0;
  requete req;
  req.type=FIN;
  req.taille=0;

  message=(char*) malloc(sizeof(requete));
  memcpy(message,&req,sizeof(requete));

  nb_octets=write(sock, message, sizeof(requete));
  free(message);
  if(nb_octets==0 || nb_octets==-1)
  {
    perror("mince alors");
    exit(1);
  }




  close(sock);
}

long factoriel(int nb)
{
  requete req;
  char* message;
  int taille;
  int nb_octets;
  long resultat;

  //Init structure
  req.type=FACTORIEL;
  req.taille=sizeof(int);

  //Init message
  taille=sizeof(requete)+sizeof(int);
  message=(char*) malloc(taille);
  memcpy(message,&req,sizeof(req));
  memcpy(message+sizeof(req), &nb, sizeof(int));

  //Envoi message
  nb_octets=write(sock, message, taille);
  free(message);
  if(nb_octets==0 || nb_octets==-1)
  {
    return -1;
  }

  //Réception résultat
  nb_octets=read(sock,&resultat,sizeof(long));
  if(nb_octets==0 || nb_octets==-1)
  {
    return -1;
  }

  return resultat;
}

long puissance(int nb, int puiss)
{
  requete req;
  char* message;
  int taille;
  int nb_octets;
  long resultat;

  //Init structure
  req.type=PUISSANCE;
  req.taille=2*sizeof(int);

  //Init message
  taille=sizeof(requete)+2* sizeof(int);
  message=(char*) malloc(taille);
  memcpy(message,&req,sizeof(req));
  memcpy(message+sizeof(req), &nb, sizeof(int));
  memcpy(message+sizeof(req)+sizeof(int), &puiss, sizeof(int));

  //Envoi message
  nb_octets=write(sock, message, taille);
  free(message);
  if(nb_octets==0 || nb_octets==-1)
  {
    return -1;
  }

  //Réception résultat
  nb_octets=read(sock,&resultat,sizeof(long));
  if(nb_octets==0 || nb_octets==-1)
  {
    return -1;
  }

  return resultat;
}

int analyserDonnees(int donnees[], int taille,res_analyse_donnees *res)
{
  int taille_data;
  int nb_octets;
  char* message;
  requete req;

  req.type=STATS;
  req.taille=taille*sizeof(int);
  taille_data=sizeof(requete)+req.taille;

  message=malloc(taille_data);
  memcpy(message, &req, sizeof(requete));
  memcpy(message+sizeof(requete), donnees, req.taille);

  nb_octets=write(sock,message,taille_data);
  if(nb_octets==0 || nb_octets==-1)
  {
    return -1;
  }
  free(message);

  nb_octets=read(sock, res, sizeof(res_analyse_donnees));
  if(nb_octets==0 || nb_octets==-1)
  {
    return -1;
  }

  return 0;



}
