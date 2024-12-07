#include <stdio.h>
#include <stdlib.h>
#include "headers/structures.h"
#include "headers/display.h"
#include "headers/io.h"
#include "time.h"

// Ajouter une réservation
Reservation* ajouterReservation(Reservation* head, int idClient, int idChambre, int idFormule, int* idExtras, int nbExtras, float total) {
    // Validation des données
    if (idClient <= 0 || idChambre <= 0 || total <= 0) {
        printf("Erreur : Données invalides pour la réservation.\n");
        return head;
    }

    Reservation* new = (Reservation*)malloc(sizeof(Reservation));
    if (!new) return head;

    new->idClient = idClient;
    new->idChambre = idChambre;
    new->idFormule = idFormule;
    new->idExtras = (int*)malloc(sizeof(int) * nbExtras);

    if (new->idExtras) {
        for (int i = 0; i < nbExtras; i++) {
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

// Calcul du total d'une réservation
float calculerTotal(Chambre* chambres, Formule* formules, Extra* extras, int idChambre, int idFormule, int* idExtras, int nbExtras) {
    float total = 0.0;

    while (chambres) {
        if (chambres->id == idChambre) {
            total += chambres->prix;
            break;
        }
        chambres = chambres->next;
    }

    while (formules) {
        if (formules->id == idFormule) {
            total += formules->prix;
            break;
        }
        formules = formules->next;
    }

    for (int i = 0; i < nbExtras; i++) {
        Extra* current = extras;
        while (current) {
            if (current->id == idExtras[i]) {
                total += current->prix;
                break;
            }
            current = current->next;
        }
    }

    return total;
}

// Faire une réservation avec confirmation et vérification
#include <time.h> // Pour l'initialisation de srand

void faireReservation(Chambre* chambres, Formule* formules, Extra* extras, Reservation** reservations) {
    srand((unsigned int)time(NULL)); // Initialisation de la génération aléatoire (si ce n'est pas déjà fait ailleurs)

    int idChambre, idFormule, nbExtras, nbAdultes, nbEnfants, totalPersonnes;

    // Afficher et choisir une chambre
    afficherChambresDisponibles(chambres);
    printf("Entrez l'ID de la chambre à réserver : ");
    scanf("%d", &idChambre);

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

    totalPersonnes = nbAdultes + nbEnfants;
    if (totalPersonnes > 2) {
        printf("Erreur : Une chambre ne peut accueillir que 2 personnes. Veuillez réserver une autre chambre pour les personnes supplémentaires.\n");
        return;
    }

    afficherFormulesDisponibles(formules);
    printf("Entrez l'ID de la formule à ajouter : ");
    scanf("%d", &idFormule);

    afficherExtrasDisponibles(extras);
    printf("Combien d'extras souhaitez-vous ajouter ? ");
    scanf("%d", &nbExtras);

    int* idExtras = (int*)malloc(nbExtras * sizeof(int));
    for (int i = 0; i < nbExtras; i++) {
        printf("Entrez l'ID de l'extra %d : ", i + 1);
        scanf("%d", &idExtras[i]);
    }

    float totalBase = calculerTotal(chambres, formules, extras, idChambre, idFormule, idExtras, nbExtras);
    float total = totalBase * (nbAdultes + 0.5 * nbEnfants);

    printf("Le total de votre réservation est de %.2f €. Confirmez-vous (1 : Oui, 0 : Non) ? ", total);
    int confirmation;
    scanf("%d", &confirmation);
    if (confirmation != 1) {
        printf("Réservation annulée.\n");
        free(idExtras);
        return;
    }

    int idClient = rand() % 100000 + 1;
    Reservation* current = *reservations;
    while (current != NULL) {
        if (current->idClient == idClient) { // Assurer l'unicité
            idClient = rand() % 100000 + 1;
            current = *reservations;
        } else {
            current = current->next;
        }
    }

    // Vérifier les doublons de réservation
    current = *reservations;
    while (current != NULL) {
        if (current->idClient == idClient && current->idChambre == idChambre) {
            printf("Erreur : Une réservation avec ces informations existe déjà.\n");
            free(idExtras);
            return;
        }
        current = current->next;
    }

    Reservation* nouvelle = ajouterReservation(*reservations, idClient, idChambre, idFormule, idExtras, nbExtras, total);
    if (nouvelle) {
        nouvelle->nbAdultes = nbAdultes;
        nouvelle->nbEnfants = nbEnfants;
        *reservations = nouvelle;

        courantChambre->disponible = 0;
        sauvegarderReservations(*reservations, "../datas/reservations.txt");

        printf("Réservation effectuée avec succès ! Total : %.2f €.\n", total);
    } else {
        printf("Erreur : Impossible d'ajouter la réservation.\n");
    }

    free(idExtras);
}