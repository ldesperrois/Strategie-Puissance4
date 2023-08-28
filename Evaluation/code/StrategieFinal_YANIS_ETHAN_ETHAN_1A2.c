
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

//Les structures
typedef struct
{
    int ligne;
    int colonne;
}s_coord;

const s_coord COORDINIT = {-10, -10};

#define NB_LIGNES 6
#define NB_COLONNES 7

typedef int t_grille[NB_LIGNES][NB_COLONNES];

int main(int argc, char** argv);
void chargerGrille(char** argv, t_grille grille);
int maStrategie(t_grille laGrille, t_grille grillecopie);
void initGrille(t_grille grille) ;
int analysegrille(t_grille laGrille, s_coord coord_pion);
bool grillevide(t_grille laGrille);
bool grille_un_pion(t_grille laGrille);
void pinit(t_grille laGrille, t_grille GrilleCopie);
s_coord dernierpionjouer(t_grille laGrille, t_grille laGrille2);
void grille_to_fichier(t_grille grille);
void fichier_to_grille(t_grille grille);
int chercherLigne(t_grille laGrille, int col);
bool chercherpion(t_grille laGrille, char pion, int deb, int fin, int ligne);
bool estVainqueur(t_grille laGrille, int lig, int col);
int chercherpiondiagSOtoNE(t_grille laGrille, s_coord pion, s_coord *contre);
int chercherpiondiagNOtoSE(t_grille laGrille, s_coord pion, s_coord *contre);

const int VIDE = 0;
const int JOUEUR = 1;
const int ADVERSAIRE = 2;

/*
* VOTRE STRATEGIE
*/
int maStrategie(t_grille laGrille, t_grille grillecopie)
{
    // TODO: Votre strategie de victoire. La seule fonction que vous aurez à modifier.
    // libre à vous d'en créer d'autres pour aérer votre code. 
    // La variable grille est un tableau à deux dimensions d'entiers, c'est la grille du tour. 
    // Un VIDE représente une case vide, 1 représente vos jetons et 2 représente les jetons adverses.
    // A vous de choisir le meilleur prochain coup !

    int col;
    bool victoire;
    int ligne;
    s_coord coordonee;

    if(grillevide(laGrille)==true){ // Cas si c'est cette stratégie qui commence on met le pion à gauche
    
        col = 3;
        pinit(laGrille, grillecopie); //
        grillecopie[chercherLigne(laGrille, col)][col]=ADVERSAIRE;
    } else { // Si c'est pas le premier jeton alors on commence notre stratégie
        for (int i = 0; i < NB_COLONNES; i++){
            ligne = chercherLigne(laGrille, i);
            if (ligne > -1){
                laGrille[ligne][i] = ADVERSAIRE;
                victoire = estVainqueur(laGrille, ligne, i);
                laGrille[ligne][i] = VIDE;
                if (victoire){
                    return i;
                }
            }
        }
        if(grille_un_pion(laGrille)==true){
            initGrille(grillecopie);
        }
        else{
            fichier_to_grille(grillecopie);
        }      
        coordonee = dernierpionjouer(laGrille, grillecopie);
        col = analysegrille(laGrille, coordonee);
    }
    pinit(laGrille, grillecopie);
    grillecopie[chercherLigne(laGrille, col)][col] = JOUEUR; // Met le pion que je vais jouer dans la grille copié 
    grille_to_fichier(grillecopie);
    return col;
}

void grille_to_fichier(t_grille grille){
    FILE * f;
    f = fopen("GRILLE.DATA", "wb");
    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            fwrite(&grille[i][j], sizeof(grille[i][j]), 1, f);
        }
    }
}

void fichier_to_grille(t_grille grille){
    FILE * f;
    f = fopen("GRILLE.DATA", "rb");
    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            fread(&grille[i][j], sizeof(grille[i][j]), 1, f);
        }
    }
}

void initGrille(t_grille grille) 
{
    for(int i = 0; i < NB_LIGNES; i++) {
        for(int j = 0; j < NB_COLONNES; j++) {
            grille[i][j] = VIDE;
        }
    }
}

/**
 * @fn int analysegrille(Grille laGrille)
 * @brief Etape une de regarder dans la grille par rapport au dernier pion s'il y à 2 minimum d'aligner si c'est le cas alors faut regarder dans quel sens et trouver l'endroit ou placer notre pion pour couper l'ennemie
 * @param Grille : lagrille
 * La fonction va prendre les coord puis regarder à droite à gauche en haut en bas s'il n'y a pas 
 */
int analysegrille(t_grille laGrille, s_coord coord_pion){ // Seulement en entrée car pas besoin de modifié le contenu
    int col;
    char pion;
    int cpt,i, indent;
    bool booleen;
    int compt;
    s_coord pionAJouer;
    compt = 1;

    col = 3; // Si pas de situation alors on met le pion dans la colonne 3 ce qui permet de limiter le nombre de cas horizontale changement par rapport au premier rendu
        // Rajouter un systeme car si la colonne 3 est pleine 

    while(chercherLigne(laGrille, col)==-1 && col < NB_COLONNES && col >= 0){
        if (chercherLigne(laGrille, col+compt)!=-1){
            col = col+compt;
        }
        else if(chercherLigne(laGrille, col-compt)!=-1){
            col = col-compt;
        }
            compt++;
    } 

    pion = laGrille[coord_pion.ligne][coord_pion.colonne];


    // VERTICALEMENT ! REGARDE EN DESSOUS S'IL FAUT PLACER UN PION
    i = coord_pion.ligne;
    cpt = 0;
    while (i<NB_LIGNES && laGrille[i][coord_pion.colonne]==pion){
        cpt++;
        i++;
    }
    if (cpt>=3){
        col = coord_pion.colonne;
    }

    // VERTICALEMENT ! REGARDE EN DESSOUS S'IL FAUT PLACER UN PION

    if((laGrille[coord_pion.ligne][3]==pion) && (chercherpion(laGrille, pion, 0, 3, coord_pion.ligne)) && (chercherpion(laGrille, pion, 3, NB_COLONNES-1, coord_pion.ligne))){
        indent = 3;/*&& que entre milieu et fin pion a moi n'est pas alors ou que si entre debut et milieu pion a moi n'est pas*/
        if(coord_pion.colonne > 3 && coord_pion.colonne <=NB_COLONNES){
            booleen = false;
            while(indent<=NB_COLONNES && booleen==false)
            {
                indent++;
                if(chercherLigne(laGrille, indent)==coord_pion.ligne){
                    col = indent;
                    booleen = true;
                }
            }
        }
        else if (coord_pion.colonne < 3 && coord_pion.colonne >= 0)
        {
            booleen = false;
            while (indent>=0 && booleen==false)
            {
                indent--;
                if(chercherLigne(laGrille, indent)==coord_pion.ligne){
                    col = indent;
                    booleen = true;
                }
            }
        }
    }
    
    if (chercherpiondiagNOtoSE(laGrille, coord_pion, &pionAJouer) != 0){
        if (chercherLigne(laGrille, pionAJouer.colonne) == pionAJouer.ligne){
            col = pionAJouer.colonne;
        }
    }
    if (chercherpiondiagSOtoNE(laGrille, coord_pion, &pionAJouer) != 0){
        if (chercherLigne(laGrille, pionAJouer.colonne) == pionAJouer.ligne){
            col = pionAJouer.colonne;
        }
    }
    return col;
}

/**
 * @fn bool grillevide(Grille laGrille)
 * @brief fonction qui sert dans la stratégie 5 "Défensive" 
 * @param grille : La grille du jeu
 * @result bool : renvois un booléen true si vide false si au moins un pions
*/

bool grillevide(t_grille laGrille){
    bool res;
    res = true;
    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            if((laGrille[i][j]==JOUEUR) || (laGrille[i][j]==ADVERSAIRE)){
                res = false;
            }
        }
    }
    return res;
}

bool grille_un_pion(t_grille laGrille){
    bool res;
    int cpt;
    res = false;
    cpt = 0;
    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            if((laGrille[i][j]==JOUEUR) || (laGrille[i][j]==ADVERSAIRE)){
                cpt++;
            }
        }
    }
    if(cpt==1){
        res = true;
    }
    return res;
}

void pinit(t_grille laGrille, t_grille GrilleCopie){
    for(int i = 0; i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            GrilleCopie[i][j] = laGrille[i][j];
        }
    } 
}

s_coord dernierpionjouer(t_grille laGrille, t_grille laGrille2) {
    s_coord last_point;
    for (int i = 0; i < NB_LIGNES; i++) {
        for (int j = 0; j < NB_COLONNES; j++) {
            if (laGrille[i][j] != laGrille2[i][j]) {
                last_point.ligne = i;
                last_point.colonne = j;
            }
        }
    }
    return last_point;
}

int chercherLigne(t_grille laGrille, int col){
    int ligne = -1;

    while (ligne<NB_LIGNES-1 && laGrille[ligne+1][col]==VIDE){
        ligne++;
    }
    return ligne;
}

bool chercherpion(t_grille laGrille, char pion, int deb, int fin, int ligne){
    bool res;
    res = false;

    int i;
    i = deb;

    while (i<=fin)
    {
       if(laGrille[ligne][i] == pion){
        res = true;
       }
       i++;
    }
    return res;
}

// Diagonale Sud Ouest - Nord EST
int chercherpiondiagSOtoNE(t_grille laGrille, s_coord pion, s_coord *contre){
    int canWin = 0;
    bool bloqueHaut = false;
    int nbBas, nbHaut, nbTotal, nbPlace, nbPlaceHaut, nbPlaceBas;
    s_coord pionVideBas = {-1, -1};
    s_coord pionVideHaut = {-1, -1};
    nbBas = 0;
    nbPlaceBas = 0;
    nbHaut = 0;
    nbPlaceHaut = 0;
    int i, j;
    i = pion.colonne;
    j = pion.ligne;
    while (i > 0 && j < NB_LIGNES-1 && laGrille[j][i] != JOUEUR){
        i--;
        j++;    
        if (laGrille[j][i] == VIDE && pionVideBas.ligne == -1){
            pionVideBas.ligne = j;
            pionVideBas.colonne = i;
        }
        nbBas++;
        if (laGrille[j][i] == ADVERSAIRE){
            nbPlaceBas++;
        }
    }

    i = pion.colonne;
    j = pion.ligne;
    while (i < NB_COLONNES-1 && j > 0 && laGrille[j][i] != JOUEUR){
        i++;
        j--;
        if (laGrille[j][i] == JOUEUR){
            bloqueHaut = true;
        }  else if (laGrille[j][i] == VIDE && pionVideHaut.ligne == -1){
            pionVideHaut.ligne = j;
            pionVideHaut.colonne = i;
        }
        nbHaut++;
        if (laGrille[j][i] == ADVERSAIRE){
            nbPlaceHaut++;
        }
    }

    nbTotal = 1 + nbBas + nbHaut;
    nbPlace = 1 + nbPlaceBas + nbPlaceHaut;
    if (nbTotal > 4 && nbPlace >= 2){
        if (nbPlaceBas > nbPlaceHaut || bloqueHaut){
            canWin = -1;
            *contre = pionVideBas;
        } else {
            canWin = 1;
            *contre = pionVideHaut;
        }
    }
    return canWin;
}

// Diagonale Nord Ouest - Sud Est
int chercherpiondiagNOtoSE(t_grille laGrille, s_coord pion, s_coord *contre){
    int canWin = 0;
    bool bloqueHaut = false;
    int nbBas, nbHaut, nbTotal, nbPlace, nbPlaceHaut, nbPlaceBas;
    s_coord pionVideBas = {-1, -1};
    s_coord pionVideHaut = {-1, -1};
    nbBas = 0;
    nbPlaceBas = 0;
    nbHaut = 0;
    nbPlaceHaut = 0;
    int i, j;
    i = pion.colonne;
    j = pion.ligne;
    while (i > 0 && j > 0 && laGrille[j][i] != JOUEUR){
        i--;
        j--;    
        if (laGrille[j][i] == VIDE && pionVideBas.ligne == -1){
            pionVideBas.ligne = j;
            pionVideBas.colonne = i;
        }
        nbBas++;
        if (laGrille[j][i] == ADVERSAIRE){
            nbPlaceBas++;
        }
    }

    i = pion.colonne;
    j = pion.ligne;
    while (i < NB_COLONNES-1 && j < NB_LIGNES - 1 && laGrille[j][i] != JOUEUR){
        i++;
        j++;
        if (laGrille[j][i] == JOUEUR){
            bloqueHaut = true;
        }  else if (laGrille[j][i] == VIDE && pionVideHaut.ligne == -1){
            pionVideHaut.ligne = j;
            pionVideHaut.colonne = i;
        }
        nbHaut++;
        if (laGrille[j][i] == ADVERSAIRE){
            nbPlaceHaut++;
        }
    }
    nbTotal = 1 + nbBas + nbHaut;
    nbPlace = 1 + nbPlaceBas + nbPlaceHaut;
    if (nbTotal >= 4 && nbPlace >= 2){
        if (nbPlaceBas > nbPlaceHaut || bloqueHaut){
            canWin = -1;
            *contre = pionVideBas;
        } else {
            *contre = pionVideHaut;
            return 1;
        }
    }
    return canWin;
}

// La fonction principale reçoit la grille du tour et retourne le coup choisi
// Vous n'avez pas à modifier cette fonction
int main(int argc, char** argv) 
{
    t_grille grille;
    t_grille grillecopie;

    chargerGrille(argv, grille);

    return maStrategie(grille, grillecopie);
}

// Charge la grille du tour actuel
// Vous n'avez pas à modifier cette fonction
void chargerGrille(char** argv, t_grille grille) 
{
    for(int i = 0; i < NB_LIGNES; i++)
        for(int j = 0; j < NB_COLONNES; j++)
            grille[i][j] = atoi(argv[i * NB_COLONNES + j + 1]);
}

bool estVainqueur(t_grille laGrille, int lig, int col){
    // consiste Ã  regarder si une ligne de 4 pions s'est formÃ©e autour du pion qui vient de tomber en (lig, col)
    int lePion = laGrille[lig][col];
    int cpt,i,j;

    // regarder la verticale, en comptant le nombre de pions au Sud (inutile de regarder au Nord du pion qui vient de tomber)
    i = lig;
    cpt = 0;
    while (i<NB_LIGNES && laGrille[i][col]==lePion){
        cpt++;
        i++;
    }
    if (cpt>=4){
        return true;
    }
    // regarder l'horizontale, en comptant le nombre de pions Ã  l'Est et Ã  l'Ouest
    j = col;
    cpt = 0;
    // on regarde Ã  l'est
    while (j>=0 && laGrille[lig][j]==lePion){
        cpt++;
        j--;
    }
    j = col+1;
    // on regarde Ã  l'ouest
    while (j<NB_COLONNES && laGrille[lig][j]==lePion){
        cpt++;
        j++;
    }
    if (cpt>=4 ){
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Ouest et au Sud Est
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Ouest
    while (j>=0 && i>=0 && laGrille[i][j]==lePion){
        cpt++;
        i--;
        j--;
    }
    i = lig+1;
    j = col+1;
    // on regarde au Sud Est
    while (i<NB_LIGNES && j<NB_COLONNES && laGrille[i][j]==lePion){
        cpt++;
        i++;
        j++;
    }
    if (cpt>=4 ){
        return true;
    }
    // regarder la diagonale descendante, en comptant le nombre de pions au Nord Est et au Sud Ouest
    i = lig;
    j = col;
    cpt = 0;
    // on regarde au Nord Est
    while (j<NB_COLONNES && i>=0 && laGrille[i][j]==lePion){
        cpt++;
        i--;
        j++;
    }
    i = lig+1;
    j = col-1;
    // on regarde au Sud Ouest
    while (i<NB_LIGNES && j>=0 && laGrille[i][j]==lePion){
        cpt++;
        i++;
        j--;
    }
    if (cpt>=4 ){
        return true;
    }
    return false;
}
