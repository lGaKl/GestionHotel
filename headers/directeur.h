#ifndef DIRECTEUR_H
#define DIRECTEUR_H

void modifierPrixChambre(Chambre* listeChambres, int idChambre, float nouveauPrix, const char* nomFichier);
Chambre* supprimerChambre(Chambre* listeChambres, int idChambre, const char* nomFichier);
Extra* supprimerExtra(Extra* listeExtras, int idExtra, const char* nomFichier);
void modifierExtra(Extra* listeExtras, int idExtra, float nouveauPrix, const char* nomFichier);
float calculerRevenusTotaux(Reservation* listeReservations);
void afficherChambresPopulaires(Reservation* reservations, Chambre* chambres);
void afficherReservations(const char* nomFichier, Chambre* chambres, Formule* formules, Extra* extras);
void modifierPrixFormule(Formule* listeFormules, int idFormule, float nouveauPrix, const char* nomFichier);
Formule* supprimerFormule(Formule* listeFormules, int idFormule, const char* nomFichier);
Formule* ajouterFormule(Formule* listeFormules, int id, const char* nom, float prix, const char* nomFichier);
void afficherMenuDirecteur();
void gestionDirecteur(Chambre** chambres, Extra** extras, Formule* formules, Reservation* reservations);

#endif //DIRECTEUR_H
