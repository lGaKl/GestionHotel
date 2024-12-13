#include <stdio.h>
#include "headers/structures.h"
#include "headers/io.h"
#include "headers/client.h"
#include "headers/directeur.h"
#include "headers/display.h"

int main() {
    // Chargement des données
    Chambre* listeChambres = chargerChambres("../datas/chambres.txt");
    Extra* listeExtras = chargerExtras("../datas/extras.txt");
    Formule* listeFormules = chargerFormules("../datas/formules.txt");
    Reservation* listeReservations = chargerReservations("../datas/reservations.txt");

    int choix;
    do {
        afficherMenu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                gestionDirecteur(&listeChambres, &listeExtras, &listeFormules, listeReservations);
            break;
            case 2:
                faireReservation(listeChambres, listeFormules, listeExtras, &listeReservations);
            break;
            case 3:
                consulterInformations(listeChambres, listeFormules, listeExtras);
            break;
            case 4:
                printf("Au revoir !\n");
            break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 4);

    return 0;
}
