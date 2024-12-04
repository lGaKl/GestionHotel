#ifndef CLIENT_H
#define CLIENT_H

#include "structures.h"

Reservation* ajouterReservation(Reservation* head, int idClient, int idChambre, int idFormule, int* idExtras, int nbExtras, float total);
float calculerTotal(Chambre* chambres, Formule* formules, Extra* extras, int idChambre, int idFormule, int* idExtras, int nbExtras);
void faireReservation(Chambre* chambres, Formule* formules, Extra* extras, Reservation** reservations);

#endif
