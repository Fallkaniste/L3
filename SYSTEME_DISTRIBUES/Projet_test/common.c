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

void afficherClients(info_client infoClients[], int nbClients, int id)
{
  //system("clear");
  printf(BOLD "NOMBRE DE JOUEURS:" FONT_RESET COLOR_RED " %d\n\n" COLOR_RESET BOLD "LISTE JOUEURS\n" FONT_RESET, nbClients);
  for(int i=0; i<nbClients; i++)
  {
    printf(UNDERLINE "%d:" FONT_RESET "%s#%04d",i+1,infoClients[i].pseudo,infoClients[i].id);
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
