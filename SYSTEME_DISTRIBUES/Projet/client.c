#include "common.h"

#define EOOF 133
#define ENAN 134

void helpMessage();
void versionMessage();
int ouvrirConnexion(char* host, int port);
int stringToInt(char* optarg);

int main(int argc, char** argv)
{
  /*int opt=0;
  int port;
  char* hostname;

  do
  {
    opt=getopt_long(argc,argv,"hv", options,NULL);
    switch(opt)
    {
      case 'h':
        helpMessage();
        exit(EXIT_SUCCESS);
      case 'v':
        versionMessage();
        exit(EXIT_SUCCESS);
      default:
        if(argc==3)
        {
          hostname=argv[1];
          port=stringToInt(argv[2]);
          break;
        }
        break;
    }
  } while(opt!=-1);
  /*sock=ouvrirConnexion("localhost", 7777);*/

  /*int choix=-1;

  while(choix!=3)
  {
    choix=-1;

    do
    {
      printf("TODO"); //TODO
      scanf("%d", &choix);
    } while(choix<0 || choix>3);

    switch(choix)
    {
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
    }
  }*/

  return 0;
}

int ouvrirConnexion(char* host, int port)
{
  static struct sockaddr_in addr_serveur;
  struct hostent* host_serveur;

  //création de la socket locale
  int sock=creerSocketTCP(0);

  // récupération identifiants de la machine serveur
  host_serveur = gethostbyname(host);
  if (host_serveur==NULL)
  {
    perror("erreur récupération adresse serveur\n");
    exit(1);
  }

  // création de l'identifiant de la socket d'écoute du serveur
  bzero((char *) &addr_serveur, sizeof(addr_serveur));
  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(port);
  memcpy(&addr_serveur.sin_addr.s_addr, host_serveur->h_addr, host_serveur->h_length);

  // connexion de la socket client locale à la socket coté serveur
  if (connect(sock, (struct sockaddr *)&addr_serveur, sizeof(struct sockaddr_in)) == -1)
  {
    perror("erreur connexion serveur");
    exit(1);
  }

  return sock;
}

int stringToInt(char* optarg)
{
  char* endptr=NULL;
  long value=strtol(optarg, &endptr, 10);

  if (endptr!=optarg+strlen(optarg))
  {
    errno=ENAN;
    fprintf(stderr, "ENAN\n");
  }
  if(value>=INT_MAX||value<=INT_MIN)
  {
    errno=EOOF;
    fprintf(stderr, "EOOF\n");
  }
  return (int)value;
}

void helpMessage()
{
  printf("Options:\n \
  \t-h, --help display this help and exit\n \
  \t-v, --version output version information and exit\n");
  printf("Report bugs to Croquefer Gaëtan <gaetan.croquefer@etud.univ-pau.fr> and Amiard Landry <landry.amiard@etud.univ-pau.fr>.\n");
}

void versionMessage()
{
  printf("cdc-client 1.0\nCopyright (C) 2019 Croquefer Gaëtan and Amiard Landry.\nWritten by Croquefer Gaëtan <gaetan.croquefer@etud.univ-pau.fr> and Amiard Landry <landry.amiard@etud.univ-pau.fr>.\n");
}
