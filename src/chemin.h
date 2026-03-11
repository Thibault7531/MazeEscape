#ifndef CHEMIN_H
#define CHEMIN_H

#include "lab.h" 

typedef struct {
    int x, y;
} Coord;

int trouverCheminAStar(Labyrinthe *l, Coord *cheminResultat);

#endif