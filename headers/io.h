#ifndef IO_H
#define IO_H

#include <stdbool.h>
#include "structures.h"

// En tetes des fonctions
bool fichierExiste(const char* nomFichier);
void sauvegarderChambres(Chambre* listeChambres, const char* nomFichier);
Chambre* chargerChambres(const char* nomFichier);
void sauvegarderExtras(Extra* listeExtras, const char* nomFichier);
Extra* chargerExtras(const char* nomFichier);
int compterUtilisationExtra(Reservation* reservations, int idExtra);

// Répéter pour formules et réservations
void sauvegarderFormules(Formule* listeFormules, const char* nomFichier);
Formule* chargerFormules(const char* nomFichier);
void sauvegarderReservations(Reservation* listeReservations, const char* nomFichier);
Reservation* chargerReservations(const char* nomFichier);

void afficherMenu();

#endif //IO_H
