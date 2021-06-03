#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp3.h"

/*
1)
Cree une nouvelle semaine de numero et au nombre de vaccins passes en parametres
Retourne la nouvelle semaine
*/
t_semaine_elt* creerSemaine(int num_semaine, int nb_vaccins)
{
    t_semaine_elt* nouvelleSemaine = malloc(sizeof(struct semaine_elt));
    if (nouvelleSemaine == NULL) { exit(EXIT_FAILURE); }
    nouvelleSemaine->numero_semaine = num_semaine;
    nouvelleSemaine->nombre_vaccins = nb_vaccins;
    nouvelleSemaine->suivant = NULL;
    return(nouvelleSemaine);
}

/*
2)
Cree une nouvelle ville au nom passe en parametre
Retourne la nouvelle ville
*/
t_ville_elt* creerVille(char* ville)
{
    t_ville_elt* nouvelleVille = malloc(sizeof(struct ville_elt));
    if (nouvelleVille == NULL) { exit(EXIT_FAILURE); }
    nouvelleVille->nom_ville = malloc(strlen(ville) + 1);
    strcpy(nouvelleVille->nom_ville, ville);
    nouvelleVille->nombre_vaccins_total = 0;
    nouvelleVille->semaines_planifiees = NULL;
    nouvelleVille->suivant = NULL;
    return(nouvelleVille);
}

/*
3)
Cree un nouveau vaccin de la marque passee en parametre
Retourne le nouveau vaccin
*/
t_vaccin_elt* creerVaccin(char* marque)
{
    t_vaccin_elt* nouveauVaccin = malloc(sizeof(struct vaccin_elt));
    if (nouveauVaccin == NULL) { exit(EXIT_FAILURE); }
    nouveauVaccin->marque = malloc(strlen(marque) + 1);
    strcpy(nouveauVaccin->marque, marque);
    nouveauVaccin->villes_dispo = NULL;
    return(nouveauVaccin);
}

/*
4)
Ajout d'un nombre de vaccins a une semaine
Si la semaine n'existe pas, elle sera creee
Fonction recursive
Retourne le maillon de la liste passee en parametres : le retour du premier appel sera donc le premier element de la liste passee en parametre au premier appel
*/

t_semaine_elt* ajouterVaccinS(t_semaine_elt* liste_semaines, int semaine, int nb_vaccins)
{
    if (liste_semaines == NULL) {                                       //Liste vide => premiere insertion
        liste_semaines = creerSemaine(semaine, nb_vaccins);
    }
    else if (liste_semaines->numero_semaine == semaine) {               //Semaine existante => ajout du nombre de vaccins a la semaine
        liste_semaines->nombre_vaccins += nb_vaccins;
    }
    else if (semaine < liste_semaines->numero_semaine) {                //Insertion en tete de liste, si liste non vide
        t_semaine_elt* sem = creerSemaine(semaine, nb_vaccins);
        sem->suivant = liste_semaines;
        liste_semaines = sem;
    }
    else {
        t_semaine_elt* sem = creerSemaine(semaine, nb_vaccins);
        if (liste_semaines->suivant != NULL) {
            if (semaine < liste_semaines->suivant->numero_semaine) {    //Insertion d'une nouvelle semaine entre deux semaines existantes
                sem->suivant = liste_semaines->suivant;
                liste_semaines->suivant = sem;
            }
            else {                                                      //Passage a l'element suivant pour l'insertion / ajout
                liste_semaines->suivant = ajouterVaccinS(liste_semaines->suivant, semaine, nb_vaccins);
            }
        }
        else {                                                          //Ajout en queue de liste
            liste_semaines->suivant = sem;
        }
    }
    return(liste_semaines);
}

/*
5)
Retrait d'un nombre de vaccins a la semaine de numero donne, dans la liste de semaines passee en parametres
Retourne le premier element de la liste passee en parametre
*/
t_semaine_elt* deduireVaccinS(t_semaine_elt* liste_semaines, int semaine, int nb_vaccins)
{
    t_semaine_elt* temp_semaine = liste_semaines;
    //Recherche de la semaine
    while (temp_semaine != NULL && temp_semaine->numero_semaine != semaine && temp_semaine->numero_semaine < semaine) {
        temp_semaine = temp_semaine->suivant;
    }
    //Queue de la liste atteint sans trouver la semaine recherchee => celle-ci n'existe pas
    if (temp_semaine == NULL) {
        printf("\n\nSuppression impossible : semaine inexistante.");
    }
    else {
        if (temp_semaine->numero_semaine == semaine) {
            if (temp_semaine->nombre_vaccins < nb_vaccins) {    //On retire plus de vaccins que possible => gestion du type unsigned int
                printf("\n\nSuppression impossible : le nombre de vaccins a deduire doit etre inferieur ou egal a : %d vaccins disponibles", temp_semaine->nombre_vaccins);
            }
            else {                                              //Nombre de vaccin a deduire correct
                temp_semaine->nombre_vaccins = temp_semaine->nombre_vaccins - nb_vaccins;
                if (temp_semaine->nombre_vaccins == 0) {        //La semaine ne comporte plus aucun vaccin => on peut la supprimer
                    liste_semaines = supprimerSemaine(liste_semaines, semaine);
                }
            }
        }
    }
    return(liste_semaines);
}

/*
6)
Ajoute un nombre de vaccins a la semaine donnee, de la ville donnee de la liste de villes passee en parametres
Retourne le premier element de la liste passee en parametre
*/
t_ville_elt* ajouterVaccinV(t_ville_elt* liste_villes, char* ville, int semaine, int nb_vaccins)
{
    if (liste_villes == NULL) {         //Liste vide => insertion en tete
        printf("\n\nAucune ville presente dans la liste... on va creer la liste et ajouter %s dans la liste.", ville);
        liste_villes = creerVille(ville);
        liste_villes->nombre_vaccins_total += nb_vaccins;
        liste_villes->semaines_planifiees = ajouterVaccinS(liste_villes->semaines_planifiees, semaine, nb_vaccins);
    }
    else {
        t_ville_elt* temp_villes = liste_villes;
        t_ville_elt* elmt_prec = NULL;
        int found_ville = 0;

        if (strcmp(temp_villes->nom_ville, ville) == 0) {               // Test du premier element
            found_ville = 1;
        }

        //Recherche de la ville dans la liste
        while (temp_villes->suivant != NULL && found_ville == 0) {
            elmt_prec = temp_villes;
            temp_villes = temp_villes->suivant;

            //Ville trouvee
            if (strcmp(temp_villes->nom_ville, ville) == 0) {
                found_ville = 1;
            }
        }

        if (found_ville) {              //Ville trouvee => ajout du nombre de vaccins a la semaine existante
            temp_villes->nombre_vaccins_total += nb_vaccins;
            temp_villes->semaines_planifiees = ajouterVaccinS(temp_villes->semaines_planifiees, semaine, nb_vaccins);

            //Tri de la liste suivant le nouveau total de vaccins
            t_ville_elt* elmt_parcouru = temp_villes;
            if (elmt_parcouru->suivant != NULL && temp_villes->nombre_vaccins_total >= elmt_parcouru->suivant->nombre_vaccins_total) {
                while (elmt_parcouru->suivant != NULL && elmt_parcouru->suivant->nombre_vaccins_total < temp_villes->nombre_vaccins_total) {
                    elmt_parcouru = elmt_parcouru->suivant;
                }

                t_ville_elt* mem_temp_villes = temp_villes;             //mem_temp_villes = la ville qu'on deplace

                //Retrait de l'element a deplacer de son emplacement d'origine
                if (elmt_prec == NULL) {        //Si l'element deplace etait en tete de liste, on modifie cette derniere
                    liste_villes = liste_villes->suivant;
                    temp_villes = temp_villes->suivant;
                }
                else {                          //Si l'element etait en milieu ou queue de liste, on raccorde son precedent et son suivant
                    elmt_prec->suivant = elmt_prec->suivant->suivant;
                }

                mem_temp_villes->suivant = elmt_parcouru->suivant;      //on raccroche la suite de la liste a l'element deplace
                elmt_parcouru->suivant = mem_temp_villes;               //on raccroche la ville qu'on deplace
            }

        }
        else {              //Ville inexistante : on la cree puis on l'insere
            temp_villes = liste_villes;
            t_ville_elt* nouvelle_ville = creerVille(ville);
            nouvelle_ville->semaines_planifiees = ajouterVaccinS(nouvelle_ville->semaines_planifiees, semaine, nb_vaccins);
            nouvelle_ville->nombre_vaccins_total += nb_vaccins;

            if (temp_villes->suivant == NULL) {                         //Liste contenant une unique ville
                if (temp_villes->nombre_vaccins_total > nb_vaccins) {       //Insertion en tete de liste
                    nouvelle_ville->suivant = temp_villes;
                    liste_villes = nouvelle_ville;
                    temp_villes = liste_villes;
                }
                else {                                                      //Insertion en queue de liste
                    temp_villes->suivant = nouvelle_ville;
                }
            }
            else{                                                        //Il y a au moins 2 villes differentes dans la liste
                if (temp_villes->nombre_vaccins_total > nb_vaccins) {       //Insertion en tete de liste
                    nouvelle_ville->suivant = temp_villes;
                    liste_villes = nouvelle_ville;
                    temp_villes = liste_villes;
                }
                else {
                    while (temp_villes->nombre_vaccins_total <= nb_vaccins) {
                        if (temp_villes->suivant == NULL) {                 //Insertion en queue de liste
                            temp_villes->suivant = nouvelle_ville;
                            break;
                        }
                        if (temp_villes->nombre_vaccins_total <= nb_vaccins
                            && temp_villes->suivant->nombre_vaccins_total >= nb_vaccins) { //Insertion entre deux elements de liste
                            nouvelle_ville->suivant = temp_villes->suivant;
                            temp_villes->suivant = nouvelle_ville;
                            temp_villes = temp_villes->suivant;     //on passe a la ville suivante pour l'ajout
                            break;
                        }
                        temp_villes = temp_villes->suivant;
                    }
                }
            }
        }

    }
    return(liste_villes);
}

/*
7)
Deduit un nombre de vaccins a la semaine donnee, de la ville donnee de la liste de villes passee en parametres
Si le total de vaccins de la ville atteint 0, celle-ci est supprimee de la liste
Retourne la tete de liste pass�e
*/
t_ville_elt* deduireVaccinV(t_ville_elt* liste_villes, char* ville, int semaine, int nb_vaccins)
{
    //Liste de villes vide
    if (liste_villes == NULL) {
        printf("\n\nOperation impossible : ville inexistante.");
    }
    else {
        t_ville_elt* temp_villes = liste_villes;
        t_ville_elt* precedent = NULL;
        int found_ville = 0;

        //Test du premier element de la liste
        if (strcmp(temp_villes->nom_ville, ville) == 0) { // pour tester le 1er element ou si la liste a un seul element
            found_ville = 1;
        }

        //Parcours de la liste de ville a la recherche de la ville donnee
        while (temp_villes->suivant != NULL && found_ville == 0) {
            precedent = temp_villes;
            temp_villes = temp_villes->suivant;
            if (strcmp(temp_villes->nom_ville, ville) == 0) {
                found_ville = 1;
            }
        }

        //Ville trouvee dans la liste
        if (found_ville) {
            if (temp_villes->nombre_vaccins_total < nb_vaccins) {
                printf("\n\nOperation impossible : le nombre de vaccins a deduire doit etre inferieur ou egal a : %d vaccins disponibles total", temp_villes->nombre_vaccins_total);
            }
            else {
                temp_villes->semaines_planifiees = deduireVaccinS(temp_villes->semaines_planifiees, semaine, nb_vaccins);
                temp_villes->nombre_vaccins_total = calculTotalVaccinsVille(temp_villes->semaines_planifiees);
                if (temp_villes->nombre_vaccins_total == 0) { // on doit supprimer la ville
                    liste_villes = supprimerVille(liste_villes, temp_villes->nom_ville);
                }else if (precedent != NULL) {  //Tri de la ville
                    precedent->suivant = temp_villes->suivant;
                    liste_villes = deplacerVille(liste_villes, temp_villes);
                }

            }
        }
        else {      //Ville inexistante
            printf("\n\nAucun vaccin de la marque recherchee prevu a %s", ville);
        }
    }
    //return(trierVilles(liste_villes)); // autre facon de renvoyer une liste triee
    return(liste_villes);
}

/*
8)
Affiche le stock du vaccin donne pour chaque semaine de chaque ville ou celui-ci est disponible
*/
void afficherStock(t_vaccin_elt* vaccin)
{
    t_ville_elt* temp_ville = vaccin->villes_dispo;
    while (temp_ville != NULL) {
        t_semaine_elt* temp_semaine = temp_ville->semaines_planifiees;
        printf("\n  --- %s [Total = %d]", temp_ville->nom_ville, temp_ville->nombre_vaccins_total);
        while (temp_semaine != NULL) {
            printf("\n\t  --- semaine %d : %d", temp_semaine->numero_semaine, temp_semaine->nombre_vaccins);
            temp_semaine = temp_semaine->suivant;
        }
        temp_ville = temp_ville->suivant;
    }
    printf("\n");
}

/*
9)
Affiche le stock du vaccin donne pour la semaine donnee de chaque ville ou celui-ci est disponible
*/
void afficherPlanification(t_vaccin_elt* vaccin, int semaine)
{
    t_ville_elt* temp_ville = vaccin->villes_dispo;
    if (vaccin == NULL) {
        printf("\nLe vaccin n'existe pas.\n");
    }
    else {
        printf("\n %s :", vaccin->marque);
        while (temp_ville != NULL) {
            t_semaine_elt* temp_semaine = temp_ville->semaines_planifiees;
            //On arrete le parcours si le numero de semaine parcourue est superieur a celui voulu
            while (temp_semaine != NULL && temp_semaine->numero_semaine<=semaine) {
                if (temp_semaine->numero_semaine == semaine) {
                    printf("\n\t --- %s : %d", temp_ville->nom_ville, temp_semaine->nombre_vaccins);
                }
                temp_semaine = temp_semaine->suivant;
            }
            temp_ville = temp_ville->suivant;
        }
    }
    printf("\n");
}


/*
10)
Cree un nouveau vaccin rassemblant le stock des deux vaccins passes en parametres
Fusionne les villes, ainsi que les semaines planifiees des deux vaccins
Retourne le "vaccin" ainsi compose
*/
t_vaccin_elt* fusionnerStocks(t_vaccin_elt* vaccinA, t_vaccin_elt* vaccinB)
{
    //Creation du nouveau vaccin
    char* marque = malloc(sizeof(char) * 256);
    strcpy(marque, vaccinA->marque);
    strcat(marque, "_");
    strcat(marque, vaccinB->marque);
    printf("\nOn cree le vaccin : %s\n", marque);
    t_vaccin_elt* nv_vaccin = creerVaccin(marque);
    free(marque);

    t_ville_elt* liste_villes_A = vaccinA->villes_dispo;
    t_ville_elt* liste_villes_B = vaccinB->villes_dispo;

    //Insertion de la liste de villes du vaccin A :
    //Initialisation de la liste de villes avec le premier element de la liste de villes du vaccin A (par copie)
    t_ville_elt* nvlle_liste_villes_parcours = creerVille(liste_villes_A->nom_ville);
    nv_vaccin->villes_dispo = nvlle_liste_villes_parcours;
    nvlle_liste_villes_parcours->nombre_vaccins_total = liste_villes_A->nombre_vaccins_total;

    //Copie des semaines planifiees
    if (liste_villes_A->semaines_planifiees != NULL) {
        t_semaine_elt* semaine_a_parcourue = liste_villes_A->semaines_planifiees;
        t_semaine_elt* semaine_parcourue = creerSemaine(semaine_a_parcourue->numero_semaine, semaine_a_parcourue->nombre_vaccins);
        nvlle_liste_villes_parcours->semaines_planifiees = semaine_parcourue;
        semaine_a_parcourue = semaine_a_parcourue->suivant;
        while (semaine_a_parcourue != NULL) {
            semaine_parcourue->suivant = creerSemaine(semaine_a_parcourue->numero_semaine, semaine_a_parcourue->nombre_vaccins);
            semaine_parcourue = semaine_parcourue->suivant;
            semaine_a_parcourue = semaine_a_parcourue->suivant;
        }
    }

    liste_villes_A = liste_villes_A->suivant;
    while (liste_villes_A != NULL) {
        nvlle_liste_villes_parcours->suivant = creerVille(liste_villes_A->nom_ville);
        nvlle_liste_villes_parcours = nvlle_liste_villes_parcours->suivant;
        nvlle_liste_villes_parcours->nombre_vaccins_total = liste_villes_A->nombre_vaccins_total;
        //Copie des semaines planifiees
        if (liste_villes_A->semaines_planifiees != NULL) {
//            printf("\nLa ville %s a au moins une semaine planifiee\n", liste_villes_A->nom_ville);
            t_semaine_elt* semaine_a_parcourue = liste_villes_A->semaines_planifiees;
            t_semaine_elt* semaine_parcourue = creerSemaine(semaine_a_parcourue->numero_semaine, semaine_a_parcourue->nombre_vaccins);
            nvlle_liste_villes_parcours->semaines_planifiees = semaine_parcourue;
            semaine_a_parcourue = semaine_a_parcourue->suivant;
            while (semaine_a_parcourue != NULL) {
                semaine_parcourue->suivant = creerSemaine(semaine_a_parcourue->numero_semaine, semaine_a_parcourue->nombre_vaccins);
                semaine_parcourue = semaine_parcourue->suivant;
                semaine_a_parcourue = semaine_a_parcourue->suivant;
            }
        }
        liste_villes_A = liste_villes_A->suivant;
    }

    //Cas ou le premier element de la liste de villes du vaccin B doit aller en tete de la nouvelle liste
    if (nv_vaccin == NULL) {
        //Liste vide apres insertion de A: on insere simplement les villes du vaccin B
        while (liste_villes_B != NULL) {
            //inserer copie dans nv_vaccin
            liste_villes_B = liste_villes_B->suivant;
        }
    }
    else {
        //Insertion de la premiere ville du vaccin B en tete de liste
        if ((liste_villes_B != NULL && liste_villes_B->nombre_vaccins_total < nv_vaccin->villes_dispo->nombre_vaccins_total) || (nv_vaccin == NULL && liste_villes_B != NULL)) {
            t_ville_elt* nvlle_ville = creerVille(liste_villes_B->nom_ville);
            nvlle_ville->suivant = nv_vaccin->villes_dispo;
            nv_vaccin->villes_dispo = nvlle_ville;
            liste_villes_B = liste_villes_B->suivant;
        }
        //On insere les elements de la liste du vaccin B de facon triee
        while (liste_villes_B != NULL) {
            t_ville_elt* nvlle_liste_villes = nv_vaccin->villes_dispo;
            t_ville_elt* elmt_avant_insertion = NULL;        //on inserera forcement au moins a la suite du premier element de la liste grace au test effectue precedemment
            t_ville_elt* elmt_precedent = NULL;
            int insere = 0;
            while (nvlle_liste_villes != NULL) {
                if (strcmp(liste_villes_B->nom_ville, nvlle_liste_villes->nom_ville) == 0) {    //fusion des deux villes
                    nvlle_liste_villes->nombre_vaccins_total += liste_villes_B->nombre_vaccins_total;
                    //On ajoute les semaines de la ville parcourue du vaccin B a celles de la meme ville dans la nouvelle liste
                    t_semaine_elt* semaine_parcourue = liste_villes_B->semaines_planifiees;
                    while (semaine_parcourue != NULL) {
                        nvlle_liste_villes->semaines_planifiees = ajouterVaccinS(nvlle_liste_villes->semaines_planifiees, semaine_parcourue->numero_semaine, semaine_parcourue->nombre_vaccins);
                        semaine_parcourue = semaine_parcourue->suivant;
                    }
                    //On garde en memoire que l'insertion a bien ete faite
                    insere = 1;

                    //Tri de la liste suivant le nouveau total de vaccins
                    t_ville_elt* elmt_parcouru = nvlle_liste_villes;
                    if (elmt_parcouru->suivant != NULL && nvlle_liste_villes->nombre_vaccins_total >= elmt_parcouru->suivant->nombre_vaccins_total) {
//                        printf("Il faut trier %s\n", nvlle_liste_villes->nom_ville);
                        while (elmt_parcouru->suivant != NULL && elmt_parcouru->suivant->nombre_vaccins_total < nvlle_liste_villes->nombre_vaccins_total) {
//                            printf("On passe devant %s\n", elmt_parcouru->nom_ville);
                            elmt_parcouru = elmt_parcouru->suivant;
                        }

                        t_ville_elt* mem_temp_villes = nvlle_liste_villes;             //mem_temp_villes = la ville qu'on deplace

                        //Retrait de l'element a deplacer de son emplacement d'origine
                        if (elmt_precedent == NULL) {        //Si l'element deplace etait en tete de liste, on modifie cette derniere
                            nv_vaccin->villes_dispo = nv_vaccin->villes_dispo->suivant;
                        }
                        else {                          //Si l'elemente etait en milieu ou queue de liste, on raccorde son precedent et son suivant
                            elmt_precedent->suivant = elmt_precedent->suivant->suivant;
                        }

                        mem_temp_villes->suivant = elmt_parcouru->suivant;      //on raccroche la suite de la liste a l'element deplace
                        elmt_parcouru->suivant = nvlle_liste_villes;               //on raccroche la ville qu'on deplace
                    }

                }
                else {
                    //Si possibilite d'insertion (en cas de non fusion)
                    //Sauvegarder ici l'element a la suite duquel on peut inserer => en fin de parcours de temp_A, si pas insere, on inserera a la suite de l'elmt sauvegarde
                    if (nvlle_liste_villes->nombre_vaccins_total < liste_villes_B->nombre_vaccins_total) {
                        elmt_avant_insertion = nvlle_liste_villes;
                    }
                }
                elmt_precedent = nvlle_liste_villes;
                nvlle_liste_villes = nvlle_liste_villes->suivant;
            }
            if (insere == 0) {      //La ville n'etait pas presente dans la liste A : on doit donc l'inserer a l'emplacement marque
                //On cree la ville et on l'insere a la suite de elmt_prec
                t_ville_elt* nvlle_ville = creerVille(liste_villes_B->nom_ville);
                nvlle_ville->nombre_vaccins_total = liste_villes_B->nombre_vaccins_total;
                if (elmt_avant_insertion != NULL) {
                    nvlle_ville->suivant = elmt_avant_insertion->suivant;
                    elmt_avant_insertion->suivant = nvlle_ville;
                }
                //On recopie aussi les semaines planifiees de la ville recopiee
                t_semaine_elt* semaine_parcourue = liste_villes_B->semaines_planifiees;
                while (semaine_parcourue != NULL) {
                    nvlle_ville->semaines_planifiees = ajouterVaccinS(nvlle_ville->semaines_planifiees, semaine_parcourue->numero_semaine, semaine_parcourue->nombre_vaccins);
                    semaine_parcourue = semaine_parcourue->suivant;
                }
            }
            liste_villes_B = liste_villes_B->suivant;
        }
    }
    // autre facon de trier : Tri complet (moins optimise)
    //nv_vaccin->villes_dispo = trierVilles(nv_vaccin->villes_dispo);
    return nv_vaccin;
}

/*
Permet de vider le buffer de l'entree standard, afin d'eviter des problomes a la saisie
*/
void viderBuffer()
{
    char c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/*
Recherche le vaccin du nom donne dans la liste de vaccins passee
Le parametre vaccins_existants permet de connaitre le nombre d'elements a parcourir
Retourne l'element vaccin trouve, ou NULL s'il n'est pas trouve
*/
t_vaccin_elt* parcourirVaccins(t_vaccin_elt* vaccin[10], int vaccins_existants, char* nom_vaccin)
{
    int i = 0;
    if (vaccin != NULL && vaccins_existants>0) {
        while (i < vaccins_existants - 1 && strcmp(nom_vaccin, vaccin[i]->marque) != 0) { // on cherche l'emplacement du vaccin qui nous interesse, ou soit on va jusqu'au bout de la liste
            i++;
        }
        if (strcmp(nom_vaccin, vaccin[i]->marque) == 0) { // vaccin trouve!
            return(vaccin[i]); // on renvoie juste le vaccin qui nous interesse
        } // si on trouve pas le vaccin, meme au bout de la liste, c'est qu'il n'existe pas
    }
    printf("\n\nLe vaccin %s n'existe pas.", nom_vaccin);
    return(NULL); // vaccin existe pas
}

/*
    Supprime la semaine de numero donne de la liste pass�e
    Retourne la liste modifiee
*/
t_semaine_elt* supprimerSemaine(t_semaine_elt* liste_semaines, int semaine)
{
    if (liste_semaines == NULL) { // cas liste nulle
        printf("\n\nSuppression impossible : semaine inexistante.");
    }
    else {
        if (liste_semaines->numero_semaine == semaine) { // cas ou on doit supprimer la premiere semaine de la liste
            liste_semaines = liste_semaines->suivant;
        }
        else {
            // utilisation de deux temp_villes, car il faut garder trace de maillon parent
            t_semaine_elt* sem_parcourue = liste_semaines;
            t_semaine_elt* sem_precedente = liste_semaines;
            // on cherche la bonne semaine (si elle existe)
            while (sem_parcourue != NULL && sem_parcourue->numero_semaine != semaine && sem_parcourue->numero_semaine < semaine) {
                sem_precedente = sem_parcourue;
                sem_parcourue = sem_parcourue->suivant;
            }
            if (sem_parcourue->numero_semaine == semaine) { // on a trouve la bonne semaine a supprimer
                sem_precedente->suivant = sem_parcourue->suivant;
                free(sem_parcourue); // un des 2, histoire de free() tout ca
                sem_parcourue == NULL; // same
            }
            else {
                printf("\n\nSuppression impossible : semaine inexistante.");
            }
        }
    }
    return(liste_semaines);
}

/*
Supprime la ville de nom donne de la liste passee en parametre
Retourne la liste modifiee
*/
t_ville_elt* supprimerVille(t_ville_elt* liste_villes, char* ville)
{
    if (liste_villes == NULL) {
        printf("\n\nSuppression impossible : ville inexistante.");
    }
    else {
        if (strcmp(liste_villes->nom_ville, ville) == 0) { // cas ou on doit supprimer la premier ville de la liste
            liste_villes = liste_villes->suivant;
        }
        else {
            // utilisation de deux temp_villes, car il faut garder trace de maillon parent
            t_ville_elt* elmt_parcouru = liste_villes;
            t_ville_elt* precedent = liste_villes;
            // on cherche la bonne ville (si elle existe)
            while (elmt_parcouru != NULL && strcmp(elmt_parcouru->nom_ville, ville)!= 0) {
                precedent = elmt_parcouru;
                elmt_parcouru = elmt_parcouru->suivant;
            }
            if (elmt_parcouru!=NULL && strcmp(elmt_parcouru->nom_ville, ville) == 0) {
                precedent->suivant = elmt_parcouru->suivant;
                while (elmt_parcouru->semaines_planifiees != NULL) {
                    supprimerSemaine(elmt_parcouru->semaines_planifiees, elmt_parcouru->semaines_planifiees->numero_semaine);
                }
                free(elmt_parcouru->nom_ville); // on libere l'espace alloue a la chaine de caracteres
                free(elmt_parcouru); // on libere l'espace alloue a la structure ville
            }
            else {
                printf("\n\nSuppression impossible : ville inexistante.");
            }
        }
    }
    return(liste_villes);
}

/*
Méthode permettant de déplacer une ville de façon à garder la liste triée en fonction du nombre de vaccin total de chaque ville
Est appelée dans déduireVaccinV afin de déplacer cette ville vers l'avant si son nombre de vaccin devient inférieur à celui de ses prédécesseurs
*/
t_ville_elt* deplacerVille(t_ville_elt* liste_villes, t_ville_elt* ville)
{
    //Insertion de la ville dans la liste de villes, de fa�on triee
    if (liste_villes == NULL || ville->nombre_vaccins_total <= liste_villes->nombre_vaccins_total) {
        ville->suivant = liste_villes;
        liste_villes = ville;
    }
    else {
        t_ville_elt* precedent = liste_villes; // maillon parent
        t_ville_elt* suivant = liste_villes; // maillon fils (le ->suivant)
        while (suivant != NULL && ville->nombre_vaccins_total > suivant->nombre_vaccins_total) { // s'arrete a la fin de la chaine ou quand le maillon suivant a un nb vaccin total + grand que celui qu'on manipule -> on l'ajoute a cet endroit
            precedent = suivant;
            suivant = suivant->suivant; // il va parcourir toute la liste
        }
        precedent->suivant = ville; // on rebranche les listes
        ville->suivant = suivant;
    }
    return(liste_villes);
}

/*
Méthode triant une liste de villes dans son intégralité. Pas utilisée dans le programme pour économiser en temps d'exécution
(toutes les opérations gardent en effet la liste triée)
*/
t_ville_elt* trierVilles(t_ville_elt* villes) // on va carrement re-creer une nouvelle liste triee et supprimer l'ancienne
{
    t_ville_elt* nouvelle_liste_villes = villes;
    if (villes != NULL) {
        t_ville_elt* temp_ville = villes->suivant;
        nouvelle_liste_villes = NULL; // initialiser a NULL pour etre sur qu'on construit la liste proprement
        while (temp_ville != NULL) { // on copie la liste de ville non triee temp_ville dans la variable nouvelles_villes, qui elle sera triee
            nouvelle_liste_villes = deplacerVille(nouvelle_liste_villes, temp_ville);
            temp_ville = temp_ville->suivant;
        }
    }
    return(nouvelle_liste_villes); // on renvoie juste la nouvelle liste, triee
}

/*
Méthode calculant le nombre de vaccins total d'une ville
*/
int calculTotalVaccinsVille(t_semaine_elt* semaines_planifiees) {
    int totalVaccins = 0;
    while (semaines_planifiees != NULL) {
        totalVaccins += semaines_planifiees->nombre_vaccins;
        semaines_planifiees = semaines_planifiees->suivant;
    }
    return totalVaccins;
}