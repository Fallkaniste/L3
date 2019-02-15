#include "common.h"

int main(int argc, char** argv)
{
  struct hostent *serveur_host;   // identifiant de la machine serveur
  static struct sockaddr_in addr_serveur;   // adresse de la socket coté serveur
  socklen_t lg;   // taille de l'addresse socket
  int sock;   // descripteur de la socket locale
  char* hostName="scinfe056";
  long port;
  char *msg = "bonjour";    // chaine  à envoyer
  char buffer[TAILLEBUF];   // buffer de réception
  char *reponse;  // chaine reçue en réponse
  int nb_octets;  // nombre d'octets lus ou envoyés

  if(argc==3)
  {
    hostName=argv[1];
    port=strtol(argv[2], NULL, 10);
  }
  else
  {
    exit(-1);
  }



  // création d'une socket UDP
  sock = creerSocketUDP(0);

  // récupération identifiant du serveur
  serveur_host = gethostbyname(hostName);
  if (serveur_host==NULL)
  {
    perror("erreur adresse serveur");
    exit(1);
  }

  // création adresse socket destinatrice
  bzero(&addr_serveur, sizeof(struct sockaddr_in));
  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(port);
  memcpy(&addr_serveur.sin_addr.s_addr, serveur_host -> h_addr, serveur_host -> h_length);

  // on envoie le message "bonjour" au serveur
  lg = sizeof(struct sockaddr_in);
  nb_octets = sendto(sock, msg, strlen(msg)+1, 0, (struct sockaddr*)&addr_serveur, lg);
  if (nb_octets == -1)
  {
    perror("erreur envoi message");
    exit(1);
  }
  printf("paquet envoyé, nb_octets = %d\n",nb_octets);

  // on attend la réponse du serveur
  nb_octets = recvfrom(sock, buffer, TAILLEBUF, 0, (struct sockaddr*)&addr_serveur, &lg);
  if (nb_octets == -1)
  {
    perror("erreur réponse serveur");
    exit(1);
  }
  reponse = (char *)malloc(nb_octets * sizeof(char));
  memcpy(reponse, buffer, nb_octets);
  printf("reponse recue du serveur : %s\n",reponse);

  // on ferme la socket
  close(sock);

  return 0;
}
