#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/structures.h"
#include "headers/display.h"

// Modifier le prix d'une chambre
void modifierPrixChambre(Chambre* listeChambres, int idChambre, float nouveauPrix, const char* nomFichier) {
    Chambre* courant = listeChambres;
    int trouve = 0; // Flag pour vérifier si la chambre existe

    // Rechercher et modifier la chambre
    while (courant != NULL) {
        if (courant->id == idChambre) {
            courant->prix = nouveauPrix;
            printf("Prix de la chambre ID %d mis à jour à %.2f €.\n", idChambre, nouveauPrix);
            trouve = 1;
            break;
        }
        courant = courant->next;
    }

    if (!trouve) {
        printf("Erreur : Chambre avec ID %d introuvable.\n", idChambre);
        return;
    }

    // Réécrire toutes les chambres dans le fichier
    FILE* fichier = fopen(nomFichier, "w");
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s en écriture.\n", nomFichier);
        return;
    }

    courant = listeChambres; // Revenir au début de la liste
    while (courant != NULL) {
        fprintf(fichier, "%d;%s;%.2f;%d\n", courant->id, courant->type, courant->prix, courant->disponible);
        courant = courant->next;
    }

    fclose(fichier);
    printf("Le fichier %s a été mis à jour avec les nouveaux prix.\n", nomFichier);
}


// Ajouter une chambre
Chambre* ajouterChambre(Chambre* listeChambres, int id, const char* type, float prix, int disponible, const char* nomFichier) {
    // Allouer la mémoire pour la nouvelle chambre
    Chambre* nouvelle = (Chambre*)malloc(sizeof(Chambre));
    if (!nouvelle) {
        printf("Erreur d'allocation mémoire.\n");
        return listeChambres;
    }

    // Remplir les informations de la nouvelle chambre
    nouvelle->id = id;
    strcpy(nouvelle->type, type);
    nouvelle->prix = prix;
    nouvelle->disponible = disponible;
    nouvelle->next = listeChambres;

    // Afficher un message de confirmation
    printf("Chambre ajoutée : ID %d, Type : %s, Prix : %.2f €.\n", id, type, prix);

    // Ajouter la chambre au fichier
    FILE* fichier = fopen(nomFichier, "a"); // Ouvrir le fichier en mode "append"
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", nomFichier);
        return listeChambres;
    }

    // Écrire la nouvelle chambre dans le fichier
    fprintf(fichier, "\n%d;%s;%.2f;%d\n", id, type, prix, disponible);
    fclose(fichier);

    printf("Chambre ajoutée au fichier %s.\n", nomFichier);

    return nouvelle;
}


// Supprimer une chambre
Chambre* supprimerChambre(Chambre* listeChambres, int idChambre, const char* nomFichier) {
    Chambre* courant = listeChambres;
    Chambre* precedent = NULL;

    // Rechercher la chambre
    while (courant != NULL) {
        if (courant->id == idChambre) {
            // Demander une confirmation
            char confirmation;
            printf("Êtes-vous sûr de vouloir supprimer la chambre ID %d (%s) ? (o/n) : ", idChambre, courant->type);
            scanf(" %c", &confirmation);

            if (confirmation == 'o' || confirmation == 'O') {
                // Supprimer la chambre de la liste chaînée
                if (precedent == NULL) {
                    listeChambres = courant->next;
                } else {
                    precedent->next = courant->next;
                }

                free(courant);
                printf("Chambre ID %d supprimée avec succès.\n", idChambre);

                // Réécrire le fichier
                FILE* fichier = fopen(nomFichier, "w");
                if (!fichier) {
                    printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
                    return listeChambres;
                }

                // Réécrire toutes les chambres sauf la supprimée
                courant = listeChambres;
                while (courant != NULL) {
                    fprintf(fichier, "%d;%s;%.2f;%d\n", courant->id, courant->type, courant->prix, courant->disponible);
                    courant = courant->next;
                }
                fclose(fichier);
                printf("Le fichier %s a été mis à jour.\n", nomFichier);
            } else {
                printf("Suppression annulée.\n");
            }

            return listeChambres;
        }

        precedent = courant;
        courant = courant->next;
    }

    printf("Erreur : Chambre avec ID %d introuvable.\n", idChambre);
    return listeChambres;
}

Extra* chargerExtrasDepuisFichier(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
        return NULL;
    }

    Extra* listeExtras = NULL;
    Extra* dernierExtra = NULL;

    int id;
    char nom[50];
    float prix;
    int disponible;

    while (fscanf(fichier, "%d;%49[^;];%f;%d\n", &id, nom, &prix, &disponible) == 4) {
        Extra* nouvelExtra = (Extra*)malloc(sizeof(Extra));
        if (!nouvelExtra) {
            printf("Erreur d'allocation mémoire.\n");
            fclose(fichier);
            return listeExtras;
        }

        nouvelExtra->id = id;
        strcpy(nouvelExtra->nom, nom);
        nouvelExtra->prix = prix;
        nouvelExtra->disponible = disponible;
        nouvelExtra->next = NULL;  // Initialisation correcte

        if (!listeExtras) {
            // Premier élément de la liste
            listeExtras = nouvelExtra;
        } else {
            // Chaînage des éléments
            dernierExtra->next = nouvelExtra;
        }
        dernierExtra = nouvelExtra; // Mise à jour du dernier élément
    }

    fclose(fichier);
    return listeExtras;
}

// Ajouter un extra
Extra* ajouterExtra(Extra* listeExtras, int id, const char* nom, float prix, int disponible, const char* nomFichier) {
    Extra* nouvelExtra = (Extra*)malloc(sizeof(Extra));
    if (!nouvelExtra) {
        printf("Erreur d'allocation mémoire.\n");
        return listeExtras;
    }

    nouvelExtra->id = id;
    strcpy(nouvelExtra->nom, nom);
    nouvelExtra->prix = prix;
    nouvelExtra->disponible = disponible;
    nouvelExtra->next = listeExtras;

    printf("Extra ajouté : ID %d, Nom %s, Prix %.2f €, Disponible : %s\n",
           id, nom, prix, disponible ? "Oui" : "Non");

    FILE* fichier = fopen(nomFichier, "a");
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
        return listeExtras;
    }

    fprintf(fichier, "%d;%s;%.2f;%d\n", id, nom, prix, disponible);
    fclose(fichier);

    return nouvelExtra;
}

void afficherTousExtras(Extra* listeExtras) {
    if (!listeExtras) {
        printf("Aucun extra à afficher.\n");
        return;
    }

    printf("\n--- Tous les Extras ---\n");
    Extra* courant = listeExtras;
    while (courant != NULL) {  // Parcourt toute la liste
        printf("ID : %d, Nom : %s, Prix : %.2f €, Disponible : %s\n",
               courant->id, courant->nom, courant->prix,
               courant->disponible ? "Oui" : "Non");
        courant = courant->next;  // Passe à l'élément suivant
    }
    printf("------------------------\n");
}


void afficherExtrasDisponibles(Extra* listeExtras) {
    if (!listeExtras) {
        printf("Aucun extra disponible.\n");
        return;
    }

    printf("\n--- Extras Disponibles ---\n");
    Extra* courant = listeExtras;
    while (courant) {
        if (courant->disponible) {
            printf("ID : %d, Nom : %s, Prix : %.2f €\n", courant->id, courant->nom, courant->prix);
        }
        courant = courant->next;
    }
    printf("--------------------------\n");
}

void afficherExtrasNonDisponibles(Extra* listeExtras) {
    if (!listeExtras) {
        printf("Aucun extra non disponible.\n");
        return;
    }

    printf("\n--- Extras Non Disponibles ---\n");
    Extra* courant = listeExtras;
    while (courant) {
        if (!courant->disponible) {
            printf("ID : %d, Nom : %s, Prix : %.2f €\n", courant->id, courant->nom, courant->prix);
        }
        courant = courant->next;
    }
    printf("-------------------------------\n");
}

// Supprimer un extra
Extra* supprimerExtra(Extra* listeExtras, int idExtra, const char* nomFichier) {
    Extra* courant = listeExtras;
    Extra* precedent = NULL;

    // Rechercher l'extra
    while (courant != NULL) {
        if (courant->id == idExtra) {
            // Demander une confirmation
            char confirmation;
            printf("Êtes-vous sûr de vouloir supprimer l'extra ID %d (%s) ? (o/n) : ", idExtra, courant->nom);
            scanf(" %c", &confirmation);

            if (confirmation == 'o' || confirmation == 'O') {
                // Supprimer l'extra de la liste chaînée
                if (precedent == NULL) {
                    listeExtras = courant->next;
                } else {
                    precedent->next = courant->next;
                }

                free(courant);
                printf("Extra ID %d supprimé avec succès.\n", idExtra);

                // Réécrire le fichier
                FILE* fichier = fopen(nomFichier, "w");
                if (!fichier) {
                    printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
                    return listeExtras;
                }

                // Réécrire tous les extras sauf celui supprimé
                courant = listeExtras;
                while (courant != NULL) {
                    fprintf(fichier, "%d;%s;%.2f\n", courant->id, courant->nom, courant->prix);
                    courant = courant->next;
                }
                fclose(fichier);
                printf("Le fichier %s a été mis à jour.\n", nomFichier);
            } else {
                printf("Suppression annulée.\n");
            }

            return listeExtras;
        }

        precedent = courant;
        courant = courant->next;
    }

    printf("Erreur : Extra avec ID %d introuvable.\n", idExtra);
    return listeExtras;
}

void modifierExtra(Extra* listeExtras, int idExtra, float nouveauPrix, int nouvelleDisponibilite, const char* nomFichier) {
    Extra* courant = listeExtras;
    int trouve = 0;

    while (courant != NULL) {
        if (courant->id == idExtra) {
            courant->prix = nouveauPrix;
            courant->disponible = nouvelleDisponibilite;
            printf("Extra ID %d mis à jour : Prix = %.2f €, Disponible = %s\n",
                   idExtra, nouveauPrix, nouvelleDisponibilite ? "Oui" : "Non");
            trouve = 1;
            break;
        }
        courant = courant->next;
    }

    if (!trouve) {
        printf("Erreur : Extra avec ID %d introuvable.\n", idExtra);
        return;
    }

    FILE* fichier = fopen(nomFichier, "w");
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s en écriture.\n", nomFichier);
        return;
    }

    courant = listeExtras;
    while (courant != NULL) {
        fprintf(fichier, "%d;%s;%.2f;%d\n", courant->id, courant->nom, courant->prix, courant->disponible);
        courant = courant->next;
    }

    fclose(fichier);
}

// Calculer les revenus totaux
float calculerRevenusTotaux(Reservation* listeReservations) {
    float total = 0.0;
    Reservation* courant = listeReservations;

    while (courant != NULL) {
        total += courant->total;
        courant = courant->next;
    }

    return total;
}

// Afficher les chambres les plus populaires
void afficherChambresPopulaires(Reservation* reservations, Chambre* chambres) {
    Chambre* courantChambre = chambres;

    printf("\n--- Chambres les plus réservées ---\n");
    while (courantChambre != NULL) {
        int compteur = 0;
        Reservation* courantReservation = reservations;

        while (courantReservation != NULL) {
            if (courantReservation->idChambre == courantChambre->id) {
                compteur++;
            }
            courantReservation = courantReservation->next;
        }

        printf("Chambre ID %d (%s) : %d réservations.\n", courantChambre->id, courantChambre->type, compteur);
        courantChambre = courantChambre->next;
    }
}

#include <stdio.h>
#include "headers/structures.h"

// Afficher toutes les réservations
void afficherReservations(Reservation* listeReservations) {
    printf("\n--- Liste des réservations ---\n");

    // Parcourir la liste des réservations
    Reservation* courant = listeReservations;
    while (courant != NULL) {
        printf("Client ID: %d\n", courant->idClient);
        printf("Chambre ID: %d\n", courant->idChambre);
        printf("Formule ID: %d\n", courant->idFormule);
        printf("Nombre d'extras: %d\n", courant->nbExtras);
        printf("Extras: ");
        for (int i = 0; i < courant->nbExtras; i++) {
            printf("%d ", courant->idExtras[i]);
        }
        printf("\nNombre d'adultes: %d\n", courant->nbAdultes);
        printf("Nombre d'enfants: %d\n", courant->nbEnfants);
        printf("Total: %.2f €\n", courant->total);
        printf("-------------------------------\n");

        // Passer à la réservation suivante
        courant = courant->next;
    }

    if (listeReservations == NULL) {
        printf("Aucune réservation trouvée.\n");
    }

    // Retour au menu
    int retour;
    do {
        printf("Entrez 1 pour retourner au menu principal : ");
        scanf("%d", &retour);
    } while (retour != 1);
}

void afficherToutesFormules(Formule* listeFormules) {
    printf("\n--- Liste des Formules ---\n");
    Formule* courant = listeFormules;
    while (courant != NULL) {
        printf("ID : %d, Nom : %s, Prix : %.2f €\n", courant->id, courant->nom, courant->prix);
        courant = courant->next;
    }
    printf("--------------------------\n");
}

void modifierPrixFormule(Formule* listeFormules, int idFormule, float nouveauPrix, const char* nomFichier) {
    Formule* courant = listeFormules;
    int trouve = 0;

    // Rechercher et modifier la formule
    while (courant != NULL) {
        if (courant->id == idFormule) {
            courant->prix = nouveauPrix;
            printf("Prix de la formule ID %d mis à jour à %.2f €.\n", idFormule, nouveauPrix);
            trouve = 1;
            break;
        }
        courant = courant->next;
    }

    if (!trouve) {
        printf("Erreur : Formule avec ID %d introuvable.\n", idFormule);
        return;
    }

    // Réécrire toutes les formules dans le fichier
    FILE* fichier = fopen(nomFichier, "w");
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s en écriture.\n", nomFichier);
        return;
    }

    courant = listeFormules;
    while (courant != NULL) {
        fprintf(fichier, "%d;%s;%.2f\n", courant->id, courant->nom, courant->prix);
        courant = courant->next;
    }

    fclose(fichier);
    printf("Le fichier %s a été mis à jour avec les nouveaux prix.\n", nomFichier);
}

Formule* supprimerFormule(Formule* listeFormules, int idFormule, const char* nomFichier) {
    Formule* courant = listeFormules;
    Formule* precedent = NULL;

    // Rechercher la formule
    while (courant != NULL) {
        if (courant->id == idFormule) {
            // Demander une confirmation
            char confirmation;
            printf("Êtes-vous sûr de vouloir supprimer la formule ID %d (%s) ? (o/n) : ", idFormule, courant->nom);
            scanf(" %c", &confirmation);

            if (confirmation == 'o' || confirmation == 'O') {
                // Supprimer la formule de la liste chaînée
                if (precedent == NULL) {
                    listeFormules = courant->next;
                } else {
                    precedent->next = courant->next;
                }

                free(courant);
                printf("Formule ID %d supprimée avec succès.\n", idFormule);

                // Réécrire le fichier
                FILE* fichier = fopen(nomFichier, "w");
                if (!fichier) {
                    printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
                    return listeFormules;
                }

                courant = listeFormules;
                while (courant != NULL) {
                    fprintf(fichier, "%d;%s;%.2f\n", courant->id, courant->nom, courant->prix);
                    courant = courant->next;
                }
                fclose(fichier);
                printf("Le fichier %s a été mis à jour.\n", nomFichier);
            } else {
                printf("Suppression annulée.\n");
            }

            return listeFormules;
        }

        precedent = courant;
        courant = courant->next;
    }

    printf("Erreur : Formule avec ID %d introuvable.\n", idFormule);
    return listeFormules;
}

Formule* ajouterFormule(Formule* listeFormules, int id, const char* nom, float prix, const char* nomFichier) {
    Formule* nouvelle = (Formule*)malloc(sizeof(Formule));
    if (!nouvelle) {
        printf("Erreur d'allocation mémoire.\n");
        return listeFormules;
    }

    nouvelle->id = id;
    strcpy(nouvelle->nom, nom);
    nouvelle->prix = prix;
    nouvelle->next = listeFormules;

    printf("Formule ajoutée : ID %d, Nom : %s, Prix : %.2f €.\n", id, nom, prix);

    FILE* fichier = fopen(nomFichier, "a");
    if (!fichier) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", nomFichier);
        return listeFormules;
    }

    fprintf(fichier, "%d;%s;%.2f\n", id, nom, prix);
    fclose(fichier);

    printf("Formule ajoutée au fichier %s.\n", nomFichier);
    return nouvelle;
}

void sousMenuChambres(Chambre** chambres) {
    int choix;
    do {
        printf("\n--- Gestion des Chambres ---\n");
        printf("1. Afficher les chambres\n");
        printf("2. Modifier le prix d'une chambre\n");
        printf("3. Ajouter une chambre\n");
        printf("4. Supprimer une chambre\n");
        printf("5. Retourner au menu précédent\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                afficherChambresDisponibles(*chambres);
                afficherChambresNonDisponibles(*chambres);
                afficherToutesChambres(*chambres);
                break;
            }
            case 2: {
                int idChambre;
                float nouveauPrix;
                afficherToutesChambres(*chambres);
                printf("Entrez l'ID de la chambre : ");
                scanf("%d", &idChambre);
                printf("Entrez le nouveau prix : ");
                scanf("%f", &nouveauPrix);
                modifierPrixChambre(*chambres, idChambre, nouveauPrix, "../datas/chambres.txt");
                break;
            }
            case 3: {
                int id, disponible;
                char type[20];
                float prix;
                printf("Entrez l'ID de la chambre : ");
                scanf("%d", &id);
                printf("Entrez le type de la chambre : ");
                scanf(" %[^\n]", type);
                printf("Entrez le prix : ");
                scanf("%f", &prix);
                printf("Entrez la disponibilité (1=Oui, 0=Non) : ");
                scanf("%d", &disponible);
                *chambres = ajouterChambre(*chambres, id, type, prix, disponible, "../datas/chambres.txt");
                break;
            }
            case 4: {
                int idChambre;
                printf("Entrez l'ID de la chambre à supprimer : ");
                scanf("%d", &idChambre);
                *chambres = supprimerChambre(*chambres, idChambre, "../datas/chambres.txt");
                break;
            }
            case 5:
                printf("Retour au menu principal.\n");
            break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 5);
}

void sousMenuExtras(Extra** extras) {
    int choix;
    do {
        printf("\n--- Gestion des Extras ---\n");
        printf("1. Afficher les extras\n");
        printf("2. Modifier un extra\n");
        printf("3. Ajouter un extra\n");
        printf("4. Supprimer un extra\n");
        printf("5. Retourner au menu précédent\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherTousExtras(*extras);
                afficherExtrasDisponibles(*extras);
                afficherExtrasNonDisponibles(*extras);
                break;

            case 2: {
                afficherTousExtras(*extras);
                int idExtra, nouvelleDisponibilite;
                float nouveauPrix;
                printf("Entrez l'ID de l'extra à modifier : ");
                scanf("%d", &idExtra);
                printf("Entrez le nouveau prix : ");
                scanf("%f", &nouveauPrix);
                printf("Entrez la nouvelle disponibilité (1=Oui, 0=Non) : ");
                scanf("%d", &nouvelleDisponibilite);
                modifierExtra(*extras, idExtra, nouveauPrix, nouvelleDisponibilite, "extras.txt");
                break;
            }

            case 3: {
                afficherTousExtras(*extras);
                int id, disponible;
                char nom[50];
                float prix;
                printf("Entrez l'ID de l'extra : ");
                scanf("%d", &id);
                printf("Entrez le nom de l'extra : ");
                scanf(" %[^\n]", nom);
                printf("Entrez le prix de l'extra : ");
                scanf("%f", &prix);
                printf("Entrez la disponibilité (1=Oui, 0=Non) : ");
                scanf("%d", &disponible);
                *extras = ajouterExtra(*extras, id, nom, prix, disponible, "../datas/extras.txt");
                break;
            }

            case 4: {
                afficherTousExtras(*extras);
                int idExtra;
                printf("Entrez l'ID de l'extra à supprimer : ");
                scanf("%d", &idExtra);
                *extras = supprimerExtra(*extras, idExtra, "extras.txt");
                break;
            }

            case 5:
                printf("Retour au menu principal.\n");
                break;

            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 5);
}

void sousMenuFormules(Formule** formules) {
    int choix;
    do {
        printf("\n--- Gestion des Formules ---\n");
        printf("1. Modifier le prix d'une formule\n");
        printf("2. Ajouter une formule\n");
        printf("3. Supprimer une formule\n");
        printf("4. Retourner au menu précédent\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                afficherToutesFormules(*formules);
                int idFormule;
                float nouveauPrix;
                printf("Entrez l'ID de la formule à modifier : ");
                scanf("%d", &idFormule);
                printf("Entrez le nouveau prix : ");
                scanf("%f", &nouveauPrix);
                modifierPrixFormule(*formules, idFormule, nouveauPrix, "../datas/formules.txt");
                break;
            }
            case 2: {
                int id;
                char nom[50];
                float prix;
                printf("Entrez l'ID de la formule : ");
                scanf("%d", &id);
                printf("Entrez le nom de la formule : ");
                scanf(" %[^\n]", nom);
                printf("Entrez le prix de la formule : ");
                scanf("%f", &prix);
                *formules = ajouterFormule(*formules, id, nom, prix, "../datas/formules.txt");
                break;
            }
            case 3: {
                afficherToutesFormules(*formules);
                int idFormule;
                printf("Entrez l'ID de la formule à supprimer : ");
                scanf("%d", &idFormule);
                *formules = supprimerFormule(*formules, idFormule, "../datas/formules.txt");
                break;
            }
            case 4:
                printf("Retour au menu principal.\n");
            break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 4);
}


void gestionDirecteur(Chambre** chambres, Extra** extras, Formule** formules, Reservation* reservations) {
    int choix;
    do {
        printf("\n--- Menu Directeur ---\n");
        printf("1. Gérer les chambres\n");
        printf("2. Gérer les extras\n");
        printf("3. Gérer les formules\n");
        printf("4. Afficher les statistiques\n");
        printf("5. Consulter les réservations\n");
        printf("6. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                sousMenuChambres(chambres);
            break;
            case 2:
                sousMenuExtras(extras);
            break;
            case 3:
                sousMenuFormules(formules);
            break;
            case 4:
                printf("Revenus totaux : %.2f €\n", calculerRevenusTotaux(reservations));
            afficherChambresPopulaires(reservations, *chambres);
            break;
            case 5:
                afficherReservations(reservations);
            break;
            case 6:
                printf("Retour au menu principal.\n");
            break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 6);
}