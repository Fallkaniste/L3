#include "common.h"

int creerSocketUDP(int port)
{
  int sock;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1)
  {
    perror("erreur création socket");
    return -1;
  }

  bzero(&addr_local, sizeof(struct sockaddr_in));
  addr_local.sin_family = AF_INET;
  addr_local.sin_port = htons(port);
  addr_local.sin_addr.s_addr=htonl(INADDR_ANY);
  if( bind(sock, (struct sockaddr*)&addr_local, sizeof(addr_local))== -1 )
  {
    perror("erreur bind");
    return -1;
  }

  return sock;
}
