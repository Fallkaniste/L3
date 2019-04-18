#include "common.h"

#define multicast "226.1.2.3"

int main()
{
  int sock;
  struct in_addr ip;
  static struct sockaddr_in ad_multicast, adresse;
  struct ip_mreq gr_multicast;
  // création de la socket UDP
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  // récupération adresse ip du groupe
  inet_aton(multicast, &ip);
  // création identificateur du groupe
  gr_multicast.imr_multiaddr.s_addr = ip.s_addr;
  gr_multicast.imr_interface.s_addr = htons(INADDR_ANY);
  // abonnement de la socket au groupe multicast
  setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,&gr_multicast, sizeof(struct ip_mreq));
  // autorise de lier plusieurs sockets sur le port utilisé par cette
  // socket, c'est-à-dire sur le port du groupe multicast
  int reuse = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,(int *)&reuse, sizeof(reuse));
  // liaison de la socket au port du groupe multicast
  bzero((char *) &adresse, sizeof(adresse));
  ad_multicast.sin_family = AF_INET;
  ad_multicast.sin_addr.s_addr = htons(INADDR_ANY);
  ad_multicast.sin_port = htons(1234);
  bind(sock, (struct sockaddr*)&adresse, sizeof(struct sockaddr_in));


  return 0;
}
