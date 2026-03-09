#ifndef GRAPH_H
#define GRAPH_H

#include "Maze.h"

typedef struct {
    int x, y;
} NodeCoord;

typedef struct {
    NodeCoord neighbors[4];
    int count;
} NodeAdjacency;

NodeAdjacency getAccessibleNeighbors(Maze maze, int x, int y);

#endif