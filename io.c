#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "headers/structures.h"

// Vérifie si un fichier existe
bool fichierExiste(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier) {
        fclose(fichier);
        return true;
    }
    return false;
}

// Sauvegarde des chambres dans un fichier
void sauvegarderChambres(Chambre* listeChambres, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "w");
    if (!fichier) {
        printf("Erreur : Impossible de créer le fichier %s\n", nomFichier);
        return;
    }

    Chambre* courant = listeChambres;
    while (courant != NULL) {
        fprintf(fichier, "%d;%s;%.2f;%d\n", courant->id, courant->type, courant->prix, courant->disponible);
        courant = courant->next;
    }
    fclose(fichier);
    printf("Chambres sauvegardées dans %s.\n", nomFichier);
}

// Chargement des chambres depuis un fichier
Chambre* chargerChambres(const char* nomFichier) {
    if (!fichierExiste(nomFichier)) {
        printf("Avertissement : Fichier %s introuvable. Liste vide.\n", nomFichier);
        return NULL;
    }

    FILE* fichier = fopen(nomFichier, "r");
    Chambre* tete = NULL;
    int id, disponible, count[100] = {0}; // Compteur par type
    char type[20];
    float prix;

    while (fscanf(fichier, "%d;%19[^;];%f;%d\n", &id, type, &prix, &disponible) == 4) {
        if (disponible && count[type[0]] < 3) { // Limite à 3 chambres disponibles par type
            count[type[0]]++;
            Chambre* nouvelle = (Chambre*)malloc(sizeof(Chambre));
            nouvelle->id = id;
            strcpy(nouvelle->type, type);
            nouvelle->prix = prix;
            nouvelle->disponible = disponible;
            nouvelle->next = tete;
            tete = nouvelle;
        }
    }
    fclose(fichier);
    return tete;
}


// Sauvegarde des extras dans un fichier
void sauvegarderExtras(Extra* listeExtras, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "w");
    if (!fichier) {
        printf("Erreur : Impossible de créer le fichier %s\n", nomFichier);
        return;
    }

    Extra* courant = listeExtras;
    while (courant != NULL) {
        fprintf(fichier, "%d;%s;%.2f;%d\n", courant->id, courant->nom, courant->prix, courant->disponible);
        courant = courant->next;
    }
    fclose(fichier);
    printf("Extras sauvegardés dans %s.\n", nomFichier);
}

// Chargement des extras depuis un fichier
Extra* chargerExtras(const char* nomFichier) {
    if (!fichierExiste(nomFichier)) {
        printf("Avertissement : Fichier %s introuvable. Liste vide.\n", nomFichier);
        return NULL;
    }

    FILE* fichier = fopen(nomFichier, "r");
    Extra* tete = NULL;
    int id;
    char nom[50];
    float prix;
    int disponible;

    while (fscanf(fichier, "%d;%49[^;];%f;%d\n", &id, nom, &prix, &disponible) == 4) {
        Extra* nouvelExtra = (Extra*)malloc(sizeof(Extra));
        nouvelExtra->id = id;
        strcpy(nouvelExtra->nom, nom);
        nouvelExtra->prix = prix;
        nouvelExtra->disponible = disponible;
        nouvelExtra->next = tete;
        tete = nouvelExtra;
    }
    fclose(fichier);
    return tete;
}

// Sauvegarde des formules dans un fichier
void sauvegarderFormules(Formule* listeFormules, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "w");
    if (!fichier) {
        printf("Erreur : Impossible de créer le fichier %s\n", nomFichier);
        return;
    }

    Formule* courant = listeFormules;
    while (courant != NULL) {
        fprintf(fichier, "%d;%s;%.2f\n", courant->id, courant->nom, courant->prix);
        courant = courant->next;
    }
    fclose(fichier);
    printf("Formules sauvegardées dans %s.\n", nomFichier);
}

// Chargement des formules depuis un fichier
Formule* chargerFormules(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
        return NULL;
    }

    Formule* tete = NULL;
    int id;
    char nom[50];
    float prix;

    while (fscanf(fichier, "%d;%49[^;];%f\n", &id, nom, &prix) == 3) {
        //printf("Chargement : ID = %d, Nom = %s, Prix = %.2f\n", id, nom, prix); // Débogage
        Formule* nouvelle = (Formule*)malloc(sizeof(Formule));
        if (!nouvelle) {
            printf("Erreur d'allocation mémoire.\n");
            fclose(fichier);
            return tete;
        }
        nouvelle->id = id;
        strcpy(nouvelle->nom, nom);
        nouvelle->prix = prix;
        nouvelle->next = tete;
        tete = nouvelle;
    }

    fclose(fichier);
    return tete;
}

// Sauvegarde des réservations dans un fichier
void sauvegarderReservations(Reservation* listeReservations, const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "w"); // Ouvre en mode écriture pour écraser le contenu précédent
    if (!fichier) {
        printf("Erreur : Impossible de créer le fichier %s\n", nomFichier);
        return;
    }

    Reservation* courant = listeReservations;
    while (courant != NULL) {
        if (courant->idClient > 0 && courant->idChambre > 0 && courant->total > 0) {
            fprintf(fichier, "%d;%d;%d;%d;", courant->idClient, courant->idChambre, courant->idFormule, courant->nbExtras);

            for (int i = 0; i < courant->nbExtras; i++) {
                fprintf(fichier, "%d", courant->idExtras[i]);
                if (i < courant->nbExtras - 1) {
                    fprintf(fichier, ",");
                }
            }

            fprintf(fichier, ";%d;%d;%.2f\n", courant->nbAdultes, courant->nbEnfants, courant->total);
        }
        courant = courant->next;
    }

    fclose(fichier);
    printf("Réservations sauvegardées dans %s.\n", nomFichier);
}

// Chargement des réservations depuis un fichier
Reservation* chargerReservations(const char* nomFichier) {
    if (!fichierExiste(nomFichier)) {
        printf("Avertissement : Fichier %s introuvable. Liste vide.\n", nomFichier);
        return NULL;
    }

    FILE* fichier = fopen(nomFichier, "r");
    Reservation* tete = NULL;
    int idClient, idChambre, idFormule, nbExtras, nbAdultes, nbEnfants;
    char extras[100];
    float total;

    while (fscanf(fichier, "%d;%d;%d;%d;%99[^;];%d;%d;%f\n", &idClient, &idChambre, &idFormule, &nbExtras, extras, &nbAdultes, &nbEnfants, &total) == 8) {
        int* idExtras = (int*)malloc(nbExtras * sizeof(int));
        char* token = strtok(extras, ",");
        int count = 0;

        while (token != NULL && count < nbExtras) {
            if (*token != '\0') {
                idExtras[count++] = atoi(token);
            }
            token = strtok(NULL, ",");
        }

        if (count != nbExtras) {
            printf("Avertissement : Correction du nombre d'extras pour le client %d.\n", idClient);
            nbExtras = count;
        }

        Reservation* nouvelle = (Reservation*)malloc(sizeof(Reservation));
        nouvelle->idClient = idClient;
        nouvelle->idChambre = idChambre;
        nouvelle->idFormule = idFormule;
        nouvelle->idExtras = idExtras;
        nouvelle->nbExtras = nbExtras;
        nouvelle->nbAdultes = nbAdultes;
        nouvelle->nbEnfants = nbEnfants;
        nouvelle->total = total;
        nouvelle->next = tete;
        tete = nouvelle;
    }

    fclose(fichier);
    return tete;
}

int compterUtilisationExtra(Reservation* reservations, int idExtra) {
    int compteur = 0;

    while (reservations != NULL) {
        for (int i = 0; i < reservations->nbExtras; i++) {
            if (reservations->idExtras[i] == idExtra) {
                compteur += reservations->nbAdultes + reservations->nbEnfants;
            }
        }
        reservations = reservations->next;
    }

    return compteur;
}

void afficherMenu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Accéder au mode Directeur\n");
    printf("2. Faire une réservation\n");
    printf("3. Consulter les informations\n");
    printf("4. Quitter\n");
    printf("Votre choix : ");
}
