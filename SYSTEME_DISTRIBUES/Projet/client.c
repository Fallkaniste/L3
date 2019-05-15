#include "common.h"
#include "cdc.h"

//TODO
//FAIRE LES AFFICHAGES
//CODER LE JEU
//CODER LE JAVA

void helpMessage();
void versionMessage();
void afficherLobby();
void afficherPartie(int scoreMax, int scoreClients[]);
void decoServeur();
void gestionArret();
void* threadLectureHost(void* arg);
int saisieScoreMax();
void determinerOrdre(int ordreSocket[]);
void* threadLectureJeu(void* arg);
int traiterCombinaison(int ch1, int ch2, int cul, combinaison c, int scoreClients [], int scoreMax, int idJoueur, int* partieLancee);

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
  char buffer[T_CONTENU_MESSAGE];
  message msg;
  int nbOctets;
  int tmp;
  pthread_t threadLectureHostAdr;
  int threadLectureHostCree=-1;
  char saisieUsr=0;
  int partieLancee=0;
  int ordreSocket[NB_CLIENT_MAX-1];
  int scoreClients[NB_CLIENT_MAX]={0};
  int hostID;
  int hostSocket;
  int tourDeJouer=0;
  int ch1=0;
  int ch2=0;
  int cul=0;
  int idJoueur=0;
  combinaison c=AUCUNE;
  pthread_t threadLectureJeuAdr;
  int compteurTour=0;
  int scoreMax=0;
  int score=0;


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

  bzero(buffer,T_CONTENU_MESSAGE);
  nbOctets=read(socketServiceServeur,buffer,T_CONTENU_MESSAGE);
  if(nbOctets==-1)
  {
    perror("readServer");
  }
  close(socketServiceServeur);

  if(nbOctets==T_CONTENU_MESSAGE)
  {
    memcpy(&id,buffer,sizeof(int));
    infoClient.id=id;
    memcpy(&nbClients,buffer+sizeof(int),sizeof(int));
    for(int i=0; i<nbClients; i++)
    {
      memcpy(&infoClients[i], buffer+2*sizeof(int)+i*sizeof(info_client),sizeof(info_client));
    }

    signal(SIGINT, gestionArret);
    signal(SIGQUIT, gestionArret);
    signal(SIGTERM, gestionArret);
    signal(SIGHUP, gestionArret);
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
        if(read(socketClients[j],&msg,sizeof(message))==-1)
        {
          perror("readClient");
        }

        switch(msg.type)
        {
          case ACK:
            afficherLobby();
            break;
          case CANCEL:
            printf(COLOR_RED "Connection refusée :" RESET " Une partie est déjà en cours, vous ne pouvez rejoindre pour le moment.\n");
            decoServeur();
            exit(EXIT_SUCCESS);
          default:
            printf("ERREUR REPONSE INATTENDUE BOUCLE CONNECT\n");
            exit(EXIT_FAILURE);
        }

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
      //Acceptation nouvelles connexions
      tmp=-1;
      if(nbClients<4)
      {
        tmp=accept(socketEcoute,(sockaddr*)&infoClients[nbClients-1].adr,&(socklen_t){sizeof(sockaddr)});
      }

      //Si on a une connexion et que l'on n'est pas en partie
      if(tmp!=-1 && !partieLancee)
      {
        socketClients[nbClients-1]=tmp;
        fcntl(socketClients[nbClients-1], F_SETFL, O_NONBLOCK);
        msg.type=ACK;
        if(write(tmp,&msg,sizeof(message))==-1)
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
      //Si on a une connexion et que l'on est déjà en partie
      else if(tmp!=-1 && partieLancee)
      {
        msg.type=CANCEL;
        if(write(tmp,&msg,sizeof(message))==-1)
        {
          perror("writeCancelClient");
        }
      }

      //Si on a pas reçu de demande de connexion
      else
      {
        //Demande de début de partie
        if(nbClients>1)
        {
          if(threadLectureHostCree==-1)
          {
            afficherLobby();
            threadLectureHostCree=pthread_create(&threadLectureHostAdr, NULL, (void*)threadLectureHost, (void*)&saisieUsr);
            if(threadLectureHostCree==-1)
            {
              perror("errorHostThread");
            }
          }
        }

        //Gestion tour de jeu
        if(tourDeJouer==1)
        {
          tourDeJouer=0;

          printf("Appuyez sur entrée pour jouer.\n\n");
          threadLectureHostCree=pthread_create(&threadLectureHostAdr, NULL, (void*)threadLectureHost, (void*)&saisieUsr);
          if(threadLectureHostCree==-1)
          {
            perror("errorHostThread");
          }
        }

        if(!partieLancee && idJoueur!=0)
        {
          for(int i=0; i<nbClients; i++)
          {
            scoreClients[i]=0;
          }

          if(idJoueur!=id)
          {
            tmp=indexOf(id, infoClients, nbClients);
            printf(COLOR_RED BOLD "PERDU!\n" RESET);
            printf(UNDERLINE "Victoire de :" RESET COLOR_YELLOW " %s#%d" RESET, infoClients[tmp].pseudo, infoClients[tmp].id);
          }
          else
          {
            printf(COLOR_GREEN BOLD "VICTOIRE!\n" RESET);
          }
          printf("\nAppuyez sur entrée pour continuer.");
          getchar();
          compteurTour=0;
          idJoueur=0;
          tourDeJouer=0;
          threadLectureHostCree=-1;
        }


        //Gestion retour thread
        if(saisieUsr=='y')
        {
          saisieUsr=0;
          //Pour initialiser la partie
          if(!partieLancee)
          {
            partieLancee=1;
            msg.type=STARTGAME;
            for(int i=0; i<nbClients-1; i++)
            {
              if(write(socketClients[i],&msg,sizeof(message))==-1)
              {
                perror("writeClientStartGame");
              }
            }

            scoreMax=saisieScoreMax();

            hostID=id;
            bzero(msg.contenu, 2*sizeof(int));
            memcpy(msg.contenu, &hostID, sizeof(int));
            memcpy(msg.contenu+sizeof(int), &scoreMax, sizeof(int));
            msg.type=INFOHOST;

            for(int i=0; i<nbClients-1; i++)
            {
              if(write(socketClients[i],&msg,sizeof(msg))==-1)
              {
                perror("writeGameInfos");
              }
            }

            determinerOrdre(ordreSocket);
            afficherPartie(scoreMax, scoreClients);
            tourDeJouer=1;
            idJoueur=id;
          }
          //Pour lancer un tour de jeu
          else
          {
            idJoueur=id;

            lancerDe(&ch1, &ch2, &cul);
            bzero(msg.contenu, 4*sizeof(int));
            memcpy(msg.contenu, &id, sizeof(int));
            memcpy(msg.contenu+sizeof(int), &ch1, sizeof(int));
            memcpy(msg.contenu+2*sizeof(int), &ch2, sizeof(int));
            memcpy(msg.contenu+3*sizeof(int), &cul, sizeof(int));
            msg.type=DES;

            for(int i=0; i<nbClients-1; i++)
            {
              if(write(socketClients[i], &msg, sizeof(message))!=sizeof(message))
              {
                perror("writeDe");
              }
            }

            afficherDes(ch1, ch2, cul, infoClient.pseudo, idJoueur, id);
            c=trouverCombinaison(ch1,ch2,cul);


            if(idJoueur==hostID)
            {
              score=traiterCombinaison(ch1, ch2, cul, c, scoreClients, scoreMax, idJoueur, &partieLancee);
              printf("Vous avez marqué " COLOR_RED "%d" RESET " points.\n\n", score);

              if(partieLancee)
              {
                msg.type=TOURSUIVANT;
                if(write(ordreSocket[compteurTour],&msg,sizeof(message))==-1)
                {
                  perror("erreurtoursuivant");
                }
              }
              compteurTour++;
              printf("Attente du joueur suivant...\n\n");
            }
          }
        }



        //Gestion interactions
        /*if(hostID!=id && (saisieUsr=='0' || saisieUsr=='1'))
        {
          bzero(msg.contenu, sizeof(int)+sizeof(char));
          memcpy(msg.contenu, &id, sizeof(int));
          memcpy(msg.contenu+sizeof(int), &saisieUsr, sizeof(char));
          msg.type=ANSWER;

          if(write(hostSocket, &msg, sizeof(message))==-1)
          {
            perror("errorAnswerWrite");
          }

          saisieUsr=0;
        }*/

        //Lecture et traitement sur toute les sockets
        for(int i=0; i<nbClients-1; i++)
        {

          bzero(&msg, sizeof(message));
          nbOctets=read(socketClients[i],&msg,sizeof(message));

          if(msg.type==UNLOG)
          {
            memcpy(&tmp, msg.contenu, sizeof(int));
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
              system("clear");
              printf(COLOR_RED "Partie annulée :" RESET " Joueur(s) déconnecté.\n");
              tourDeJouer=0;
              idJoueur=0;
              sleep(1);
            }
            afficherLobby();
          }
          else if(msg.type==STARTGAME)
          {
            partieLancee=1;
            pthread_cancel(threadLectureHostAdr);
            printf("\nLa partie a été lancé par un autre utilisateur." COLOR_YELLOW " Veuillez patienter...\n" RESET);
            sleep(2);
          }
          else if(msg.type==CANCEL)
          {
            //pthread_cancel(threadLectureJeuAdr);
            printf("Un autre utilisateur a répondu avant vous." COLOR_YELLOW " Veuillez patienter...\n" RESET);
            sleep(2);
          }
          else if(msg.type==INFOHOST)
          {
            memcpy(&hostID, msg.contenu, sizeof(int));
            memcpy(&scoreMax, msg.contenu+sizeof(int), sizeof(int));
            hostSocket=socketClients[i];
            afficherPartie(scoreMax, scoreClients);
            printf("Attente du joueur suivant...\n\n");
          }
          else if(msg.type==DES)
          {
            afficherPartie(scoreMax, scoreClients);
            memcpy(&idJoueur, msg.contenu, sizeof(int));
            memcpy(&ch1, msg.contenu+sizeof(int), sizeof(int));
            memcpy(&ch2, msg.contenu+2*sizeof(int), sizeof(int));
            memcpy(&cul, msg.contenu+3*sizeof(int), sizeof(int));
            afficherDes(ch1, ch2, cul, infoClient.pseudo, idJoueur, id);
            c=trouverCombinaison(ch1, ch2, cul);

            if(id==hostID)
            {
              score=traiterCombinaison(ch1, ch2, cul, c, scoreClients, scoreMax, idJoueur, &partieLancee);

              compteurTour++;
              if(compteurTour==nbClients)
              {
                compteurTour=0;
                tourDeJouer=1;
              }
              else
              {
                msg.type=TOURSUIVANT;
                if(write(ordreSocket[compteurTour-1],&msg,sizeof(message))==-1)
                {
                  perror("erreurtoursuivant");
                }
              }

              idJoueur=0;
            }



          }
          else if(msg.type==SCORE)
          {
            for(int i=0; i<nbClients; i++)
            {
              memcpy(&scoreClients[i], msg.contenu+i*sizeof(int), sizeof(int));
              if(scoreClients[i]>scoreMax)
              {
                partieLancee=0;
                break;
              }
            }

            if(idJoueur==id)
            {
                printf("Vous avez marqué " COLOR_RED "%d" RESET " points.\n\n", calculPoints(ch1, ch2, cul, c));
            }
            else
            {
              tmp=indexOf(idJoueur, infoClients, nbClients);
              printf("Le joueur " COLOR_YELLOW "%s#%d" RESET " a marqué " COLOR_RED "%d" RESET " points.\n\n", infoClients[tmp].pseudo, infoClients[tmp].id, calculPoints(ch1, ch2, cul, c));
            }
            printf("Attente du joueur suivant...\n\n");
          }

          /*else if (hostID==id && (saisieUsr=='0' || saisieUsr=='1' || msg.type==ANSWER))
          {
            if(nbOctets!=-1)
            {
              pthread_cancel(threadLectureJeuAdr);
              memcpy(&tmp, msg.contenu, sizeof(int));
              memcpy(&saisieUsr, msg.contenu+sizeof(int), sizeof(char));
            }
            else
            {
              tmp=id;
            }

            if(saisieUsr=='0' && c==SUITE)
            {
              bzero(&msg.contenu, sizeof(int));
              memcpy(&msg.contenu, &tmp, sizeof(int));
              msg.type=YETANSWERED;
              for(int j=0; j<nbClients-1; j++)
              {
                if(write(socketClients[j],&msg,sizeof(message))==-1)
                {
                  perror("writeYetAnswered");
                }
              }
              if(tmp==id)
              {
                printf("Vous avez été le plus rapide et remportez %d points.\n\n", scoreDes);
                for(int i=0; i<nbClients; i++)
                {
                  if(infoClients[i].id==id)
                  {
                    scoreClients[i]+=scoreDes;
                  }
                  memcpy(msg.contenu+i*sizeof(int), &scoreClients[i], sizeof(int));
                }
              }
              else
              {
                printf("Le joueur %s#%d remporte %d points.\n\n", infoClients[i].pseudo, infoClients[i].id, scoreDes);
                bzero(msg.contenu, sizeof(T_CONTENU_MESSAGE));
                for(int i=0; i<nbClients; i++)
                {
                  if(infoClients[i].id==idJoueur)
                  {
                    scoreClients[i]+=scoreDes;
                  }
                  memcpy(msg.contenu+i*sizeof(int), &scoreClients[i], sizeof(int));
                }
              }

              msg.type=SCORE;
              for(int i=1; i<nbClients; i++)
              {
                if(idJoueur==infoClients[i].id)
                {
                  if(write(socketClients[i-1],&msg,sizeof(message) )==-1)
                  {
                    perror("writeScore");
                  }
                }
              }

            }
            else if(saisieUsr=='1' && c==CHOUETTEVELUTE)
            {
              reponseChouetteVelute[nbRepondus-1]=id;
              nbRepondus++;
            }
            else
            {
              printf(COLOR_RED "Ce n'était pas la bonne réponse...\n\n" RESET);
            }
            saisieUsr=0;
          }
          else if(msg.type==YETANSWERED)
          {
            pthread_cancel(threadLectureJeuAdr);
            memcpy(&tmp, msg.contenu, sizeof(int));
            for(int j=1; j<nbClients; j++)
            {
              if(tmp==infoClients[j].id)
              {
                printf("%s#%d a répondu avant vous.", infoClients[j].pseudo, tmp);
                break;
              }
            }
          }*/
          else if(msg.type==TOURSUIVANT)
          {
            afficherPartie(scoreMax, scoreClients);
            tourDeJouer=1;
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

void gererInteractions(combinaison c, pthread_t* threadAdr, char* saisieUsr)
{
  /*if(c==SUITE || c==CHOUETTEVELUTE)
  {
    afficherReponses();
    if(pthread_create(threadAdr, NULL, (void*)threadLectureJeu, (void*)saisieUsr)==-1)
    {
      perror("errorHostJeu");
    }
  }
  else
  {*/

  //}

}

int traiterCombinaison(int ch1, int ch2, int cul, combinaison c, int scoreClients[], int scoreMax,  int idJoueur, int* partieLancee)
{
  message msg;
  int index;
  int scoreDes=calculPoints(ch1, ch2, cul, c);

  bzero(msg.contenu, sizeof(T_CONTENU_MESSAGE));
  for(int i=0; i<nbClients; i++)
  {
    if(infoClients[i].id==idJoueur)
    {
      scoreClients[i]+=scoreDes;
    }
    if(scoreClients[i]>scoreMax)
    {
      *partieLancee=0;
    }
    memcpy(msg.contenu+i*sizeof(int), &scoreClients[i], sizeof(int));
  }

  if(id!=idJoueur)
  {
    index=indexOf(idJoueur, infoClients, nbClients);
    printf("Le joueur " COLOR_YELLOW "%s#%d" RESET " a marqué " COLOR_RED "%d" RESET " points.\n\n", infoClients[index].pseudo, infoClients[index].id, scoreDes);
  }

  msg.type=SCORE;
  for(int i=0; i<nbClients-1; i++)
  {
      if(write(socketClients[i],&msg,sizeof(message) )==-1)
      {
        perror("writeScore");
      }
  }

  return scoreDes;
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
    printf("\nAppuyez sur entrée pour commencer la partie.");
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
      if(idRandomized[j]<idRandomized[i])
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
}

void* threadLectureJeu(void* arg)
{
  char saisie;

  do
  {
    saisie=getchar();
  } while(saisie != '0' && saisie!='1');

  memcpy(arg, &saisie, sizeof(char));

  pthread_exit(NULL);
}

void* threadLectureHost(void* arg)
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
  message msg;

  printf(COLOR_RED "Arrêt du jeu...\n" RESET);
  decoServeur();

  bzero(&msg, sizeof(message));
  msg.type=UNLOG;
  for(int i=0; i<nbClients-1; i++)
  {
    if(write(socketClients[i],&msg,sizeof(message))==-1)
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
