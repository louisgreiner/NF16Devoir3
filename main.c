#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp3.h"

int main()
{

    t_vaccin_elt* vaccin[10] = { NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
    int vaccins_existants = 0; // on sera toujours decale de 1 entre le nombre reel de vaccin, et le vaccin[i] correspondant (comme on commence  0)

    int semaine, nb_vaccins;
    t_vaccin_elt* temp_vaccin;
    int choix;

    char* nom_vaccin1 = malloc(sizeof(*nom_vaccin1 * 256));
    char* nom_vaccin2 = malloc(sizeof(*nom_vaccin2 * 256)); // a utiliser pour la fusion des vaccins
    char* nom_ville = malloc(sizeof(*nom_ville * 256));

    do {
        printf("\n\n======================================");
        printf("\n1. Initialiser la liste des marques de vaccin disponibles");
        printf("\n2. Ajouter et planifier un stock");
        printf("\n3. Retirer un nombre de vaccins du stock");
        printf("\n4. Afficher le stock d'un vaccin");
        printf("\n5. Afficher la planification pour une semaine");
        printf("\n6. Fusionner deux stocks");
        printf("\n7. Quitter");
        printf("\n======================================");
        printf("\n   Votre choix ? \n");


        choix = getchar();
        viderBuffer();

        switch (choix) {
        case '1':
            if (vaccins_existants == 10) { // liste pleine
                printf("\nLa liste de vaccins est pleine, on ne peut pas en rajouter plus.\n");
            }
            else {
                printf("\nNom du vaccin a creer ?");
                scanf("%s", nom_vaccin1);
                if (vaccins_existants != 0 && parcourirVaccins(vaccin, vaccins_existants, nom_vaccin1) != NULL) {
                    printf("Le vaccin %s existe deja.", nom_vaccin1);
                    break;
                }
                vaccin[vaccins_existants] = creerVaccin(nom_vaccin1);
                printf("\nLe vaccin %s a bien ete cree", vaccin[vaccins_existants]->marque);
                vaccins_existants++;
                printf("\nListe des vaccins existants : ");
                for (int j = 0; j < vaccins_existants; j++) {
                    printf("\n\t - %s", vaccin[j]->marque);
                }
            }
            break;

        case '2':
            printf("\nDe quel vaccin voulez-vous augmenter les stocks?");
            scanf("%s", nom_vaccin1);
            temp_vaccin = parcourirVaccins(vaccin, vaccins_existants, nom_vaccin1);
            if (temp_vaccin != NULL) {
                printf("\nNom de la ville?");
                scanf("%s", nom_ville);
                printf("\nNumero de semaine?");
                scanf("%d", &semaine);
                if(semaine>0){
                    printf("\nNombre de vaccins a ajouter au stock?");
                    scanf("%d", &nb_vaccins);
                    if(nb_vaccins>0){
                        temp_vaccin->villes_dispo = ajouterVaccinV(temp_vaccin->villes_dispo, nom_ville, semaine, nb_vaccins);
                    }else{
                        printf("\nErreur : le nombre de vaccins a ajouter doit etre strictement positif");
                    }
                }else{
                    printf("\nErreur : le numero de semaine doit etre strictement positif");
                }
                
            } // else -> le vaccin n'existe pas
            break;

        case '3':
            printf("\nDe quel vaccin voulez-vous reduire les stocks?");
            scanf("%s", nom_vaccin1);
            temp_vaccin = parcourirVaccins(vaccin, vaccins_existants, nom_vaccin1);
            if (temp_vaccin != NULL) {
                printf("\nNom de la ville?");
                scanf("%s", nom_ville);
                printf("\nNumero de semaine?");
                scanf("%d", &semaine);
                if(semaine>0){
                    printf("\nNombre de vaccins a deduire du stock?");
                    scanf("%d", &nb_vaccins);
                    if(nb_vaccins>0){
                        temp_vaccin->villes_dispo = deduireVaccinV(temp_vaccin->villes_dispo, nom_ville, semaine, nb_vaccins);
                    }else{
                        printf("\nErreur : le nombre de vaccins a ajouter doit etre strictement positif");
                    }
                }else{
                    printf("\nErreur : le numero de semaine doit etre strictement superieur a 0\n");
                }
            } // else -> le vaccin n'existe pas
            break;

        case '4':
            printf("\nMarque du vaccin a afficher le stock?");
            scanf("%s", nom_vaccin1);
            temp_vaccin = parcourirVaccins(vaccin, vaccins_existants, nom_vaccin1);
            if (temp_vaccin != NULL) {
                afficherStock(temp_vaccin);
            } // else -> le vaccin n'existe pas
            break;

        case '5':
            printf("\nMarque du vaccin a afficher la planification?");
            scanf("%s", nom_vaccin1);
            printf("\nPlanification de quelle semaine ?");
            scanf("%d", &semaine);
            temp_vaccin = parcourirVaccins(vaccin, vaccins_existants, nom_vaccin1);
            if (temp_vaccin != NULL) {
                afficherPlanification(temp_vaccin, semaine);
            } // else -> le vaccin n'existe pas
            break;

        case '6':
            if (vaccins_existants == 10) { // liste pleine
                printf("\nLa liste de vaccins est pleine, on ne peut pas en rajouter plus.\n");
            }
            else {
                printf("\nMarque du premier vaccin a fusionner? ");
                scanf("%s", nom_vaccin1);
                temp_vaccin = parcourirVaccins(vaccin, vaccins_existants, nom_vaccin1);
                if (temp_vaccin != NULL) {
                    printf("\nMarque du deuxieme vaccin a fusionner? ");
                    scanf("%s", nom_vaccin2);
                    t_vaccin_elt* temp_vaccin_2 = parcourirVaccins(vaccin, vaccins_existants, nom_vaccin2);
                    if (temp_vaccin_2 != NULL) {
                        t_vaccin_elt* nv_vaccin = fusionnerStocks(temp_vaccin, temp_vaccin_2);
                        vaccin[vaccins_existants] = nv_vaccin;
                        vaccins_existants++;
                        printf("Vaccin cree avec succes!");
                        afficherStock(nv_vaccin);
                    } // else -> le vaccin 2 n'existe pas
                } // else -> le vaccin 1 n'existe pas
            }
            break;

        case '7':
            printf("\n         ======== PROGRAMME TERMINE ========\n");
            printf("\n======== Appuyez sur une touche pour quitter ========\n");
            for (int i = 0; i < vaccins_existants; i++) {
                while (vaccin[i]->villes_dispo != NULL) {
                    vaccin[i]->villes_dispo = supprimerVille(vaccin[i]->villes_dispo, vaccin[i]->villes_dispo->nom_ville);
                }
                free(vaccin[i]->marque);
            }
            break;

        default:
            printf("\n\nERREUR : votre choix n'est pas valide ! ");
        }

        printf("\n\n");
        viderBuffer();

    } while (choix != '7');

    free(nom_vaccin1);
    free(nom_vaccin2);
    free(nom_ville);

    return 0;
}
