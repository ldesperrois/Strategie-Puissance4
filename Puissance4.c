#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <ctype.h> 

// les constantes
#define NB_LIGNES 6
#define NB_COLONNES 7
#define PION_A 'X'
#define PION_B 'O'

#define INCONNU ' '



typedef int t_grille[NB_LIGNES][NB_COLONNES];

int main(int argc, char** argv);
void chargerGrille(char** argv, t_grille grille);
void placerPion(int ligne,int colonne,t_grille t,int Pion);
void nettoyerEcran();
int choisirColonne(int Joueur,t_grille grille);
int coup_ordi(t_grille t);

const int VIDE = 0;
const int JOUEUR = 1;
const int ADVERSAIRE = 2;


/**
 * @brief fonction qui initalise une grille vide
 * 
 * @param grille 
 */
void initgrille(t_grille grille){
    for(int i=0;i<NB_LIGNES;i++){
        for(int j=0;j<NB_COLONNES;j++){
            grille[i][j]=VIDE;
        }
    }
}


/**
 * @brief 
 * Fonction pour regarder si la grille est remplie (égalité ou victoire)
 * 
 * @param grille 
 * @return true 
 * @return false 
 */
bool grillePleine(t_grille grille)
{
    for(int j = 0; j < NB_COLONNES; j++)
        if(grille[0][j] == VIDE)
            return false;
    return true;
}
/**
 * @brief 
 * Retourne le joueur vainqueur sur la grille
 * utilise par min-max pour regarder qui a gagné sur une simulation de grille
 * 
 * @param grille 
 * @param ligne 
 * @param colonne 
 * @return int 
 */
int estVainqueurOrdi(t_grille grille, int ligne, int colonne) {
    int aligne = 0;

    // Vérifier l'alignement horizontal
    for(int j = colonne - 3; j <= colonne + 3; j++) {
        if(j >= 0 && j < NB_COLONNES) {
            if(grille[ligne][j] == grille[ligne][colonne]) {
                aligne++;
                if(aligne == 4) {
                    return grille[ligne][colonne];
                }
            } else {
                aligne = 0;
            }
        }
    }

    // Vérifier l'alignement vertical
    aligne = 0;
    for(int i = ligne - 3; i <= ligne + 3; i++) {
        if(i >= 0 && i < NB_LIGNES) {
            if(grille[i][colonne] == grille[ligne][colonne]) {
                aligne++;
                if(aligne == 4) {
                    return grille[ligne][colonne];
                }
            } else {
                aligne = 0;
            }
        }
    }

    // Vérifier la diagonale descendante (haut-gauche à bas-droite)
    aligne = 0;
    for(int i = -3; i <= 3; i++) {
        if(ligne + i >= 0 && ligne + i < NB_LIGNES && colonne + i >= 0 && colonne + i < NB_COLONNES) {
            if(grille[ligne + i][colonne + i] == grille[ligne][colonne]) {
                aligne++;
                if(aligne == 4) {
                    return grille[ligne][colonne];
                }
            } else {
                aligne = 0;
            }
        }
    }

    // Vérifier la diagonale montante (bas-gauche à haut-droite)
    aligne = 0;
    for(int i = -3; i <= 3; i++) {
        if(ligne - i >= 0 && ligne - i < NB_LIGNES && colonne + i >= 0 && colonne + i < NB_COLONNES) {
            if(grille[ligne - i][colonne + i] == grille[ligne][colonne]) {
                aligne++;
                if(aligne == 4) {
                    return grille[ligne][colonne];
                }
            } else {
                aligne = 0;
            }
        }
    }

    // Vérifier si la grille est pleine
    if(grillePleine(grille)) {
        return -1; // Match nul
    }

    return -2; // Aucun vainqueur pour le moment
}
/**
 * @brief 
 * Fonction qui recherche la victoire d'un joueur après son coup
 * 
 * @param grille 
 * @param ligne 
 * @param colonne 
 * @return true 
 * @return false 
 */
bool estVainqueur(t_grille grille, int ligne, int colonne) 
{
    int aligne;

    // Horizontal
    aligne = 0;
    for(int j = colonne - 3; j <= colonne + 3; j++) {
        if(j >= 0 && j < NB_COLONNES){
            if(grille[ligne][j] == grille[ligne][colonne]) aligne++;
            else aligne = 0;
        }
        if(aligne == 4) return true;
    }

    // Vertical
    aligne = 0;
    for(int i = ligne - 3; i <= ligne + 3; i++) {
        if(i >= 0 && i < NB_LIGNES){
            if(grille[i][colonne] == grille[ligne][colonne]) aligne++;
            else aligne = 0;
        }
        if(aligne == 4) return true;
    }

    // Diagonale 1
    aligne = 0;
    for(int i = -3; i <= 3; i++) {
        if(i + ligne >= 0 && i + ligne < NB_LIGNES){
            if(i + colonne >= 0 && i + colonne < NB_COLONNES){
                if(grille[i + ligne][i + colonne] == grille[ligne][colonne]) aligne++;
                else aligne = 0;
            }
        }
        if(aligne == 4) return true;
    }

    // Diagonale 2
    aligne = 0;
    for(int i = -3; i <= 3; i++) {
        if(ligne - i >= 0 && ligne - i < NB_LIGNES){
            if(i + colonne >= 0 && i + colonne < NB_COLONNES){
                if(grille[ligne - i][i + colonne] == grille[ligne][colonne]) aligne++;
                else aligne = 0;
            }
        }
        if(aligne == 4) return true;
    }

    return false;
}


/**
 * @brief 
 * Fonction qui affiche la grille
 * 
 * @param grille 
 */
void afficheGrille(t_grille grille) 
{
    

    for(int i = 0; i < NB_COLONNES; i++) {
        printf("  %d ", i + 1);
    }
    printf("\n\n");

    for(int i = 0; i < NB_LIGNES; i++) {
        for(int j = 0; j < NB_COLONNES; j++) {
            printf("| "); 
            if(grille[i][j] == JOUEUR) 
                printf("%c", PION_A);
            else if(grille[i][j] == ADVERSAIRE) 
                printf("%c", PION_B);
            else 
                printf(" ");
            printf(" ");
        }
        printf("|\n");
        for(int j = 0; j < NB_COLONNES; j++)
            printf("+---");
        printf("+\n");
    }
}


/**
 * @brief 
 * Fonction qui regarde dans la grille 
 * entière si une victoire est présente
 * 
 * @param grille 
 * @return int 
 */
int endGames(t_grille grille){
    int winner=0;
    for(int j = 0; j < NB_COLONNES; j++){
        for(int i = NB_LIGNES - 1; i >= 0; i--){
            if(grille[i][j]!=VIDE){
                winner=estVainqueurOrdi(grille,i,j);
                
            }
            if(winner==JOUEUR || winner==ADVERSAIRE || winner==-1 ||winner==-2){
                return winner;
            }
        }
    }
    return winner;
    
}
/**
 * @brief 
 * 
 * Fonction qui permet en fonction de la colonne remplie ou pas
 * de trouver une ligne pour une colonne
 * 
 * @param t 
 * @param colonne 
 * @return int 
 */
int trouverLigne(t_grille t, int colonne) {
    for (int i = NB_LIGNES - 1; i >= 0; i--) {
        if (t[i][colonne] == VIDE) {
            return i;
        }
    }
    return -1; // Si la colonne est pleine
}
/**
 * @brief 
 * Fonction qui permet d'évaleur les alignements de pions
 * pour y attribuer des scores
 * 
 * 
 * @param grille 
 * @param ligne 
 * @param colonne 
 * @param delta_ligne 
 * @param delta_colonne 
 * @param joueur 
 * @return int 
 */
int evaluateSegment(t_grille grille, int ligne, int colonne, int delta_ligne, int delta_colonne, int joueur) {
    int score = 0;
    int joueurCount = 0;
    int adversaireCount = 0;
    int emptyCount = 0;

    for (int k = 0; k < 4; k++) {
        int currentRow = ligne + k * delta_ligne;
        int currentCol = colonne + k * delta_colonne;

        if (grille[currentRow][currentCol] == joueur) {
            joueurCount++;
        } else if (grille[currentRow][currentCol] == (joueur == JOUEUR ? ADVERSAIRE : JOUEUR)) {
            adversaireCount++;
        } else {
            emptyCount++;
        }
    }
    // Si l'ia a 4 pions d'alignés
    if (joueurCount == 4) {
        score += 300; 
    }
     else if (joueurCount == 3 && emptyCount == 1) {
        score += 10; 

    } else if (joueurCount == 2 && emptyCount == 2) {
        score += 5; 
    }

    if (adversaireCount == 3 && emptyCount == 1) {
        score -=200 ; 
    }
    else if(adversaireCount==2 && emptyCount>=2){
        score-=25;
    }

    return score;
}
/**
 * @brief 
 * 
 * @param grille 
 * @param joueur 
 * @return int 
 */
int scorePosition(t_grille grille, int joueur) {
    int score = 0;

    int central_col = NB_COLONNES / 2;
    for (int i = 0; i < NB_LIGNES; i++) {
        if (grille[i][central_col] == joueur) {
            score += 15; 

        }
    }

    //  horizontal 
    for (int i = 0; i < NB_LIGNES; i++) {
        for (int j = 0; j < NB_COLONNES - 3; j++) {
            int scoreSegment = evaluateSegment(grille, i, j, 0, 1, joueur); 
            score += scoreSegment;
        }
    }

    //  vertical 
    for (int i = 0; i < NB_LIGNES - 3; i++) {
        for (int j = 0; j < NB_COLONNES; j++) {
            int scoreSegment = evaluateSegment(grille, i, j, 1, 0, joueur); 
            score += scoreSegment;


        }
    }

    // Score diagonal alignments (descending)
    for (int i = 0; i < NB_LIGNES - 3; i++) {
        for (int j = 0; j < NB_COLONNES - 3; j++) {
            int scoreSegment = evaluateSegment(grille, i, j, 1, 1, joueur); 
            score += scoreSegment;


        }
    }

    // Score diagonal alignments (ascending)
    for (int i = 3; i < NB_LIGNES; i++) {
        for (int j = 0; j < NB_COLONNES - 3; j++) {
            int scoreSegment = evaluateSegment(grille, i, j, -1, 1, joueur);
            score += scoreSegment;

        }
    }

    return score;
}
/**
 * @brief 
 * Fonction qui gère le fonctionnement d'une partie contre l'ordinnateur
 * 
 * @param ordre 
 * @param grille 
 */
void joueurcontrOrdi(char ordre[100],t_grille grille){
    int  victoire =VIDE;
    int colonne;
    int joueur;
    int ligne;
    if(strcmp(ordre,"2")==0){
        joueur = ADVERSAIRE;
    }
    else{
        joueur = JOUEUR;
    }
    while(grillePleine(grille)==false && victoire==VIDE){
        if(joueur==ADVERSAIRE){
            colonne = coup_ordi(grille);
            ligne = trouverLigne(grille,colonne);

            placerPion(ligne,colonne,grille,joueur);
            if(estVainqueur(grille,ligne,colonne)){
                victoire=ADVERSAIRE;

            }
            else{
                joueur=JOUEUR;
            }
            
        }
        else{
            nettoyerEcran();
            afficheGrille(grille);
            colonne=choisirColonne(JOUEUR,grille);
            ligne  = trouverLigne(grille,colonne);
            while(ligne==-1){
                nettoyerEcran();
                afficheGrille(grille);
                printf("Veuillez choisir une colonne pas remplit\n");
                colonne = choisirColonne(JOUEUR,grille);
                ligne = trouverLigne(grille,colonne);
            }
            placerPion(ligne,colonne,grille,JOUEUR);
            if(estVainqueur(grille,ligne,colonne)){
                victoire = JOUEUR;
            }
            else{
                joueur=ADVERSAIRE;
            }
            

        }
    }
    nettoyerEcran();
    afficheGrille(grille);
    if(victoire==ADVERSAIRE){
        printf("C'est le robot qui a gagné, looser");
    }
    else if(victoire==JOUEUR){
        printf("Chaud tu as gagné");

    }
    else{
        printf("Egalité, c'est nul quand même");
    }
}


/**
 * @brief 
 * Fonction qui permet de placer un pion sur la grille
 * en fonction de la colonne
 * 
 * @param ligne 
 * @param colonne 
 * @param grille 
 * @param Pion 
 */
void placerPion(int ligne,int colonne,t_grille grille,int Pion){
   if(ligne!=-1){
        grille[ligne][colonne]=Pion;
   }
   
}
/**
 * @brief 
 * Permet de reconnaître si une saisie est un nombre ou pas
 * 
 * @param chaine 
 * @return true 
 * @return false 
 */
bool isInteger(char chaine[]){
    bool resultat=true;
    for(int i=0;chaine[i]!='\0';i++){
        if(!isdigit(chaine[i])){
            resultat = false;
            return resultat;
        

        }
    }
    return resultat;

}
/**
 * @brief 
 * Fonction qui permet à un joueur de choisir sa colonne 
 * vérification pour jouer
 * 
 * @param Pion 
 * @param grille 
 * @return int 
 */
int choisirColonne(int Pion,t_grille grille){
    char choix[100];
    printf("Veuillez choisir une colonne entre 1 et 7 : ");
    scanf("%s",choix);
    while(!isInteger(choix)){
        printf("Veuillez choisir une colonne correcte : ");
        scanf("%s",choix);
    }
    int colonne = atoi(choix);
    return colonne-1;


}

void joueurContreHumain(t_grille grille){
    int colonne;
    int ligne;
    int joueur=JOUEUR;
    int victoire=VIDE;
    while(grillePleine(grille)==false && victoire==VIDE){
        if(joueur==JOUEUR){
            nettoyerEcran();
            afficheGrille(grille);
            colonne=choisirColonne(JOUEUR,grille);
            ligne  = trouverLigne(grille,colonne);
            while(ligne==-1){
                nettoyerEcran();
                afficheGrille(grille);
                printf("Veuillez choisir une colonne pas remplit\n");
                colonne = choisirColonne(JOUEUR,grille);
                ligne = trouverLigne(grille,colonne);
            }
            placerPion(ligne,colonne,grille,JOUEUR);
            if(estVainqueur(grille,ligne,colonne)){
                victoire = JOUEUR;
            }
            else{
                joueur=ADVERSAIRE;
            }
        }
        else{
            nettoyerEcran();
            afficheGrille(grille);
            colonne=choisirColonne(JOUEUR,grille);
            ligne  = trouverLigne(grille,colonne);
            while(ligne==-1){
                nettoyerEcran();
                afficheGrille(grille);
                printf("Veuillez choisir une colonne pas remplit\n");
                colonne = choisirColonne(JOUEUR,grille);
                ligne = trouverLigne(grille,colonne);
            }
            placerPion(ligne,colonne,grille,ADVERSAIRE);
            if(estVainqueur(grille,ligne,colonne)){
                victoire = ADVERSAIRE;
            }
            else{
                joueur=JOUEUR;
            }
        }

            
    }
    nettoyerEcran();
    afficheGrille(grille);
    if(victoire==JOUEUR){
        printf("Joueur 1 tu as gagné, bien ouej petit canard, joueur 2 pas besoin de parler plus\n");
    }
    else if(victoire==ADVERSAIRE){
                printf("Joueur 2 tu as gagné, bien ouej bro, joueur 1 pas foutu de gagner alors que tu joues en premier\n");

    }
    else{
        printf("Egalite lekip\n");
    }
    
}
/**
 * @brief 
 * Menu de selection options pour Puissance4
 * 
 * @param choixAdervsaire 
 * @param choixOrdre 
 */
void menu(char choixAdervsaire[100],char choixOrdre[100]){
    
    printf("Bonjour bienvenu sur Puissance4 Interface Console\nChoississez votre adversaire:\n");
    printf("1 - Humain\n");
    printf("2 - Ordinateur(il détruit des carrières)\n");
    scanf("%s",choixAdervsaire);
    while(strcmp(choixAdervsaire,"1")!=0 && strcmp(choixAdervsaire,"2")!=0){
        printf("Veuillez faire un choix entre 1 et 2 comme demandé\n");
        printf("1 - Humain\n");
        printf("2 - Ordinateur(il détruit des carrières)\n");
        scanf("%s",choixAdervsaire);
    }
    nettoyerEcran();
    if(strcmp(choixAdervsaire,"2")==0){
        
        printf("Choississez entre 1 et 2 pour savoir qui doit jouer en premier : \n");
        printf("1 - vous\n");
        printf("2 - Robot\n");
        scanf("%s",choixOrdre);
        while(strcmp(choixOrdre,"1")!=0 && strcmp(choixOrdre,"2")!=0){
            printf("Option choisie Incorrecte\n");
            printf("Choississez entre 1 et 2 pour savoir qui doit jouer en premier : \n");
            printf("1 - vous\n");
            printf("2 - Robot\n");
            scanf("%s",choixOrdre);
        }
    }


    
}
/**
 * @brief 
 * Nettoyer l'ecran de jeu à chaque coup ou actions
 * 
 */
void nettoyerEcran(){
    system("@cls||clear");
}


/**
 * @brief 
 * Algorithme min-max pour Puissance4
 * donne des score à des situations de parties
 * 
 * @param t 
 * @param profondeur 
 * @param alpha 
 * @param beta 
 * @param Estmax 
 * @return int 
 */
int minmax(t_grille t, int profondeur, int alpha, int beta, bool Estmax) {
    int result = endGames(t);
    if(profondeur==0 || result==-2){
        return scorePosition(t,ADVERSAIRE);
    }
     else if (result == JOUEUR) {
        return -200; // L'IA perd
    } else if (result == ADVERSAIRE) {
        return 300; // L'IA gagne
    } else if (result == -1) {
        return 0; // Match nul ou profondeur atteinte
    }
    

    if (Estmax) {
        int meilleur_score = -1000;
        for (int j = 0; j < NB_COLONNES; j++) {
            int i = trouverLigne(t, j);
            if (i != -1) {
                t[i][j] = ADVERSAIRE;
                int score = minmax(t, profondeur - 1, alpha, beta, false);
                t[i][j] = VIDE;
                meilleur_score = score > meilleur_score ? score : meilleur_score;
                alpha = alpha > meilleur_score ? alpha : meilleur_score;
                if (alpha >= beta) break; 
            }
        }
        return meilleur_score;
    } else {
        int meilleur_score = 1000;
        for (int j = 0; j < NB_COLONNES; j++) {
            int i = trouverLigne(t, j);
            if (i != -1) {
                t[i][j] = JOUEUR;
                int score = minmax(t, profondeur - 1, alpha, beta, true);
                t[i][j] = VIDE;
                meilleur_score = score < meilleur_score ? score : meilleur_score;
                beta = beta < meilleur_score ? beta : meilleur_score;
                if (alpha >= beta) break; 
            }
        }
        return meilleur_score;
    }
}





/**
 * @brief 
 * Fonction qui retourne la colonne que veut jouer l'ordi
 * utilisation de l'algorithme min-max
 * 
 * @param t 
 * @return int 
 */
int coup_ordi(t_grille t) {
    int meilleur_score = INT_MIN;
    int meilleur_colonne = -1;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    // Ordre de priorisation des colonnes (centrales en premier)
    int colonnes[NB_COLONNES] = {3, 2, 4, 1, 5, 0, 6};
    for (int index = 0; index < NB_COLONNES; index++) {
        int j = colonnes[index];
        int i = trouverLigne(t, j);
        if (i != -1) { 
            t[i][j] = ADVERSAIRE;
            int score = minmax(t, 8, alpha, beta, false); 
            t[i][j] = VIDE;

            if (score > meilleur_score) {
                meilleur_score = score;
                meilleur_colonne = j;
            }
        }
    }

    return meilleur_colonne;
}


int main(int argc, char** argv){
    char choixAdversaire[100],choixOrdre[100];
    menu(choixAdversaire,choixOrdre);
    
    t_grille grille;
    initgrille(grille);
    if(strcmp(choixAdversaire,"1")==0){
        joueurContreHumain(grille);
    }
    else if(strcmp(choixAdversaire,"2")==0)
    {
        joueurcontrOrdi(choixOrdre,grille); 
    }
    return EXIT_SUCCESS;
}





