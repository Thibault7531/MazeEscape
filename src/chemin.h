#ifndef CHEMIN_H
#define CHEMIN_H
#include "Maze.h"

typedef struct {
    int x, y;
} Coord;

int trouverCheminAStar(Maze maze, Coord *cheminResultat);

void renderPath(Maze maze, Coord *path, int pathLength);

#endif