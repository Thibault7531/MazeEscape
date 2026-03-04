#include "lab.h"

//création d'un carré vide qu'on va transformer en labyrinthe
void creer_labyrinthe(Labyrinthe *l){
    //création d'un carré vide
    int x,y,i,n;
    for(x=0;x<LARGEUR;x++){
        for(y=0;y<HAUTEUR;y++){
            l->lab[x][y]=0; //initialisation des caes à 0
        }
    }

    for(x=0;x<LARGEUR;x++){ // on fait les bordures
        for(y=0;y<HAUTEUR;y++){
            if(y==0){
                l->lab[x][y]|=HAUT;               
            }
            if(y==HAUTEUR-1){
                l->lab[x][y]|=BAS;
            }
            if(x==0){
                l->lab[x][y]|=GAUCHE;
            }
            if(x==LARGEUR-1){
                l->lab[x][y]|=DROITE;
            }
        }
    }
    //on place des murs de manière aléatoire
    for(i=0;i<3;i++){
        x=rand()%40;
        y=rand()%40;
        l->lab[x][y]|=HAUT;
        x=rand()%40;
        y=rand()%40;
        l->lab[x][y]|=BAS;
        x=rand()%40;
        y=rand()%40;
        l->lab[x][y]|=GAUCHE;
        x=rand()%40;
        y=rand()%40;
        l->lab[x][y]|=DROITE;
    }
    //on place le point de départ de l'IA
    l->départX=0;
    l->départY=0;
    //on place la sortie de manière aléatoire
    l->sortieX=39;
    l->sortieY=39;
    //placement de l'IA au milieu
    l->départX=rand()%40;
    l->départY=rand()%40;
}