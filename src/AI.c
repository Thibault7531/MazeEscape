#include "AI.h"

#include <math.h>
#include <stdlib.h>

#include "../build/external/raylib-master/src/raylib.h"

AI createAI(int x, int y, Texture texture)
{
    AI ai = malloc(sizeof(struct AI));
    ai->x = x;
    ai->y = y;
    ai->texture = texture;
    ai->moving = false;
    Path path;
    path.length = 0;
    path.path = NULL;
    ai->path = path;
    return ai;
}

void destroyAI(AI ai)
{
    free(ai);
}

void updateAI(Maze maze, AI ai, float deltaTime)
{
    if (!ai->moving && ai->pathIndex < ai->path.length)
    {
        ai->currentCoord = ai->path.path[ai->pathIndex-1];
        ai->nextCoord = ai->path.path[ai->pathIndex];
        ai->moving = true;
        ai->pathIndex++;
    }

    if (ai->moving)
    {
        ai->x += (ai->nextCoord.x - ai->currentCoord.x) * deltaTime;
        ai->y += (ai->nextCoord.y - ai->currentCoord.y) * deltaTime;
        if (fabs(ai->x - ai->nextCoord.x) < .01f && fabs(ai->y - ai->nextCoord.y) < .01f)
        {
            ai->x = ai->nextCoord.x;
            ai->y = ai->nextCoord.y;
            ai->moving = false;
        }
        if (fabs(ai->x - ai->nextCoord.x) > 1 && fabs(ai->y - ai->nextCoord.y) < 1)
        {
            ai->x = ai->nextCoord.x;
            ai->y = ai->nextCoord.y;
            ai->moving = false;
        }
    }
}

void setPath(AI ai, Path path)
{
    if (ai->path.length != 0)
    {
        ai->x = ai->nextCoord.x;
        ai->y = ai->nextCoord.y;
    }
    ai->moving = false;
    ai->path = path;
    ai->pathIndex = 1;
}

void getNextPosition(AI ai, int* xOut, int* yOut)
{
    *xOut = ai->nextCoord.x;
    *yOut = ai->nextCoord.y;
}

void renderAI(Maze maze, AI ai)
{
    Vector2 startCoord = {290, 10};
    int nodeSize = 700 / maze->size;

    Vector2 pos = {startCoord.x + ai->x * nodeSize + 8, startCoord.y + ai->y * nodeSize + 8};
    DrawTextureEx(ai->texture, pos, 0, (nodeSize-16.f)/32, WHITE);
}
