#ifndef DISPLAY_H
#define DISPLAY_H
#include "structures.h"

//En tete des méthodes
Chambre* trierChambresParID(Chambre* liste);
Formule* trierFormulesParID(Formule* liste);
Extra* trierExtrasParID(Extra* liste);
void afficherChambresDisponibles(Chambre* listeChambres);
void afficherChambresNonDisponibles(Chambre* listeChambres);
void afficherFormulesDisponibles(Formule* listeFormules);
void afficherTousExtras(Extra* listeExtras);
void afficherExtrasDisponibles(Extra* listeExtras);
void afficherExtrasNonDisponibles(Extra* listeExtras);
void afficherToutesChambres(Chambre* listeChambres);
void afficherMenuConsultation();
void consulterInformations(Chambre* listeChambres, Formule* listeFormules, Extra* listeExtras);

#endif //DISPLAY_H
