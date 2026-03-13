#pragma once

#include "Maze.h"
#include "Path.h"
#include "raylib.h"

struct AI
{
    float x, y;
    Path path;
    Coord currentCoord;
    Coord nextCoord;
    bool moving;
    int pathIndex;
    Texture texture;
};
typedef struct AI* AI;

AI createAI(int x, int y, Texture texture);
void destroyAI(AI ai);

void updateAI(Maze maze, AI ai, float deltaTime);

void setPath(AI ai, Path path);

void renderAI(Maze maze, AI ai);