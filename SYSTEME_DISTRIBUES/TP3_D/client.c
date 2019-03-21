#include "common.h"
#include "client.h"


static int sock;

int main()
{
  sock=ouvrirConnexion("localhost", 7777);
  printf("%ld\n",puissance(5,3));
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
