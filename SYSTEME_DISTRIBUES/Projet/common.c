#include "common.h"
#include <math.h>

int creerSocketTCP(int port)
{
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    perror("creation socket");
    exit(1);
  }

  bzero((char *) &addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr=htonl(INADDR_ANY);
  if(bind(sock, (struct sockaddr*)&addr, sizeof(addr))== -1)
  {
    perror("erreur bind socket");
    exit(1);
  }

  return sock;
}

int calculerID(char* pseudo, int nbClients)
{
  int id=0;

  id+=nbClients+1;
  time_t currentTime=time(NULL);
  for(int i=0; i<4; i++)
  {
    id+=(currentTime%10)*pow(10,i);
    currentTime /= 10;
  }
  return id;
}

void removeClient(info_client infoClients[], int nbClients, int id)
{
  for(int i=0; i<nbClients; i++)
  {
    if(infoClients[i].id == id)
    {
      for(int j=i; j<nbClients-1; j++)
      {
        infoClients[j]=infoClients[j+1];
      }
    }
  }
}
