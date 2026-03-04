#define MAZE_H
#define LARGEUR 40 //on fixe la taille du tableau
#define HAUTEUR 40 
#define HAUT 1
#define DROITE 2
#define BAS 4
#define GAUCHE 8 
//#define MUR 5
//#define VIDE 6

typedef struct{
    int lab[LARGEUR][HAUTEUR];
    int sortieX, sortieY; //coordonnés de la sortie
    int départX,départY; // coordonnés du départ
}Labyrinthe;
void creer_labyrinthe(Labyrinthe *l);