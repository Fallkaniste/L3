#include "common.h"

int main(int argc, char** argv)
{
  // adresse socket coté client
  static struct sockaddr_in addr_client;
  // longueur adresse
  int lg_addr;
  // socket d'écoute et de service
  int socket_ecoute, socket_service;
  // buffer qui contiendra le message reçu
  char message[TAILLEBUF];
  // chaîne reçue du client
  char *chaine_recue;
  // chaîne renvoyée au client
  char *reponse = "bien recu";
  // nombre d'octets reçus ou envoyés
  int nb_octets;

  int port=7777;


  if(argc==2)
  {
    port=strtol(argv[1], NULL, 10);
  }
  else
  {
    exit(-1);
  }

  // création socket TCP d'écoute
  socket_ecoute=creerSocketTCP(port);


  // configuration socket écoute : 5 connexions max en attente
  if (listen(socket_ecoute, 5) == -1)
  {
    perror("erreur listen");
    exit(1);
  }

  // on attend la connexion du client
  lg_addr = sizeof(struct sockaddr_in);
  socket_service = accept(socket_ecoute, (struct sockaddr *)&addr_client, (socklen_t *)&lg_addr);
  if (socket_service == -1)
  {
    perror("erreur accept");
    exit(1);
  }

  // la connexion est établie, on attend les données envoyées par le client
  nb_octets = read(socket_service, message, TAILLEBUF);
  chaine_recue =(char *)malloc(nb_octets * sizeof(char));
  memcpy(chaine_recue, message, nb_octets);

  FILE* fichier = NULL;

  fichier = fopen("reponse.jpg", "wb");

  if (fichier != NULL)
  {
    fputs(chaine_recue, fichier);
    fclose(fichier);
  }

  // on envoie la réponse au client
  write(socket_service, reponse, strlen(reponse)+1);

  // on ferme les sockets
  close(socket_service);
  close(socket_ecoute);

  return 0;
}
