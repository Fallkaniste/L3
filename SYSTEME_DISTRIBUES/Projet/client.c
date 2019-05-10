#include "common.h"

#define EOOF 133
#define ENAN 134

void helpMessage();
void versionMessage();
int stringToInt(char* optarg);

int main(int argc, char** argv)
{
  char pseudo[L_PSEUDO_MAX]="Anonyme";
  int socketEcoute;
  sockaddr_in adrEcoute;
  socklen_t tailleAdr;
  int socketMulticast;
  sockaddr_in adrMulticast;
  info_client infoClient;
  int socketServiceServeur;
  sockaddr_in adrServeur;
  int socketsService[NB_CLIENT_MAX];
  char buffer[2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client)];
  int id;
  int nbClients;
  info_client infoClients[NB_CLIENT_MAX];

  //Création socket écoute
  socketEcoute = creerSocketTCP(0);
  if(getsockname(socketEcoute,(struct sockaddr*)&adrEcoute,&tailleAdr) == -1)
  {
    //TODO gestion erreur
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
    //TODO gestion erreur
  }
  close(socketMulticast);

  //Réception de la réponse du serveur
  if(listen(socketEcoute,NB_CLIENT_MAX) == -1)
  {
    //TODO gestion erreur
    printf("erreur listen");
  }

  if((socketServiceServeur = accept(socketEcoute,(struct sockaddr*)&adrServeur, &tailleAdr)) == -1)
  {
    //TODO gestion erreur
    printf("erreur accept");
  }
  bzero(buffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client));

  while(read(socketServiceServeur,buffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client)) != 2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client))
  {
    //TODO gestion erreur
    printf("erreur lecture!");
  }

  memcpy(&id,buffer,sizeof(int));
  memcpy(&nbClients,buffer+sizeof(int),sizeof(int));
  for(int i=0; i<nbClients; i++)
  {
      memcpy(&infoClients+i*sizeof(info_client),buffer+2*sizeof(int)+i*sizeof(info_client),sizeof(info_client));
  }

  printf("buffer:%d\n", sizeof(buffer));
  printf("id:%d,nbClients:%d\n\n==LISTE PSEUDOS==\n", id, nbClients);
  printf("pseudo:%s\n",infoClients[0].pseudo);
  close(socketServiceServeur);

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

int stringToInt(char* optarg)
{
  char* endptr=NULL;
  long value=strtol(optarg, &endptr, 10);

  if (endptr!=optarg+strlen(optarg))
  {
    errno=ENAN;
    fprintf(stderr, "ENAN\n");
  }
  if(value>=INT_MAX||value<=INT_MIN)
  {
    errno=EOOF;
    fprintf(stderr, "EOOF\n");
  }
  return (int)value;
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
