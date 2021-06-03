#pragma once

typedef struct semaine_elt {
    unsigned int numero_semaine; // [1;53]
    unsigned int nombre_vaccins; // > 0
    struct semaine_elt* suivant;
}t_semaine_elt;

typedef struct ville_elt {
    char* nom_ville;
    unsigned int nombre_vaccins_total; // > 0
    t_semaine_elt* semaines_planifiees;
    struct ville_elt* suivant;
}t_ville_elt;

typedef struct vaccin_elt {
    char* marque;
    t_ville_elt* villes_dispo;
}t_vaccin_elt;

t_semaine_elt* creerSemaine(int num_semaine, int nb_vaccins);

t_ville_elt* creerVille(char* ville);

t_vaccin_elt* creerVaccin(char* marque);

t_semaine_elt* ajouterVaccinS(t_semaine_elt* liste_semaines, int semaine, int nb_vaccins);

t_semaine_elt* deduireVaccinS(t_semaine_elt* liste_semaines, int semaine, int nb_vaccins);

t_ville_elt* ajouterVaccinV(t_ville_elt* liste_villes, char* ville, int semaine, int nb_vaccins);

t_ville_elt* deduireVaccinV(t_ville_elt* liste_villes, char* ville, int semaine, int nb_vaccins);

void afficherStock(t_vaccin_elt* tableau_vaccins);

void afficherPlanification(t_vaccin_elt* vaccin, int semaine);

t_vaccin_elt* fusionnerStocks(t_vaccin_elt* vaccinA, t_vaccin_elt* vaccinB);

void viderBuffer();

t_vaccin_elt* parcourirVaccins(t_vaccin_elt* vaccin[10], int vaccins_existants, char* nom_vaccin);

t_semaine_elt* supprimerSemaine(t_semaine_elt* liste_semaines, int semaine);

t_ville_elt* supprimerVille(t_ville_elt* liste_villes, char* ville);

t_ville_elt* deplacerVille(t_ville_elt* liste_villes, t_ville_elt* ville);

t_ville_elt* trierVilles(t_ville_elt* villes);

int calculTotalVaccinsVille(t_semaine_elt* semaines_planifiees);