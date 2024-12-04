#include "headers/display.h"

#include <stdio.h>
#include <string.h>

#include "headers/structures.h"


Chambre* trierChambresParID(Chambre* liste) {
    if (liste == NULL || liste->next == NULL) {
        return liste; // Liste vide ou avec un seul élément, déjà triée
    }

    Chambre* courant = liste;
    Chambre* index = NULL;
    Chambre temp;

    // Tri par sélection
    while (courant != NULL) {
        index = courant->next;
        while (index != NULL) {
            if (courant->id > index->id) {
                // Échanger les données entre les deux chambres
                temp.id = courant->id;
                strcpy(temp.type, courant->type);
                temp.prix = courant->prix;
                temp.disponible = courant->disponible;

                courant->id = index->id;
                strcpy(courant->type, index->type);
                courant->prix = index->prix;
                courant->disponible = index->disponible;

                index->id = temp.id;
                strcpy(index->type, temp.type);
                index->prix = temp.prix;
                index->disponible = temp.disponible;
            }
            index = index->next;
        }
        courant = courant->next;
    }

    return liste;
}

Formule* trierFormulesParID(Formule* liste) {
    if (liste == NULL || liste->next == NULL) {
        return liste;
    }

    Formule* courant = liste;
    Formule* index = NULL;
    Formule temp;

    while (courant != NULL) {
        index = courant->next;
        while (index != NULL) {
            if (courant->id > index->id) {
                // Échanger les données entre les deux formules
                temp.id = courant->id;
                strcpy(temp.nom, courant->nom);
                temp.prix = courant->prix;

                courant->id = index->id;
                strcpy(courant->nom, index->nom);
                courant->prix = index->prix;

                index->id = temp.id;
                strcpy(index->nom, temp.nom);
                index->prix = temp.prix;
            }
            index = index->next;
        }
        courant = courant->next;
    }

    return liste;
}

Extra* trierExtrasParID(Extra* liste) {
    if (liste == NULL || liste->next == NULL) {
        return liste;
    }

    Extra* courant = liste;
    Extra* index = NULL;
    Extra temp;

    while (courant != NULL) {
        index = courant->next;
        while (index != NULL) {
            if (courant->id > index->id) {
                // Échanger les données entre les deux extras
                temp.id = courant->id;
                strcpy(temp.nom, courant->nom);
                temp.prix = courant->prix;

                courant->id = index->id;
                strcpy(courant->nom, index->nom);
                courant->prix = index->prix;

                index->id = temp.id;
                strcpy(index->nom, temp.nom);
                index->prix = temp.prix;
            }
            index = index->next;
        }
        courant = courant->next;
    }

    return liste;
}


void afficherChambresDisponibles(Chambre* listeChambres) {

    listeChambres = trierChambresParID(listeChambres);

    printf("\n--- Chambres Disponibles ---\n");
    Chambre* courant = listeChambres;
    while (courant != NULL) {
        if (courant->disponible) {
            printf("ID : %d, Type : %s, Prix : %.2f\n", courant->id, courant->type, courant->prix);
        }
        courant = courant->next;
    }
    printf("---------------------------\n");
}

void afficherFormulesDisponibles(Formule* listeFormules) {

    listeFormules = trierFormulesParID(listeFormules);

    printf("\n--- Formules Disponibles ---\n");
    Formule* courant = listeFormules;
    while (courant != NULL) {
        printf("ID : %d, Nom : %s, Prix : %.2f\n", courant->id, courant->nom, courant->prix);
        courant = courant->next;
    }
    printf("---------------------------\n");
}

void afficherToutesChambres(Chambre* listeChambres) {

    trierChambresParID(listeChambres);

    printf("\n--- Liste de toutes les chambres ---\n");
    Chambre* courant = listeChambres;
    while (courant != NULL) {
        printf("ID : %d, Type : %s, Prix : %.2f, Disponible : %s\n",
               courant->id,
               courant->type,
               courant->prix,
               courant->disponible ? "Oui" : "Non");
        courant = courant->next;
    }
    printf("-----------------------------------\n");
}

void afficherChambresNonDisponibles(Chambre* listeChambres) {
    printf("\n--- Chambres Non Disponibles ---\n");
    Chambre* courant = listeChambres;
    while (courant != NULL) {
        if (!courant->disponible) { // Afficher uniquement les chambres non disponibles
            printf("ID : %d, Type : %s, Prix : %.2f\n",
                   courant->id,
                   courant->type,
                   courant->prix);
        }
        courant = courant->next;
    }
    printf("---------------------------------\n");
}


void afficherMenuConsultation() {
    printf("\n--- Consultation des Informations ---\n");
    printf("1. Consulter les chambres (toutes, non disponibles, disponibles\n");
    printf("2. Consulter les formules\n");
    printf("3. Consulter les extras\n");
    printf("4. Retourner au menu principal\n");
    printf("Votre choix : ");
}

void consulterInformations(Chambre* listeChambres, Formule* listeFormules, Extra* listeExtra) {
    int choix;

    do {
        afficherMenuConsultation();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherToutesChambres(listeChambres);
                afficherChambresNonDisponibles(listeChambres);
                afficherChambresDisponibles(listeChambres);
            break;
            case 2:
                afficherFormulesDisponibles(listeFormules);
            break;
            case 3:
                afficherExtrasDisponibles(listeExtra);
            break;
            case 4:
                printf("Retour au menu principal...\n");
            break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 4);
}
