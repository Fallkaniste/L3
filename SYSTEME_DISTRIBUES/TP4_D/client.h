#ifndef CLIENT_H
#define CLIENT_H

long factoriel(int nb);
int analyserDonnees(int donnees[], int taille,res_analyse_donnees *res);
long puissance(int nb, int puiss);
int ouvrirConnexion(char* host, int port);
void fermerConnexion();

#endif
