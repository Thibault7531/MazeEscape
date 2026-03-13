#ifndef CHEMIN_H
#define CHEMIN_H
#include "Maze.h"

typedef struct {
    int x, y;
} Coord;

typedef struct
{
    int length;
    Coord* path;
} Path;

Path FindPathAStar(Maze maze, int fromX, int fromY);

void renderPath(Maze maze, Path path);

#endif