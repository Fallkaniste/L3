#include "common.h"

#define ENAN 133
#define EOOF 134

void helpMessage();
void versionMessage();
void gestionArret();

int socketMulticast;
int socketEcoute;
static sockaddr_in adrMulticast;
int id;

int main(int argc, char** argv)
{
  char pseudo[L_PSEUDO_MAX]="Anonyme";
  sockaddr_in adrEcoute;
  socklen_t tailleAdr;

  info_client infoClient;
  int socketServiceServeur;
  sockaddr_in adrServeur;
  char buffer[2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client)];
  int nbClients;
  info_client infoClients[NB_CLIENT_MAX];
  int nbOctets;

  //Création socket écoute
  socketEcoute = creerSocketTCP(0);
  tailleAdr=sizeof(sockaddr_in);
  if(getsockname(socketEcoute,(struct sockaddr*)&adrEcoute,&tailleAdr) == -1)
  {
    perror("getsockname");
  }

  //Création socket service multicast
  socketMulticast = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketMulticast == -1)
  {
    perror("socket");
    return -1;
  }

  bzero((char *)&adrMulticast, sizeof(sockaddr_in));
  adrMulticast.sin_family = AF_INET;
  adrMulticast.sin_addr.s_addr = inet_addr(MULTICAST_ADR);
  adrMulticast.sin_port = htons(MULTICAST_PORT);

  //Envoi des infos clients
  memcpy(&infoClient.adr, &adrEcoute, sizeof(sockaddr_in));
  strcpy(infoClient.pseudo, pseudo);

  if(sendto(socketMulticast,&infoClient,sizeof(info_client),0,(struct sockaddr*) &adrMulticast,sizeof(sockaddr_in))!=sizeof(infoClient))
  {
    perror("send");
  }

  //Réception de la réponse du serveur
  if(listen(socketEcoute,NB_CLIENT_MAX) == -1)
  {
    perror("listen");
  }

  if((socketServiceServeur = accept(socketEcoute,(struct sockaddr*)&adrServeur, &tailleAdr)) == -1)
  {
    perror("accept");
  }

  bzero(buffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client));
  do
  {
    nbOctets=read(socketServiceServeur,buffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client));
  } while(nbOctets == -1);

  close(socketServiceServeur);

  if(nbOctets==2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client))
  {
    memcpy(&id,buffer,sizeof(int));
    memcpy(&nbClients,buffer+sizeof(int),sizeof(int));
    printf(BOLD "NOMBRE DE JOUEURS:" FONT_RESET COLOR_RED " %d\n\n" COLOR_RESET BOLD "LISTE JOUEURS\n" FONT_RESET, nbClients);
    for(int i=0; i<nbClients; i++)
    {
      memcpy(&infoClients[i], buffer+2*sizeof(int)+i*sizeof(info_client),sizeof(info_client));

      printf(UNDERLINE "%d:" FONT_RESET "%s#%d",i+1,infoClients[i].pseudo,infoClients[i].id);
      if(id!=infoClients[i].id)
      {
        printf("\n");
      }
      else
      {
        printf(COLOR_GREEN " (Vous)\n" COLOR_RESET);
      }
    }
    printf("\n");

    signal(SIGINT, gestionArret);
    while(1)
    {
      //TODO JEU
    }
  }
  else if(nbOctets==sizeof(int))
  {
    memcpy(&id,buffer,sizeof(int));
    switch(id)
    {
      case -1:
        printf(COLOR_RED "Connection refusée :" COLOR_RESET " Aucune place disponible.\n");
        break;
      default:
        printf("ERREUR REPONSE INATTENDUE\n");
        exit(EXIT_FAILURE);
    }
  }
  else
  {
    printf("ERREUR REPONSE INATTENDUE\n");
    exit(EXIT_FAILURE);
  }



  /*for(int i=0; i<nbClients; i++)
  {
    printf("pseudo:%s\n",infoClients[i].pseudo);
  }*/
  /*int opt=0;
  int port;
  char* hostname;

  do
  {
    opt=getopt_long(argc,argv,"hv", options,NULL);
    switch(opt)
    {
      case 'h':
        helpMessage();
        exit(EXIT_SUCCESS);
      case 'v':
        versionMessage();
        exit(EXIT_SUCCESS);
      default:
        if(argc==3)
        {
          hostname=argv[1];
          port=stringToInt(argv[2]);
          break;
        }
        break;
    }
  } while(opt!=-1);


  int choix=-1;

  while(choix!=3)
  {
    choix=-1;

    do
    {
      printf("TODO"); //TODO
      scanf("%d", &choix);
    } while(choix<0 || choix>3);

    switch(choix)
    {
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
    }
  }*/

  return 0;
}

void gestionArret()
{
  printf(COLOR_RED "Arrêt du jeu...\n" COLOR_RESET);
  close(socketEcoute);
  if(sendto(socketMulticast,&id,sizeof(int),0,(struct sockaddr*) &adrMulticast,sizeof(sockaddr_in))==sizeof(int))
  {
    printf(BOLD "Terminé.\n" FONT_RESET);
  }
  exit(EXIT_SUCCESS);
}

void helpMessage()
{
  printf("Options:\n \
  \t-h, --help display this help and exit\n \
  \t-v, --version output version information and exit\n");
  printf("Report bugs to Croquefer Gaëtan <gaetan.croquefer@etud.univ-pau.fr> and Amiard Landry <landry.amiard@etud.univ-pau.fr>.\n");
}

void versionMessage()
{
  printf("cdc-client 1.0\nCopyright (C) 2019 Croquefer Gaëtan and Amiard Landry.\nWritten by Croquefer Gaëtan <gaetan.croquefer@etud.univ-pau.fr> and Amiard Landry <landry.amiard@etud.univ-pau.fr>.\n");
}
