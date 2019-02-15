#include "common.h"

int main(int argc, char** argv)
{
  static struct sockaddr_in addr_local; // adresse de la socket locale
  static struct sockaddr_in addr_client;  // adresse de la socket coté serveur
  struct hostent *host_client;  // identifiant du client
  socklen_t lg; // taille de l'addresse socket
  long port=4000;
  int sock; // descripteur de la socket locale
  char *reponse = "bien recu";  // chaine  à envoyer en réponse
  char buffer[TAILLEBUF]; // buffer de réception
  char *chaine; // chaine reçue
  int nb_octets;  // nombre d'octets lus ou envoyés

  if(argc==2)
  {
    port=strtol(argv[1], NULL, 10);

  }

  sock=creerSocketUDP(port);

  // attente de données venant d'un client
  lg = sizeof(struct sockaddr_in);
  nb_octets = recvfrom(sock, buffer, TAILLEBUF, 0, (struct sockaddr *)&addr_client, &lg);
  if (nb_octets == -1)
  {
    perror("erreur réception paquet");
    exit(1);
  }

  // récupère nom de la machine émettrice des données
  host_client = gethostbyaddr(&(addr_client.sin_addr), sizeof(long), AF_INET);
  if (host_client == NULL)
  {
    perror("erreur gethostbyaddr");
    exit(1);
  }

  // affichage message reçu et coordonnées émetteur
  chaine = (char *)malloc(nb_octets * sizeof(char));
  memcpy(chaine, buffer, nb_octets);
  printf("recu message %s de la part de %s  sur le port %d\n", chaine, host_client->h_name, ntohs(addr_client.sin_port));

  // envoi de la réponse à l'émetteur
  nb_octets = sendto(sock, reponse, strlen(reponse)+1, 0,(struct sockaddr*)&addr_client, lg);
  if (nb_octets == -1)
  {
    perror("erreur envoi réponse");
    exit(1);
  }

  // fermeture la socket
  close(sock);
  return 0;
}
