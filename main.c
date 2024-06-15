#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LIVRES 100
 = 0;#define MAX_EMPRUNTEURS 50
#define MAX_TITRE_LIVRE 100
#define MAX_NOM_AUTEUR 50
#define MAX_LIVRES_EMPRUNTES 5
#define FICHIER_BASE_DE_DONNEES "bibliotheque_base_de_donnees.txt"
/*
        PAR LE GROUPE G30 
        AVEC ILUNGA NGOLE KENAM 
        KALENGA LAILAI MOISE
        KASONGO MAMBWE PASCAL 
*/
typedef struct {
    int id;
    char titre[MAX_TITRE_LIVRE];
    char auteur[MAX_NOM_AUTEUR];
    int anneePublication;
    int disponible;
} Livre;

typedef struct {
    int id;
    char nom[50];
    Livre* livresEmpruntes[MAX_LIVRES_EMPRUNTES];
    int nombreLivresEmpruntes;
} Emprunteur;

Livre livres[MAX_LIVRES];
Emprunteur emprunteurs[MAX_EMPRUNTEURS];
int nombreLivres = 0, nombreEmprunteurs = 0;

void chargerBaseDeDonnees() {
    FILE* fichier = fopen(FICHIER_BASE_DE_DONNEES, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier de base de donn�es.\n");
        return;
    }

    // Charger les livres
    fscanf(fichier, "%d\n", &nombreLivres);
    for (int i = 0; i < nombreLivres; i++) {
        fscanf(fichier, "%d,%[^,],%[^,],%d,%d\n", &livres[i].id, livres[i].titre, livres[i].auteur, &livres[i].anneePublication, &livres[i].disponible);
    }

    // Charger les emprunteurs
    fscanf(fichier, "%d\n", &nombreEmprunteurs);
    for (int i = 0; i < nombreEmprunteurs; i++) {
        fscanf(fichier, "%d,%[^\n]\n", &emprunteurs[i].id, emprunteurs[i].nom);
        int nombreLivresEmpruntes;
        fscanf(fichier, "%d\n", &nombreLivresEmpruntes);
        emprunteurs[i].nombreLivresEmpruntes = nombreLivresEmpruntes;
        for (int j = 0; j < nombreLivresEmpruntes; j++) {
            int idLivre;
            fscanf(fichier, "%d,", &idLivre);
            for (int k = 0; k < nombreLivres; k++) {
                if (livres[k].id == idLivre) {
                    emprunteurs[i].livresEmpruntes[j] = &livres[k];
                    break;
                }
            }
        }
        fscanf(fichier, "\n");
    }

    fclose(fichier);
}

void enregistrerBaseDeDonnees() {
    FILE* fichier = fopen(FICHIER_BASE_DE_DONNEES, "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier de base de donn�es.\n");
        return;
    }

    // Enregistrer les livres
    fprintf(fichier, "%d\n", nombreLivres);
    for (int i = 0; i < nombreLivres; i++) {
        fprintf(fichier, "%d,%s,%s,%d,%d\n", livres[i].id, livres[i].titre, livres[i].auteur, livres[i].anneePublication, livres[i].disponible);
    }

    // Enregistrer les emprunteurs
    fprintf(fichier, "%d\n", nombreEmprunteurs);
    for (int i = 0; i < nombreEmprunteurs; i++) {
        fprintf(fichier, "%d,%s\n", emprunteurs[i].id, emprunteurs[i].nom);
        fprintf(fichier, "%d\n", emprunteurs[i].nombreLivresEmpruntes);
        for (int j = 0; j < emprunteurs[i].nombreLivresEmpruntes; j++) {
            fprintf(fichier, "%d,", emprunteurs[i].livresEmpruntes[j]->id);
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
}

int ajouterLivre(int id, char* titre, char* auteur, int anneePublication) {
    if (nombreLivres >= MAX_LIVRES) {
        printf("D�sol�, la biblioth�que a atteint la capacit� maximale de livres.\n");
        return 0;
    }

    livres[nombreLivres].id = id;
    strncpy(livres[nombreLivres].titre, titre, MAX_TITRE_LIVRE);
    strncpy(livres[nombreLivres].auteur, auteur, MAX_NOM_AUTEUR);
    livres[nombreLivres].anneePublication = anneePublication;
    livres[nombreLivres].disponible = 1;
    nombreLivres++;

    printf("Le livre '%s' de l'auteur '%s' a �t� ajout� avec succ�s.\n", titre, auteur);
    return 1;
}

int emprunterLivre(int idEmprunteur, int idLivre) {
    // Trouver l'emprunteur
    Emprunteur* emprunteur = NULL;
    for (int i = 0; i < nombreEmprunteurs; i++) {
        if (emprunteurs[i].id == idEmprunteur) {
            emprunteur = &emprunteurs[i];
            break;
        }
    }
    if (emprunteur == NULL) {
        printf("Emprunteur introuvable.\n");
        return 0;
    }

    // Trouver le livre
    Livre* livre = NULL;
    for (int i = 0; i < nombreLivres; i++) {
        if (livres[i].id == idLivre && livres[i].disponible) {
            livre = &livres[i];
            break;
        }
    }
    if (livre == NULL) {
        printf("Livre indisponible.\n");
        return 0;
    }

    // Ajouter le livre � l'emprunteur
    emprunteur->livresEmpruntes[emprunteur->nombreLivresEmpruntes++] = livre;
    livre->disponible = 0;
    printf("Le livre '%s' a �t� emprunt� avec succ�s.\n", livre->titre);
    return 1;
}
void rechercherLivre(char* titre, char* auteur) {
    int trouve = 0;
    for (int i = 0; i < nombreLivres; i++) {
        if (strstr(livres[i].titre, titre) != NULL && strstr(livres[i].auteur, auteur) != NULL) {
            printf("------------------------------------------------------------\n");
            printf("Livre trouv� :\n");
            printf("ID : %d\n", livres[i].id);
            printf("Titre : %s\n", livres[i].titre);
            printf("Auteur : %s\n", livres[i].auteur);
            printf("Ann�e de publication : %d\n", livres[i].anneePublication);
            printf("Disponibilit� : %s\n", livres[i].disponible ? "Oui" : "Non");
            trouve = 1;
        }
    }
    if (!trouve) {
        printf("Aucun livre ne correspond � votre recherche.\n");
    }
}

int rendreLivre(int idEmprunteur, int idLivre) {
    // Trouver l'emprunteur
    Emprunteur* emprunteur = NULL;
    for (int i = 0; i < nombreEmprunteurs; i++) {
        if (emprunteurs[i].id == idEmprunteur) {
            emprunteur = &emprunteurs[i];
            break;
        }
    }
    if (emprunteur == NULL) {
        printf("Emprunteur introuvable.\n");
        return 0;
    }

    // Trouver le livre emprunt�
    Livre* livre = NULL;
    int indexLivre = -1;
    for (int i = 0; i < emprunteur->nombreLivresEmpruntes; i++) {
        if (emprunteur->livresEmpruntes[i]->id == idLivre) {
            livre = emprunteur->livresEmpruntes[i];
            indexLivre = i;
            break;
        }
    }
    if (livre == NULL) {
        printf("Le livre %d n'est pas emprunt� par l'emprunteur %d.\n", idLivre, idEmprunteur);
        return 0;
    }

    // Supprimer le livre de la liste des livres emprunt�s
    for (int i = indexLivre; i < emprunteur->nombreLivresEmpruntes - 1; i++) {
        emprunteur->livresEmpruntes[i] = emprunteur->livresEmpruntes[i + 1];
    }
    emprunteur->nombreLivresEmpruntes--;
    livre->disponible = 1;
    printf("Le livre '%s' a �t� rendu avec succ�s.\n", livre->titre);
    return 1;
}

int ajouterEmprunteur(int id, char* nom) {
    if (nombreEmprunteurs >= MAX_EMPRUNTEURS) {
        printf("D�sol�, la biblioth�que a atteint la capacit� maximale d'emprunteurs.\n");
        return 0;
    }

    emprunteurs[nombreEmprunteurs].id = id;
    strncpy(emprunteurs[nombreEmprunteurs].nom, nom, 50);
    emprunteurs[nombreEmprunteurs].nombreLivresEmpruntes = 0;
    nombreEmprunteurs++;

    printf("L'emprunteur '%s' a �t� ajout� avec succ�s.\n", nom);
    return 1;
}

void afficherLivres() {
    if (nombreLivres == 0) {
        printf("Il n'y a pas de livres dans la biblioth�que.\n");
        return;
    }

    printf("Liste des livres :\n");
    printf("------------------------------------------------------------\n");
    printf("ID | Titre | Auteur | Ann�e de Publication | Disponible\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < nombreLivres; i++) {
        printf("%d | %s | %s | %d | %s\n",
               livres[i].id, livres[i].titre, livres[i].auteur, livres[i].anneePublication, livres[i].disponible ? "Oui" : "Non");
    }
    printf("------------------------------------------------------------\n");
}

void afficherEmprunteurs() {
    if (nombreEmprunteurs == 0) {
        printf("Il n'y a pas d'emprunteurs dans la biblioth�que.\n");
        return;
    }

    printf("Liste des emprunteurs :\n");
    printf("------------------------------------------------------------\n");
    printf("ID | Nom | Livres emprunt�s\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < nombreEmprunteurs; i++) {
        printf("%d | %s | ", emprunteurs[i].id, emprunteurs[i].nom);
        if (emprunteurs[i].nombreLivresEmpruntes == 0) {
            printf("Aucun livre emprunt�\n");
        } else {
            for (int j = 0; j < emprunteurs[i].nombreLivresEmpruntes; j++) {
                printf("%s ", emprunteurs[i].livresEmpruntes[j]->titre);
            }
            printf("\n");
        }
    }
    printf("------------------------------------------------------------\n");
}

int main() {
    chargerBaseDeDonnees();

    int choix;
    do {
        printf("------------------------------------------------------------\n");
        printf("                       MENU PRINCIPAL                       \n");
        printf("------------------------------------------------------------\n");
        printf("1. Ajouter un livre\n");
        printf("2. Emprunter un livre\n");
        printf("3. Rendre un livre\n");
        printf("4. Afficher la liste des livres\n");
        printf("5. Afficher la liste des emprunteurs\n");
        printf("6. Ajouter un emprunteur\n");
        printf("7. Chercher un livre\n");
        printf("8. Quitter\n");
        printf("------------------------------------------------------------\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar();
        printf("------------------------------------------------------------\n");

        switch (choix) {
            case 1: {
                int id, anneePublication;
                char titre[MAX_TITRE_LIVRE], auteur[MAX_NOM_AUTEUR];
                printf("Entrez l'ID du livre : ");
                scanf("%d", &id);
                printf("Entrez le titre du livre : ");
                scanf(" %[^\n]", titre);
                printf("Entrez l'auteur du livre : ");
                scanf(" %[^\n]", auteur);
                printf("Entrez l'année de publication du livre : ");
                scanf("%d", &anneePublication);
                ajouterLivre(id, titre, auteur, anneePublication);
                break;
            }
            case 2: {
                int idEmprunteur, idLivre;
                printf("Entrez l'ID de l'emprunteur : ");
                scanf("%d", &idEmprunteur);
                printf("Entrez l'ID du livre à emprunter : ");
                scanf("%d", &idLivre);
                emprunterLivre(idEmprunteur, idLivre);
                break;
            }
            case 3: {
                int idEmprunteur, idLivre;
                printf("Entrez l'ID de l'emprunteur : ");
                scanf("%d", &idEmprunteur);
                printf("Entrez l'ID du livre à rendre : ");
                scanf("%d", &idLivre);
                rendreLivre(idEmprunteur, idLivre);
                break;
            }
            case 4:
                afficherLivres();
                break;
            case 5:
                afficherEmprunteurs();
                break;
            case 6: {
                int id;
                char nom[50];
                printf("Entrez l'ID de l'emprunteur : ");
                scanf("%d", &id);
                printf("Entrez le nom de l'emprunteur : ");
                scanf(" %[^\n]", nom);
                ajouterEmprunteur(id, nom);
                break;
            }
            case 7: {
                char titre[MAX_TITRE_LIVRE];
                char auteur[MAX_NOM_AUTEUR];
                printf("Titre du livre : ");
                scanf(" %[^\n]", titre);
                printf("Auteur du livre : ");
                scanf(" %[^\n]", auteur);
                rechercherLivre(titre, auteur);
                break;
            }
            case 8:
                enregistrerBaseDeDonnees();
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
        }

        printf("------------------------------------------------------------\n");

    } while (choix != 8);

    return 0;
}
