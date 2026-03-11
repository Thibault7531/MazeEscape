#include "chemin.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "../build/external/raylib-master/src/raylib.h"

#define INF 9999

#define LARGEUR 10
#define HAUTEUR 10


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

int trouverCheminAStar(Maze maze, Coord *cheminResultat) {
    //TODO: Refactor code to achieve higher standards
    NodeIA grille[LARGEUR][HAUTEUR];
    
    for(int x=0;x<LARGEUR;x++) {
        for(int y=0;y<HAUTEUR;y++) {
            grille[x][y]=(NodeIA){INF,INF,-1,-1,false,false};
        }
    }

    int sx=getEntryPointX(maze);
    int sy=getEntryPointY(maze);
    grille[sx][sy].g=0;
    grille[sx][sy].f=distance(sx,sy, getExitPointX(maze),getEntryPointY(maze));
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
        if(curX==getExitPointX(maze) && curY == getExitPointY(maze)){
            break;
        }

        grille[curX][curY].ouvert=false;
        grille[curX][curY].ferme=true;

        int murs = getNodeWalls(maze, curX, curY);

        int dx[]={0, 0, -1, 1};
        int dy[]={-1, 1, 0, 0};
        int flags[]={TOPWALL, BOTTOMWALL, LEFTWALL, RIGHTWALL};

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
                        grille[nx][ny].f =nouveauG+distance(nx, ny, getExitPointX(maze), getExitPointY(maze));
                        grille[nx][ny].ouvert = true;
                    }
                }
            }
        }
    }

    int longueur = grille[getExitPointX(maze)][getExitPointY(maze)].g;
    int tempX = getExitPointX(maze);
    int tempY = getExitPointY(maze);

    for(int i = longueur; i >= 0; i--) {
        cheminResultat[i] = (Coord){tempX, tempY};
        int nextX = grille[tempX][tempY].px;
        int nextY = grille[tempX][tempY].py;
        tempX = nextX;
        tempY = nextY;
    }

    return longueur + 1; 
}

void renderPath(Maze maze, Coord* path, int pathLength)
{
    Coord currentCoord = path[0];
    int nodeSize = 700 / maze->size;
    float lineThickness = .2f * nodeSize;
    Vector2 startCoord = {290, 10};

    for (int i = 1; i < pathLength; i++)
    {
        Coord nextCoord = path[i];
        Vector2 lineStart = {startCoord.x + (currentCoord.x + .5f) * nodeSize, startCoord.y + (currentCoord.y + .5f) * nodeSize};
        Vector2 lineEnd = {startCoord.x + (nextCoord.x + .5f) * nodeSize, startCoord.y + (nextCoord.y + .5f) * nodeSize};
        DrawLineEx(lineStart, lineEnd, lineThickness, RED);
        currentCoord = path[i];
    }
}
