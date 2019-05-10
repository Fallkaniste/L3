#include "common.h"

int makeID(char* pseudo, int nbClients);

int main()
{
  int socketMulticast;
  ip_mreq grMulticast;
  int reuse = 1;
  sockaddr_in adrMulticast;
  char rBuffer[sizeof(info_client)];
  socklen_t tailleAdrClient;
  int nbClients=0;
  info_client infoClients[NB_CLIENT_MAX];
  int id;
  char wBuffer[2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client)];
  int socketService;


  //Création de la socket UDP
  socketMulticast = socket(AF_INET, SOCK_DGRAM, 0);
  if(socketMulticast==-1)
  {
    //TODO gestion erreur
  }

  //Création du groupe multicast
  grMulticast.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADR);
  grMulticast.imr_interface.s_addr = htons(MULTICAST_PORT);
  if(setsockopt(socketMulticast, IPPROTO_IP, IP_ADD_MEMBERSHIP,&grMulticast, sizeof(struct ip_mreq)==-1))
  {
    //TODO gestion erreur
  }
  if(setsockopt(socketMulticast, SOL_SOCKET, SO_REUSEADDR,(int *)&reuse, sizeof(reuse)==-1))
  {
    //TODO gestion erreur
  }

  //Bind du groupe sur la socket
  bzero((char *) &adrMulticast, sizeof(adrMulticast));
  adrMulticast.sin_family = AF_INET;
  adrMulticast.sin_addr.s_addr = inet_addr(MULTICAST_ADR);
  adrMulticast.sin_port = htons(MULTICAST_PORT);
  bind(socketMulticast, (struct sockaddr*)&adrMulticast, sizeof(sockaddr_in));

  //Réception message
  if(recvfrom(socketMulticast,rBuffer,sizeof(info_client),0,(struct sockaddr *) &adrMulticast,&tailleAdrClient)!=sizeof(info_client))
  {
      //TODO gestion erreur
  }

  //Ajout des infos du client dans la liste
  bzero((char*)infoClients, 4*sizeof(info_client));
  memcpy(&infoClients[nbClients],rBuffer,sizeof(info_client));
  printf("pseudo:%s\n",infoClients[0].pseudo);
  nbClients++;

  //Remplissage du buffer d'envoi
  bzero(wBuffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client));
  id=makeID(infoClients[nbClients-1].pseudo, nbClients);
  memcpy(&wBuffer,&id,sizeof(int));
  memcpy(&wBuffer+sizeof(int),&nbClients,sizeof(int));
  printf("wBuffer:%d\n", sizeof(wBuffer));
  for(int i=0; i<nbClients; i++)
  {
    memcpy(&wBuffer+(2*sizeof(int))+(i*sizeof(info_client)),&infoClients[i],sizeof(info_client));
  }

  //Envoi au client
  socketService = creerSocketTCP(0);
  if(connect(socketService,(struct sockaddr*)&infoClients[nbClients-1].adr,sizeof(struct sockaddr_in)) == -1)
  {
    //TODO gestion erreur
  }

  if(write(socketService,wBuffer,2*sizeof(int)+NB_CLIENT_MAX*sizeof(info_client))==-1)
  {
    //TODO gestion erreur
  }

  close(socketService);

  return 0;

}

int makeID(char* pseudo, int nbClients)
{
  int i=0;
  int id=0;

  if(pseudo!=NULL)
  {
    do
    {
      i++;
      if(pseudo[i]!='\0')
      {
        id+=pseudo[i];
      }
      else
      {
        break;
      }
    } while(i<L_PSEUDO_MAX);
    id+=nbClients;
    return id;
  }
  return -1;
}
