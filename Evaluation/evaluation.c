/*
* Programme servant à comparer les stratégies de Puissance4
* des élèves de l'IUT de Lannion.
* Arthur Hoarau
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <dirent.h> 
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define LIGNE 6
#define COLONNE 7
#define MAX_STRATS 200
#define MAX_LENGTH 100

const int VIDE = 0;
const int J1 = 1;
const int J2 = 2;

const char JETON1 = 'X';
const char JETON2 = 'O';

const int TEST_NUMBER = 50;

typedef char t_strats[MAX_STRATS][MAX_LENGTH];
typedef int t_grille[LIGNE][COLONNE]; 
typedef int **t_victoires;
typedef int *t_list;

void evaluation(t_grille grille, t_victoires victoires, t_list temps, t_strats listeStrat, int nbStrats);
void ordiContreOrdi(t_grille grille, t_strats listeStrat, int strat1, int strat2);
void trierTemps(t_victoires victoires, t_list temps, t_list arr, int n);
void joueurContreOrdi(t_grille grille, t_strats listeStrat, int strat);
int menu(t_strats listeStrat, int *strat1, int *strat2, int nbStrats);
bool estVainqueur(t_grille grille, int ligne, int colonne);
void trierList(t_victoires victoires, t_list arr, int n);
int jouerOrdi(t_grille grille, int joueur, char* strat);
void afficherStrats(t_strats listeStrat, int nbStrats);
int trouverLigne(t_grille grille, int colonne);
void initListIndices(t_list list, int n);
int chargerStarts(t_strats listeStrat);
int jouer(t_grille grille, int joueur);
void inverserGrille(t_grille grille);
t_victoires initVictoires(int size);
void afficheGrille(t_grille grille);
int choisirColonne(t_grille grille);
bool grillePleine(t_grille grille);
void initGrille(t_grille grille);
void swap(int* xp, int* yp);
t_list initList(int size);
void nettoyerEcran();

// Fonction principale
int main() 
{
    t_grille grille;
    t_strats listeStrat;
    t_victoires victoires = NULL;
    t_list temps = NULL;

    int nbStrats;
    int choix = -1;
    int strat1 = -1;
    int strat2 = -1;

    nbStrats = chargerStarts(listeStrat);
    victoires = initVictoires(nbStrats);
    temps = initList(nbStrats);

    initGrille(grille);

    choix = menu(listeStrat, &strat1, &strat2, nbStrats);
    
    switch (choix)
    {
        case 1:
            joueurContreOrdi(grille, listeStrat, strat1);
            break;
        case 2:
            getchar();
            ordiContreOrdi(grille, listeStrat, strat1, strat2);
            break;
        case 3:
            evaluation(grille, victoires, temps, listeStrat, nbStrats);
            break;
        default:
            break;
    }

    return EXIT_SUCCESS;
}

// Permets de nettoyer l'écran
void nettoyerEcran()
{
    system("@cls||clear");
}

// Menu de sélection du mode d'éxecution
int menu(t_strats listeStrat, int *strat1, int *strat2, int nbStrats)
{
    int choix = -1;

    while (choix == -1)
    {
        nettoyerEcran();

        printf("Choissisez le mode d'exécution :\n\t(1) Joueur Vs Stratégie\n\t(2) Stratégie Vs Stratégie\n\t(3) Lancer l'évaluation\n\t(4) Quitter\n");
        scanf("%d", &choix);

        switch (choix)
        {
            case 1:
                while (*strat1 < 0 || *strat1 > nbStrats - 1)
                {
                    nettoyerEcran();
                    printf("Choissisez la stratégie à affronter :\n");
                    afficherStrats(listeStrat, nbStrats);
                    scanf("%d", strat1);
                    (*strat1)--;
                }
                break;
            case 2:
                while (*strat1 < 0 || *strat1 > nbStrats - 1)
                {
                    nettoyerEcran();
                    printf("Choissisez la première stratégie :\n");
                    afficherStrats(listeStrat, nbStrats);
                    scanf("%d", strat1);
                    (*strat1)--;
                }
                while (*strat2 < 0 || *strat2 > nbStrats - 1)
                {
                    nettoyerEcran();
                    printf("Choissisez la seconde stratégie :\n");
                    afficherStrats(listeStrat, nbStrats);
                    scanf("%d", strat2);
                    (*strat2)--;
                }
                break;
            case 3:
                break;
            case 4:
                choix = -2;
                break;
            default:
                choix = -1;
                break;
        }
    }

    return choix;
}

// Lance l'évaluation finale
void evaluation(t_grille grille, t_victoires victoires, t_list temps, t_strats listeStrat, int nbStrats) 
{
    int gagnant = VIDE;
    int joueur = J1;
    int coups = 0;

    struct timeval tv;
    long start = 0;
    long end = 0;

    t_list nbCoups = initList(nbStrats);

    nettoyerEcran();
    printf("Evaluation en cours...\n\n");
    for(int test = 0; test < TEST_NUMBER; test++)
    {   
        for(int i = 0; i < nbStrats; i++)
        {
            for(int j = 0; j < nbStrats; j++)
            {   
                if(i != j){
                    initGrille(grille);
                    gagnant = VIDE;
                    joueur = J1;
                    coups = 0;
                    while(gagnant == VIDE) 
                    {  
                        coups++;

                        if(joueur == J1){
                            gettimeofday(&tv,NULL);
                            start = 1000000 * tv.tv_sec + tv.tv_usec;
                            gagnant = jouerOrdi(grille, joueur, listeStrat[i]);
                            gettimeofday(&tv,NULL);
                            end = 1000000 * tv.tv_sec + tv.tv_usec;
                            temps[i] += (int)(end - start);
                        }
                            
                        if(joueur == J2) {
                            inverserGrille(grille);
                            gettimeofday(&tv,NULL);
                            start = 1000000 * tv.tv_sec + tv.tv_usec;
                            gagnant = jouerOrdi(grille, joueur, listeStrat[j]);
                            gettimeofday(&tv,NULL);
                            end = 1000000 * tv.tv_sec + tv.tv_usec;
                            temps[j] += (int)(end - start);
                            inverserGrille(grille);
                        }

                        if(joueur == J1) joueur = J2;
                        else joueur = J1;

                        if(grillePleine(grille))
                            break;
                    }

                    if(gagnant == 0){
                        victoires[i][0]++;
                        victoires[j][0]++;
                    }
                    if(gagnant == 1){
                        victoires[i][1]++;
                        victoires[j][2]++;
                        nbCoups[i] += coups;
                    }
                    if(gagnant == 2){
                        victoires[i][2]++;
                        victoires[j][1]++;
                        nbCoups[j] += coups;
                    }
                }
            }
        }
    }
    
    for(int i = 0; i < nbStrats; i++){
        victoires[i][3] += victoires[i][1];
        victoires[i][3] -= victoires[i][2];
    }

    t_list ranked = initList(nbStrats);
    initListIndices(ranked, nbStrats);
    trierList(victoires, ranked, nbStrats);

    // Classement par points
    nettoyerEcran();
    printf("Classement final, par points de victoire :\n");
    for(int i = 0; i < nbStrats; i++){
        printf("\t (%d) %s : %d Victoires, %d Défaites, %d Matchs Nuls, %d Points\n", i + 1, 
                listeStrat[ranked[i]], victoires[ranked[i]][1], victoires[ranked[i]][2], 
                victoires[ranked[i]][0], victoires[ranked[i]][3]);
    }

    // Classement par temps
    ranked = initList(nbStrats);
    initListIndices(ranked, nbStrats);
    trierTemps(victoires, temps, ranked, nbStrats);
    printf("\nClassement final, par temps total en exécution :\n");
    int rank = 1;
    for(int i = 0; i < nbStrats; i++){
        if (victoires[ranked[i]][3] > 0) {
            printf("\t (%d) %s : %.4f secondes\n", rank, listeStrat[ranked[i]], ((double)temps[ranked[i]] / CLOCKS_PER_SEC));
            rank++;
        }
    }

    // Classement par coups
    ranked = initList(nbStrats);
    initListIndices(ranked, nbStrats);
    for(int i = 0; i < nbStrats; i++){
        nbCoups[i] = (int)(((float)nbCoups[i] / victoires[i][1]) * 10);
    }
    trierTemps(victoires, nbCoups, ranked, nbStrats);
    printf("\nClassement final, par nombre de coups moyens sur partie gagnante :\n");
    rank = 1;
    for(int i = 0; i < nbStrats; i++){
        if (victoires[ranked[i]][3] > 0) {
            printf("\t (%d) %s : %.1f coups\n", rank, listeStrat[ranked[i]], nbCoups[ranked[i]] / 10.0);
            rank++;
        }
    }
}

// Lance une partie de d'ordinateur contre ordinateur
void ordiContreOrdi(t_grille grille, t_strats listeStrat, int strat1, int strat2) 
{   
    int gagnant = VIDE;
    int joueur = J1;

    while(gagnant == VIDE) 
    {
        if(joueur == J1){
            gagnant = jouerOrdi(grille, joueur, listeStrat[strat1]);
            afficheGrille(grille);
                printf("\nLa stratégie %s a joué un %c.\nAppuyez sur entrée pour continuer...\n", listeStrat[strat1], JETON1);
            getchar();
        }

        if(joueur == J2){
            inverserGrille(grille);
            gagnant = jouerOrdi(grille, joueur, listeStrat[strat2]);
            inverserGrille(grille);
            afficheGrille(grille);
            printf("\nLa stratégie %s a joué un %c.\nAppuyez sur entrée pour continuer...\n", listeStrat[strat2], JETON2);
            getchar();
        }

        if(joueur == J1) joueur = J2;
        else joueur = J1;

        if(grillePleine(grille))
            break;
    }

    afficheGrille(grille);
    if(gagnant == 0)
        printf("\nLes deux stratégies ont fait égalité !\n");
    if(gagnant == 1)
        printf("\nC'est la stratégie %s qui a gagné !\n", listeStrat[strat1]);
    if(gagnant == 2)
        printf("\nC'est la stratégie %s qui a gagné !\n", listeStrat[strat2]);
}

// Lance une partie contre l'ordinateur
void joueurContreOrdi(t_grille grille, t_strats listeStrat, int strat) 
{
    int joueur = J1;
    int gagnant = VIDE;

    while(gagnant == VIDE) 
    {
        if(joueur == J1)
            gagnant = jouer(grille, joueur);

        if(joueur == J2) {
            inverserGrille(grille);
            gagnant = jouerOrdi(grille, joueur, listeStrat[strat]);
            inverserGrille(grille);
        }

        if(joueur == J1) joueur = J2;
        else joueur = J1;

        if(grillePleine(grille))
            break;
    }

    afficheGrille(grille);
    if(gagnant == 0)
        printf("\nVos avez fait égalité contre %s !\n", listeStrat[strat]);
    if(gagnant == 1)
        printf("\nVos avez gagné contre %s !\n", listeStrat[strat]);
    if(gagnant == 2)
        printf("\nVos avez perdu contre %s !\n", listeStrat[strat]);
}

// Initialise une liste d'indices
void initListIndices(t_list list, int n)
{
    for (int i = 0; i < n; i++) {
        list[i] = i;
    }
}

// Echange deux valeurs
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Fonction de tri (pour classer les stratégies)
void trierList(t_victoires victoires, t_list arr, int n)
{
    int i, j, max_idx;
 
    for (i = 0; i < n - 1; i++) {
        max_idx = i;
        for (j = i + 1; j < n; j++)
            if (victoires[arr[j]][3] > victoires[arr[max_idx]][3])
                max_idx = j;
 
        swap(&arr[max_idx], &arr[i]);
    }
}

// Fonction de tri (pour classer les stratégies)
void trierTemps(t_victoires victoires, t_list temps, t_list arr, int n)
{
    int i, j, min_idx;
 
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (temps[arr[j]] < temps[arr[min_idx]])
                min_idx = j;
 
        swap(&arr[min_idx], &arr[i]);
    }
}

// Inverse la grille pour que la statégie ne voit que des X
void inverserGrille(t_grille grille)
{
    for(int i = 0; i < LIGNE; i++) {
        for(int j = 0; j < COLONNE; j++) {
            grille[i][j] = grille[i][j] * 2 % 3;
        }
    }
}

// Initialise la liste des victoires et défaites
t_victoires initVictoires(int size) 
{
    t_victoires victoires = calloc(size, sizeof(int*));

    for(int i = 0; i < size; i++)
        victoires[i] = (int*)calloc(4, sizeof(int));
    return victoires;
}

// Initialise une liste
t_list initList(int size)
{
    t_list list = (int*)calloc(size, sizeof(int));
    return list;
}

// Permets de charger les stratégies
int chargerStarts(t_strats listeStrat)
{
    DIR *d;
    struct dirent *dir;
    int nbStarts = 0;

    d = opendir("strats");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(!strcmp(".", dir->d_name) || !strcmp("..", dir->d_name)) continue;
            strcpy(listeStrat[nbStarts], dir->d_name);
            nbStarts++;
        }
        closedir(d);
    }

    return nbStarts;
}

// Affiche à l'écran les différentes stratégies
void afficherStrats(t_strats listeStrat, int nbStrats)
{
    for(int i = 0; i < nbStrats; i++) {
        printf("(%d) %s\n", i + 1, listeStrat[i]);
    }
}

// Initialise la grille de jeu
void initGrille(t_grille grille) 
{
    for(int i = 0; i < LIGNE; i++) {
        for(int j = 0; j < COLONNE; j++) {
            grille[i][j] = VIDE;
        }
    }
}

// Affiche la grille de jeu
void afficheGrille(t_grille grille) 
{
    nettoyerEcran();

    for(int i = 0; i < COLONNE; i++) {
        printf("  %d ", i + 1);
    }
    printf("\n\n");

    for(int i = 0; i < LIGNE; i++) {
        for(int j = 0; j < COLONNE; j++) {
            printf("| "); 
            if(grille[i][j] == J1) 
                printf("%c", JETON1);
            else if(grille[i][j] == J2) 
                printf("%c", JETON2);
            else 
                printf(" ");
            printf(" ");
        }
        printf("|\n");
        for(int j = 0; j < COLONNE; j++)
            printf("+---");
        printf("+\n");
    }
}

// Verifie si la grille est pleine
bool grillePleine(t_grille grille)
{
    for(int j = 0; j < COLONNE; j++)
        if(grille[0][j] == VIDE)
            return false;
    return true;
}

// Permets de jouer un coup
int jouer(t_grille grille, int joueur)
{
    int val = 0;
    val = choisirColonne(grille);

    int ligne = 0;
    ligne = trouverLigne(grille, val);

    grille[ligne][val] = joueur;

    if(estVainqueur(grille, ligne, val)) return joueur;
    
    return VIDE;
}

// Permets à l'ordinateur de jouer un coup
int jouerOrdi(t_grille grille, int joueur, char* strat)
{
    char systemExec[255] = "./strats/";
    strcat(systemExec, strat);
    for(int i = 0; i < LIGNE; i++){
         for(int j = 0; j < COLONNE; j++){
            strcat(systemExec, " ");
            char str[3];
            sprintf(str, "%d", grille[i][j]);
            strcat(systemExec, str);
        }
    }

    int status = system(systemExec);
    int colonne = status / 256;

    // Si la stratégie renvoit une colonne eronnée, c'est une défaite !
    if(colonne < 0 || colonne > 6 || grille[0][colonne] != VIDE)
        return (joueur * 2) % 3;

    int ligne = 0;
    ligne = trouverLigne(grille, colonne);

    grille[ligne][colonne] = J1;

    if(estVainqueur(grille, ligne, colonne)) return joueur;

    return VIDE;
}

// Permets de choisir la colonne
int choisirColonne(t_grille grille)
{
    int col = -1;

    while(col < 1 || col > 7 || grille[0][col - 1] != VIDE){
        afficheGrille(grille);
        printf("Choisissez une colonne : ");
        scanf("%d", &col);
    }

    return col - 1;
}

// Permets de trouver la première case vide d'une colonne
int trouverLigne(t_grille grille, int colonne)
{
    for(int j = LIGNE - 1; j >= 0; j--) {
        if(grille[j][colonne] == VIDE) return j;
    }
    return -1;
}

// Vérifie si 4 jetons sont alignés
bool estVainqueur(t_grille grille, int ligne, int colonne) 
{
    int aligne;

    // Horizontal
    aligne = 0;
    for(int j = colonne - 3; j <= colonne + 3; j++) {
        if(j >= 0 && j < COLONNE){
            if(grille[ligne][j] == grille[ligne][colonne]) aligne++;
            else aligne = 0;
        }
        if(aligne == 4) return true;
    }

    // Vertical
    aligne = 0;
    for(int i = ligne - 3; i <= ligne + 3; i++) {
        if(i >= 0 && i < LIGNE){
            if(grille[i][colonne] == grille[ligne][colonne]) aligne++;
            else aligne = 0;
        }
        if(aligne == 4) return true;
    }

    // Diagonale 1
    aligne = 0;
    for(int i = -3; i <= 3; i++) {
        if(i + ligne >= 0 && i + ligne < LIGNE){
            if(i + colonne >= 0 && i + colonne < COLONNE){
                if(grille[i + ligne][i + colonne] == grille[ligne][colonne]) aligne++;
                else aligne = 0;
            }
        }
        if(aligne == 4) return true;
    }

    // Diagonale 2
    aligne = 0;
    for(int i = -3; i <= 3; i++) {
        if(ligne - i >= 0 && ligne - i < LIGNE){
            if(i + colonne >= 0 && i + colonne < COLONNE){
                if(grille[ligne - i][i + colonne] == grille[ligne][colonne]) aligne++;
                else aligne = 0;
            }
        }
        if(aligne == 4) return true;
    }

    return false;
}