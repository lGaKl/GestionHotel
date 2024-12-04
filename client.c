#include <stdio.h>
#include <stdlib.h>
#include "headers/structures.h"
#include "headers/display.h"
#include "headers/io.h"

//Ajouter une réservation
Reservation* ajouterReservation(Reservation* head, int idClient, int idChambre, int idFormule, int* idExtras, int nbExtras, float total) {
    Reservation* new = (Reservation*)malloc(sizeof(Reservation));

    if(!new) return head;

    new->idClient = idClient;
    new->idChambre = idChambre;
    new->idFormule = idFormule;
    new->idExtras = (int*)malloc(sizeof(int) * nbExtras);

    if(new->idExtras) {
        for(int i = 0; i < nbExtras; i++) {
            new->idExtras[i] = idExtras[i];
        }
    }

    new->nbExtras = nbExtras;
    new->total = total;
    new->nbAdultes = 0;
    new->nbEnfants = 0;
    new->next = head;
    return new;
}

//Calcul du total d'une réservation
float calculerTotal(Chambre* chambres, Formule* formules, Extra* extras, int idChambre, int idFormule, int* idExtras, int nbExtras) {
    float total = 0.0;

    while(chambres) {
        if(chambres->id == idChambre) {
            total += chambres->prix;
            break;
        }
        chambres = chambres->next;
    }

    while(formules) {
        if(formules->id == idFormule) {
            total += formules->prix;
            break;
        }
        formules = formules->next;
    }

    for(int i = 0; i < nbExtras; i++) {
        Extra* current = extras;
        while(current) {
            if(current->id == idExtras[i]) {
                total += current->prix;
                break;
            }
            current = current->next;
        }
    }

    return total;
}

//Faire un réservation avec confirmation et vérification
void faireReservation(Chambre* chambres, Formule* formules, Extra* extras, Reservation** reservations) {
    int idChambre, idFormule, nbExtras, nbAdultes, nbEnfants, totalPersonnes;

    // Message d'info sur les tarifs
    printf("\n--- Informations sur les Tarifs ---\n");
    printf("1. Les adultes paient le plein tarif pour la chambre, la formule et les extras.\n");
    printf("2. Les enfants paient la moitié du tarif pour tout.\n");
    printf("-----------------------------------\n");

    // Afficher et choisir une chambre
    afficherChambresDisponibles(chambres);
    printf("Entrez l'ID de la chambre à réserver : ");
    scanf("%d", &idChambre);

    // Vérifier si la chambre est disponible
    Chambre* courantChambre = chambres;
    while (courantChambre != NULL) {
        if (courantChambre->id == idChambre) {
            if (!courantChambre->disponible) {
                printf("Erreur : La chambre ID %d est déjà réservée.\n", idChambre);
                return;
            }
            break;
        }
        courantChambre = courantChambre->next;
    }
    if (courantChambre == NULL) {
        printf("Erreur : La chambre ID %d n'existe pas.\n", idChambre);
        return;
    }

    // Saisir le nombre d'adultes et d'enfants
    printf("Entrez le nombre d'adultes : ");
    scanf("%d", &nbAdultes);
    if (nbAdultes < 0) {
        printf("Erreur : Le nombre d'adultes ne peut pas être négatif.\n");
        return;
    }
    printf("Entrez le nombre d'enfants : ");
    scanf("%d", &nbEnfants);
    if (nbEnfants < 0) {
        printf("Erreur : Le nombre d'enfants ne peut pas être négatif.\n");
        return;
    }

    // Vérifier la capacité de la chambre
    totalPersonnes = nbAdultes + nbEnfants;
    if (totalPersonnes > 2) {
        printf("Erreur : Une chambre ne peut accueillir que 2 personnes. Veuillez réserver une autre chambre pour les personnes supplémentaires.\n");
        return;
    }

    // Afficher et choisir une formule
    afficherFormulesDisponibles(formules);
    printf("Entrez l'ID de la formule à ajouter : ");
    scanf("%d", &idFormule);

    // Vérifier si la formule existe
    Formule* courantFormule = formules;
    while (courantFormule != NULL) {
        if (courantFormule->id == idFormule) {
            break;
        }
        courantFormule = courantFormule->next;
    }
    if (courantFormule == NULL) {
        printf("Erreur : La formule ID %d n'existe pas.\n", idFormule);
        return;
    }

    // Afficher et choisir les extras
    afficherExtrasDisponibles(extras);
    printf("Combien d'extras souhaitez-vous ajouter ? ");
    scanf("%d", &nbExtras);

    int* idExtras = (int*)malloc(nbExtras * sizeof(int));
    for (int i = 0; i < nbExtras; i++) {
        printf("Entrez l'ID de l'extra %d : ", i + 1);
        scanf("%d", &idExtras[i]);

        // Vérifier si l'extra existe
        Extra* courantExtra = extras;
        int extraTrouve = 0;
        while (courantExtra != NULL) {
            if (courantExtra->id == idExtras[i]) {
                int utilisationActuelle = compterUtilisationExtra(*reservations, idExtras[i]);
                if (utilisationActuelle + nbAdultes + nbEnfants > 5) {
                    printf("Erreur : L'extra ID %d a déjà été utilisé par 5 personnes ou plus.\n", idExtras[i]);
                    free(idExtras);
                    return;
                }
                extraTrouve = 1;
                break;
            }
            courantExtra = courantExtra->next;
        }
        if (!extraTrouve) {
            printf("Erreur : L'extra ID %d n'existe pas.\n", idExtras[i]);
            free(idExtras);
            return;
        }
    }

    // Calculer le total
    float totalBase = calculerTotal(chambres, formules, extras, idChambre, idFormule, idExtras, nbExtras);
    float total = totalBase * (nbAdultes + 0.5 * nbEnfants); // Les enfants paient moitié prix

    // Confirmer la réservation
    printf("Le total de votre réservation est de %.2f €. Confirmez-vous (1 : Oui, 0 : Non) ? ", total);
    int confirmation;
    scanf("%d", &confirmation);
    if (confirmation != 1) {
        printf("Réservation annulée.\n");
        free(idExtras);
        return;
    }

    // Ajouter la réservation
    int idClient = rand() % 1000 + 1; // Générer un ID client unique
    Reservation* nouvelle = ajouterReservation(*reservations, idClient, idChambre, idFormule, idExtras, nbExtras, total);
    nouvelle->nbAdultes = nbAdultes;
    nouvelle->nbEnfants = nbEnfants;
    *reservations = nouvelle;

    // Marquer la chambre comme indisponible
    courantChambre->disponible = 0;

    sauvegarderReservations(*reservations, "../datas/reservations.txt");

    printf("Réservation effectuée avec succès ! Total : %.2f €.\n", total);
    free(idExtras);
}
