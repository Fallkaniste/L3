#include "common.h"
#include "calculs.h"
#include "serveur.h"

int main()
{
  int socket_ecoute;
  int socket_service;

  //création de la socket d'écoute
  socket_ecoute=creerSocketTCP(7777);
  socket_service=ouvrirConnexion(socket_ecoute);
  traiter_communication(socket_service);
  close(socket_service);

  return 0;
}

int ouvrirConnexion(int socket_ecoute)
{
  // adresse socket coté client
  static struct sockaddr_in addr_client;
  // longueur adresse
  int lg_addr;
  //socket_service
  int sock;

  // configuration socket écoute : 5 connexions max en attente
  if (listen(socket_ecoute, 5) == -1)
  {
    perror("erreur listen");
    exit(1);
  }

  // on attend la connexion du client
  lg_addr = sizeof(struct sockaddr_in);
  sock = accept(socket_ecoute, (struct sockaddr *)&addr_client, (socklen_t *)&lg_addr);
  if (sock == -1)
  {
    perror("erreur accept");
    exit(1);
  }
  return sock;
}

void traiter_communication(int socket_service)
{
  requete req;
  int nb_octets;
  int fini=0;
  int nb;
  int puiss;
  long res;
  int attendus, offset;
  int* tab;
  res_analyse_donnees resu;

  while(!fini)
  {
    //Réception type de requête
    nb_octets=read(socket_service, &req, sizeof(requete));
    if(nb_octets==0 || nb_octets==-1)
    {
      printf("%d", nb_octets);
      perror("landry genius");
      fini=1;
      break;
    }

    //Traitement requête
    switch(req.type)
    {
      case FIN :
        printf("J'arrête la connexion\n");
        fini=1;
        break;
      case FACTORIEL :
        printf("On m'a demandé une factorielle ");
        //Réception valeur
        nb_octets=read(socket_service, &nb, sizeof(int));
        if(nb_octets==0 || nb_octets==-1)
        {
          fini=1;
          break;
        }
        printf("de %d\n", nb);
        //Calcul
        res=factoriel(nb);

        //Réponse
        nb_octets=write(socket_service, &res, sizeof(long));
        if(nb_octets==0 || nb_octets==-1)
        {
          fini=1;
          break;
        }
        break;
      case PUISSANCE :
        printf("On m'a demandé une puissance :\n");
        //Réception valeur
        nb_octets=read(socket_service, &nb, sizeof(int));
        if(nb_octets==0 || nb_octets==-1)
        {
          fini=1;
          break;
        }

        nb_octets=read(socket_service, &puiss, sizeof(int));
        if(nb_octets==0 || nb_octets==-1)
        {
          fini=1;
          break;
        }
        printf("%d à la puissance %d\n", nb, puiss);
        //Calcul
        res=puissance(nb,puiss);

        //Réponse
        nb_octets=write(socket_service, &res, sizeof(long));
        if(nb_octets==0 || nb_octets==-1)
        {
          fini=1;
          break;
        }
        break;
      case STATS :
        printf("On m'a demandé des statistiques\n");
        tab=malloc(req.taille);
        attendus=req.taille;
        offset=0;
        printf("Avec ces donnees :\n");
        while(attendus>0)
        {
          nb_octets=read(socket_service, tab+offset, sizeof(int));
          printf("%d ", *(tab+offset));
          offset++;
          attendus-=nb_octets;
        }
        printf("\n");

        analyserDonnees(tab,offset,&resu);
        nb_octets=write(socket_service, &resu, sizeof(res_analyse_donnees));
        if(nb_octets==0 || nb_octets==-1)
        {
          fini=1;
          break;
        }
        free(tab);
        break;
      default :
        printf("Vous ne devriez pas être ici.\n");
        break;
    }
  }
}
