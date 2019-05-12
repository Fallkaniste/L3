#include "common.h"

//TODO faire une structure id+socket et les fonctions qui vont avec pour savoir quelle socket supprimer.


void helpMessage();
void versionMessage();
void gestionArret();

int socketMulticast;
int socketEcoute;
sockaddr_in adrMulticast;
int id;
int nbClients;
info_client infoClients[NB_CLIENT_MAX];
int socketClients[NB_CLIENT_MAX-1];

int main(int argc, char** argv)
{
  char pseudo[L_PSEUDO_MAX]="Anonyme";
  sockaddr_in adrEcoute;
  info_client infoClient;
  sockaddr_in adrServeur;
  int socketServiceServeur;
  char buffer[2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client)];
  int nbOctets;
  int tmp;

  //Création socket écoute
  socketEcoute = creerSocketTCP(0);
  if(getsockname(socketEcoute,(struct sockaddr*)&adrEcoute,&(socklen_t){sizeof(sockaddr_in)}) == -1)
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
  infoClient.adr=adrEcoute;
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

  socketServiceServeur = accept(socketEcoute,(sockaddr*)&adrServeur,&(socklen_t){sizeof(sockaddr)});
  if(socketServiceServeur==-1);
  {
    perror("accept1");
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
    infoClient.id=id;
    memcpy(&nbClients,buffer+sizeof(int),sizeof(int));
    for(int i=0; i<nbClients; i++)
    {
      memcpy(&infoClients[i], buffer+2*sizeof(int)+i*sizeof(info_client),sizeof(info_client));
    }
    afficherClients(infoClients, nbClients, id);

    int j=0;
    for(int i=0; i<nbClients; i++)
    {
      if(infoClients[i].id!=id)
      {
        socketClients[j] = creerSocketTCP(0);
        if(connect(socketClients[j],(sockaddr*)&infoClients[i].adr,(socklen_t)sizeof(sockaddr))==-1)
        {
          perror("connect");
        }

        if(write(socketClients[j],&infoClient,sizeof(info_client))==-1)
        {
          perror("write");
        }

        j++;
      }
    }

    while(nbClients<4)
    {
      socketClients[nbClients-1] = accept(socketEcoute,(sockaddr*)&infoClients[nbClients-1].adr,&(socklen_t){sizeof(sockaddr)});
      if (socketClients[nbClients-1]==-1)
      {
        perror("accept2");
      }

      bzero(buffer,sizeof(info_client));
      do
      {
        nbOctets=read(socketClients[nbClients-1],buffer,sizeof(info_client));
      } while(nbOctets == -1);
      if(nbOctets==sizeof(info_client))
      {
        memcpy(&infoClients[nbClients], buffer, sizeof(info_client));
        nbClients++;
      }
      afficherClients(infoClients, nbClients, id);

    }

    signal(SIGINT, gestionArret);
    while(1)
    {

      for(int i=0; i<nbClients-1; i++)
      {
        bzero(buffer, sizeof(int));
        nbOctets=read(socketClients[i],buffer,sizeof(int));
        if(nbOctets==sizeof(int))
        {
          memcpy(&tmp, buffer, sizeof(int));

          printf("i:%d\n", i);
          for(int o=0; o<NB_CLIENT_MAX-1; o++)
          {
            printf("socket1:%d\n", socketClients[o]);
          }
          printf("tmp:%d\n", tmp);
          getchar();
          for(int j=0; j<nbClients; j++)
          {
            if(infoClients[j].id==tmp)
            {
              //TODO URGENT!!! DECALER LE TABLEAU SOCKET
              for(int k=i;k<nbClients-2; k++)
              {
                socketClients[k]=socketClients[k+1];
              }

              removeClient(infoClients, nbClients, tmp);
              nbClients--;
            }
          }
          printf("%d removed.",tmp);
          getchar();
          afficherClients(infoClients, nbClients, id);
        }
      }
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

  for(int i=0; i<nbClients-1; i++)
  {
    printf("written");
    if(write(socketClients[i],&id,sizeof(int))==-1)
    {
      perror("write");
    }
    close(socketClients[i]);
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


/*GET OPTfor(int i=0; i<nbClients; i++)
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
