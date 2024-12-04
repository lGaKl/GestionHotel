#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Chambre {
    int id;
    char type[20];
    float prix;
    int disponible;
    struct Chambre* next;
}Chambre;

typedef struct Formule {
    int id;
    char nom[50];
    float prix;
    struct Formule* next;
} Formule;

typedef struct extra {
    int id;
    char nom[50];
    float prix;
    int disponible;
    struct extra* next;
}Extra;

typedef struct Reservation {
    int idClient;
    int idChambre;
    int idFormule;
    int* idExtras;
    int nbExtras;
    int nbAdultes;
    int nbEnfants;
    float total;
    struct Reservation* next;
}Reservation;

#endif //STRUCTURES_H
