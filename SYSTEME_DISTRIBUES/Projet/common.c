#include "common.h"

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
  //TODO use system time

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
    id+=nbClients+1;
    time_t currentTime=time(NULL);
    while (currentTime > 0)
    {
     id+=(int)currentTime%10L;
     currentTime /= 10L;
    }
    return id;
  }
  return -1;
}
