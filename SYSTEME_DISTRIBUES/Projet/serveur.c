#include "common.h"

int main()
{
  int socketMulticast;
  ip_mreq grMulticast;
  sockaddr_in adrMulticast;
  char rBuffer[sizeof(info_client)];
  int nbClients=0;
  info_client infoClients[NB_CLIENT_MAX];
  int id;
  char wBuffer[2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client)];
  int socketService;
  int nbOctets;
  info_client tmp;


  //Création de la socket UDP
  socketMulticast = socket(AF_INET, SOCK_DGRAM, 0);
  if(socketMulticast==-1)
  {
    perror("socket");
  }

  //Setup du multicast
  if(setsockopt(socketMulticast, SOL_SOCKET, SO_REUSEADDR,&(int){ 1 }, sizeof(int))==-1)
  {
    perror("setsockopt1");
  }

  bzero((char *) &adrMulticast, sizeof(adrMulticast));
  adrMulticast.sin_family = AF_INET;
  adrMulticast.sin_addr.s_addr = inet_addr(MULTICAST_ADR);
  adrMulticast.sin_port = htons(MULTICAST_PORT);
  if(bind(socketMulticast, (sockaddr*)&adrMulticast, sizeof(sockaddr_in))==-1)
  {
    perror("bind");
  }

  bzero((char*)&grMulticast,sizeof(ip_mreq));
  grMulticast.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADR);
  grMulticast.imr_interface.s_addr=htonl(INADDR_ANY);
  if(setsockopt(socketMulticast, IPPROTO_IP, IP_ADD_MEMBERSHIP,&grMulticast, sizeof(ip_mreq))==-1)
  {
    perror("setsockopt2");
  }

  //Init list
  bzero((char*)infoClients, 4*sizeof(info_client));

  while(1)
  {
    //Réception message
    nbOctets=read(socketMulticast,rBuffer,sizeof(info_client));
    if(nbOctets==sizeof(info_client))
    {
      if(nbClients<4)
      {
        //Ajout des infos du client dans la liste
        memcpy(&infoClients[nbClients],rBuffer,sizeof(info_client));
        id=calculerID(infoClients[nbClients].pseudo, nbClients);
        infoClients[nbClients].id=id;
        printf("Nouveau client! Pseudo:%s#%d\n",infoClients[nbClients].pseudo, id);
        nbClients++;

        //Remplissage du buffer d'envoi
        bzero(wBuffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client));
        memcpy(wBuffer,&id,sizeof(int));
        memcpy(wBuffer+sizeof(int),&nbClients,sizeof(int));
        for(int i=0; i<nbClients; i++)
        {
          memcpy(wBuffer+(2*sizeof(int))+(i*sizeof(info_client)),&infoClients[i],sizeof(info_client));
        }
        //Envoi au client
        socketService = creerSocketTCP(0);
        if(connect(socketService,(sockaddr*)&infoClients[nbClients-1].adr,sizeof(sockaddr_in)) == -1)
        {
          perror("connect");
        }

        if(write(socketService,wBuffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client))==-1)
        {
          perror("write");
        }

        close(socketService);
      }
      else
      {
        printf("Nombre de connexions maximal atteint.\n");

        memcpy(&tmp,rBuffer,sizeof(info_client));

        //Envoi au client
        socketService = creerSocketTCP(0);
        if(connect(socketService,(sockaddr*)&tmp.adr,sizeof(sockaddr_in)) == -1)
        {
          perror("connect");
        }

        if(write(socketService,&(int){-1},sizeof(int))==-1)
        {
          perror("write");
        }

        close(socketService);

      }
    }
    else if(nbOctets==sizeof(int))
    {
      memcpy(&id,rBuffer,sizeof(int));
      if(id==0)
      {
        //TODO commencer partie
      }
      else
      {
        for(int i=0; i<nbClients; i++)
        {
          if(id==infoClients[i].id)
          {
            printf("Le joueur #%d s'est déconnecté.\n", id);
            removeClient(infoClients, nbClients, id);
            nbClients--;
          }
        }
      }
    }
    else
    {
      printf("MESSAGE NON ATTENDU\n");
      exit(EXIT_FAILURE);
    }
  }
  return 0;

}
