#include "common.h"

//TODO GERER BUG RECONNEXION
//FAIRE LES AFFICHAGES
//CODER LE JEU
//CODER LE JAVA

void helpMessage();
void versionMessage();
void afficherLobby();
void afficherPartie(int scoreMax, int scoreClients[]);
void decoServeur();
void gestionArret();
void* threadLecture(void* arg);
int isID(int id);
int saisieScoreMax();
void determinerOrdre(int ordreSocket[]);

int socketMulticast;
int socketEcoute;
sockaddr_in adrMulticast;
int id;
int nbClients;
info_client infoClients[NB_CLIENT_MAX];
int socketClients[NB_CLIENT_MAX-1];
int hostID;

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
  pthread_t threadLectureAdr;
  int threadCree=-1;
  char saisieUsr=0;
  int partieLancee=0;
  int scoreMax=0;
  int ordreSocket[NB_CLIENT_MAX-1];
  int scoreClients[NB_CLIENT_MAX]={0};

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

  bzero(buffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client));
  nbOctets=read(socketServiceServeur,buffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client));
  if(nbOctets==-1)
  {
    perror("readServer");
  }
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
    afficherLobby();
    signal(SIGINT, gestionArret);
    int j=0;
    for(int i=0; i<nbClients; i++)
    {
      if(infoClients[i].id!=id)
      {
        socketClients[j] = creerSocketTCP(0);
        if(connect(socketClients[j],(sockaddr*)&infoClients[i].adr,(socklen_t)sizeof(sockaddr))==-1)
        {
          perror("connectClient");
        }
        nbOctets=read(socketClients[j],&tmp,sizeof(int));
        if(nbOctets==sizeof(int))
        {
          switch(tmp)
          {
            case 0:
              break;
            case -1:
              system("clear");
              printf(COLOR_RED "Connection refusée :" RESET " Une partie est déjà en cours, vous ne pouvez rejoindre pour le moment.\n");
              decoServeur();
              exit(EXIT_SUCCESS);
            default:
              printf("ERREUR REPONSE INATTENDUE\n");
              exit(EXIT_FAILURE);

          }
        }

        afficherLobby();
        fcntl(socketClients[j], F_SETFL, O_NONBLOCK);

        if(write(socketClients[j],&infoClient,sizeof(info_client))==-1)
        {
          perror("writeClient");
        }

        j++;
      }
    }

    fcntl(socketEcoute, F_SETFL, O_NONBLOCK);
    while(1)
    {
      tmp=-1;
      if(nbClients<4)
      {
        tmp=accept(socketEcoute,(sockaddr*)&infoClients[nbClients-1].adr,&(socklen_t){sizeof(sockaddr)});
      }

      if(tmp!=-1 && !partieLancee)
      {
        socketClients[nbClients-1]=tmp;
        fcntl(socketClients[nbClients-1], F_SETFL, O_NONBLOCK);
        if(write(tmp,&(int){0},sizeof(int))==-1)
        {
          perror("writeOKClient");
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
        afficherLobby();
      }
      else if(tmp!=-1 && partieLancee)
      {
        if(write(tmp,&(int){-1},sizeof(int))==-1)
        {
          perror("writeCancelClient");
        }
      }
      else
      {
        if(nbClients>1)
        {
          if(threadCree==-1)
          {
            afficherLobby();
            threadCree=pthread_create(&threadLectureAdr, NULL, (void*)threadLecture, (void*)&saisieUsr);
          }
          if(saisieUsr=='y')
          {
            saisieUsr=0;
            partieLancee=1;
            bzero(buffer, sizeof(int));
            for(int i=0; i<nbClients-1; i++)
            {
              if(write(socketClients[i],&(int){1},sizeof(int))==-1)
              {
                perror("writeClientStartGame");
              }
            }
            scoreMax=saisieScoreMax();

            bzero(buffer, 2*sizeof(int));
            memcpy(buffer, &id, sizeof(int));
            memcpy(buffer+sizeof(int), &scoreMax, sizeof(int));
            for(int i=0; i<nbClients-1; i++)
            {
              if(write(socketClients[i],buffer,2*sizeof(int))==-1)
              {
                perror("writeGameInfos");
              }
            }

            determinerOrdre(ordreSocket);
            afficherPartie(scoreMax, scoreClients);
          }
        }

        for(int i=0; i<nbClients-1; i++)
        {
          bzero(buffer, 2*sizeof(int));
          tmp=-1;
          nbOctets=read(socketClients[i],buffer,2*sizeof(int));
          if(nbOctets==sizeof(int))
          {
            memcpy(&tmp, buffer, sizeof(int));
            if(isID(tmp))
            {
              close(socketClients[i]);
              for(int j=i;j<nbClients-2; j++)
              {
                socketClients[j]=socketClients[j+1];
              }
              removeClient(infoClients, nbClients, tmp);
              nbClients--;
              if(partieLancee)
              {
                partieLancee=0;
                threadCree=-1;
                system("clear");
                printf(COLOR_RED "Partie annulée :" RESET " Joueur(s) déconnecté.\n");
                sleep(2);
              }
              afficherLobby();
            }
            else if(tmp==1)
            {
              partieLancee=1;
              pthread_cancel(threadLectureAdr);
              printf("La partie a été lancé par un autre utilisateur." COLOR_YELLOW " Veuillez patienter...\n" RESET);
              sleep(2);
            }
          }
          else if(nbOctets==2*sizeof(int))
          {
            if(partieLancee)
            {
              memcpy(&hostID, buffer, sizeof(int));
              memcpy(&scoreMax, buffer+sizeof(int), sizeof(int));
              afficherPartie(scoreMax, scoreClients);
            }

          }
          else if(nbOctets!=-1)
          {
            printf("ERREUR REPONSE INATTENDUEDANSFOR\n");
            exit(EXIT_FAILURE);
          }
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
        printf(COLOR_RED "Connection refusée :" RESET " Aucune place disponible.\n");
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

void afficherPartie(int scoreMax, int scoreClients[])
{
  system("clear");
  printf(BOLD UNDERLINE "PARTIE EN COURS\n\n" RESET BOLD "SCORE À ATTEINDRE:" RESET COLOR_RED " %d\n\n" RESET BOLD "SCORE JOUEURS\n" RESET, scoreMax);
  for(int i=0; i<nbClients; i++)
  {
    printf(UNDERLINE "%s#%04d" RESET,infoClients[i].pseudo,infoClients[i].id);
    if(id==infoClients[i].id)
    {
      printf(COLOR_GREEN " (Vous)" RESET);
    }
    printf(" : " COLOR_RED "%d\n" RESET, scoreClients[i]);
  }
  printf("\n");
}

void afficherResultats()
{
  //TODO
}

void afficherLobby()
{
  system("clear");
  printf(BOLD UNDERLINE "EN ATTENTE DE PARTIE\n\n" RESET BOLD "NOMBRE DE JOUEURS:" RESET COLOR_RED " %d\n\n" RESET BOLD "LISTE JOUEURS\n" RESET, nbClients);
  for(int i=0; i<nbClients; i++)
  {
    printf(UNDERLINE "%d:" RESET "%s#%04d",i+1,infoClients[i].pseudo,infoClients[i].id);
    if(id!=infoClients[i].id)
    {
      printf("\n");
    }
    else
    {
      printf(COLOR_GREEN " (Vous)\n" RESET);
    }
  }
  if(nbClients>1)
  {
    printf("\nAppuyez sur entrée pour commencer la partie.\n");
  }
  printf("\n");
}

void determinerOrdre(int ordreSocket[])
{
  int idRandomized[NB_CLIENT_MAX-1];
  int tmp;

  srand(time(NULL));

  //Pour chaque id on lui soustrait un nombre aléatoire entre 0 et lui même inclus
  //On remplit également le tableau de sockets
  for(int i=1; i<nbClients; i++)
  {
    idRandomized[i-1] = infoClients[i].id - rand()%(infoClients[i].id+1);
    ordreSocket[i-1] = socketClients[i-1];
  }

  //Tri bulle croissant du premier tableau et rangement du second dans le même ordre
  for (int i=0; i<nbClients-1; i++)
  {
     for(int j=i; j<nbClients-1; j++)
     {
       if(idRandomized[j]>idRandomized[i])
       {
         tmp=idRandomized[i];
         idRandomized[i] = idRandomized[j];
         idRandomized[j] = tmp;

         tmp=ordreSocket[i];
         ordreSocket[i] = ordreSocket[j];
         ordreSocket[j] = tmp;
       }
     }
   }

   for (int i=0; i<nbClients-1; i++)
   {
     printf("%d ", ordreSocket[i]);
   }
}

int isID(int id)
{
  for(int i=0; i<nbClients; i++)
  {
    if(infoClients[i].id==id)
    {
      return 1;
    }
  }
  return 0;
}

void* threadLecture(void* arg)
{
  while(getchar()!=0x0A);

  memcpy(arg, &(char){'y'}, sizeof(char));

  pthread_exit(NULL);
}

void decoServeur()
{
  close(socketEcoute);
  if(sendto(socketMulticast,&id,sizeof(int),0,(struct sockaddr*) &adrMulticast,sizeof(sockaddr_in))!=sizeof(int))
  {
    perror("writeServerQuitNotif");
  }
}

void gestionArret()
{
  printf(COLOR_RED "Arrêt du jeu...\n" RESET);
  decoServeur();

  for(int i=0; i<nbClients-1; i++)
  {
    if(write(socketClients[i],&id,sizeof(int))==-1)
    {
      perror("writeClientQuitNotif");
    }
    close(socketClients[i]);
  }
  printf(BOLD "Terminé.\n" RESET);
  exit(EXIT_SUCCESS);
}

int saisieScoreMax()
{
  char buffer;
  char saisie[3];
  char* endptr=NULL;
  long value;
  int saisieCorrecte=0;

  printf("Entrez le nombre de points à atteindre (min: 1; max: 343).\n");

  do
  {
    do
    {
      saisieCorrecte=0;
      bzero(saisie, 4*sizeof(char));
      for(int i=0; i<4; i++)
      {
        buffer = fgetc(stdin);
        if(buffer=='\n')
        {
          break;
        }
        saisie[i]=buffer;
      }
      if(buffer!='\n')
      {
        do
        {
          buffer=fgetc(stdin);
        } while(buffer!='\n');
        printf("Vous avez saisie un nombre trop important de caractères.\n");
      }
      else
      {
        saisieCorrecte=1;
      }
    } while(!saisieCorrecte);


    value=strtol(saisie, &endptr, 10);
    if(endptr!=saisie+strlen(saisie))
    {
      printf("%s n'est pas un nombre.\n",saisie);
    }
    if(!(endptr!=saisie+strlen(saisie)) && (value<=0||value>343))
    {
      printf("%ld n'est pas dans l'intervalle indiqué.\n",value);
    }
  } while(endptr!=saisie+strlen(saisie)||value<=0||value>343);

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
