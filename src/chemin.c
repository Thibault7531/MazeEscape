#include "chemin.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define INF 9999


typedef struct {
    int g;         
    int f;          
    int px, py;     
    bool ouvert;    
    bool ferme;     
} NodeIA;


int distance(int x1, int y1, int x2, int y2) {
    return abs(x1-x2)+abs(y1-y2);
}

int trouverCheminAStar(Labyrinthe *l, Coord *cheminResultat) {
    NodeIA grille[LARGEUR][HAUTEUR];
    
    for(int x=0;x<LARGEUR;x++) {
        for(int y=0;y<HAUTEUR;y++) {
            grille[x][y]=(NodeIA){INF,INF,-1,-1,false,false};
        }
    }

    int sx=l->départX;
    int sy=l->départY;
    grille[sx][sy].g=0;
    grille[sx][sy].f=distance(sx,sy,l->sortieX,l->sortieY);
    grille[sx][sy].ouvert=true;

    while(true) {
        int curX=-1,curY=-1;
        int minF=INF;

        for(int x=0;x<LARGEUR;x++) {
            for(int y=0;y<HAUTEUR;y++) {
                if(grille[x][y].ouvert && grille[x][y].f<minF) {
                    minF=grille[x][y].f;
                    curX=x; 
                    curY=y;
                }
            }
        }

        if(curX==-1){
            return 0;
        } 
        if(curX==l->sortieX && curY == l->sortieY){
            break;
        }

        grille[curX][curY].ouvert=false;
        grille[curX][curY].ferme=true;

        int murs=l->lab[curX][curY];

        int dx[]={0, 0, -1, 1};
        int dy[]={-1, 1, 0, 0};
        int flags[]={HAUT, BAS, GAUCHE, DROITE};

        for(int i=0; i<4; i++) {
            if(!(murs & flags[i])) { 
                int nx = curX + dx[i];
                int ny = curY + dy[i];

                if(nx >= 0 && nx < LARGEUR && ny >= 0 && ny < HAUTEUR && !grille[nx][ny].ferme) {
                    int nouveauG = grille[curX][curY].g + 1;

                    if(nouveauG < grille[nx][ny].g) {
                        grille[nx][ny].px = curX;
                        grille[nx][ny].py = curY;
                        grille[nx][ny].g = nouveauG;
                        grille[nx][ny].f =nouveauG+distance(nx, ny, l->sortieX, l->sortieY);
                        grille[nx][ny].ouvert = true;
                    }
                }
            }
        }
    }

    int longueur = grille[l->sortieX][l->sortieY].g;
    int tempX = l->sortieX;
    int tempY = l->sortieY;

    for(int i = longueur; i >= 0; i--) {
        cheminResultat[i] = (Coord){tempX, tempY};
        int nextX = grille[tempX][tempY].px;
        int nextY = grille[tempX][tempY].py;
        tempX = nextX;
        tempY = nextY;
    }

    return longueur + 1; 
}