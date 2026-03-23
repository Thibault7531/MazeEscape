#include "AI.h"

#include <math.h>
#include <stdlib.h>

#include "../build/external/raylib-master/src/raylib.h"

AI createAI(int x, int y)
{
    AI ai = malloc(sizeof(struct AI));
    ai->x = x;
    ai->y = y;
    ai->texture = LoadTexture("wabbit_alpha.png");
    ai->moving = false;
    Path path;
    path.length = 0;
    path.path = NULL;
    ai->path = path;
    return ai;
}

void destroyAI(AI ai)
{
    UnloadTexture(ai->texture);
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

        if (ai->nextCoord.x < ai->currentCoord.x && ai->nextCoord.x > ai->x)
        {
            ai->x = ai->nextCoord.x;
            ai->y = ai->nextCoord.y;
            ai->moving = false;
        }
        if (ai->nextCoord.x > ai->currentCoord.x && ai->nextCoord.x < ai->x)
        {
            ai->x = ai->nextCoord.x;
            ai->y = ai->nextCoord.y;
            ai->moving = false;
        }
        if (ai->nextCoord.y < ai->currentCoord.y && ai->nextCoord.y > ai->y)
        {
            ai->x = ai->nextCoord.x;
            ai->y = ai->nextCoord.y;
            ai->moving = false;
        }
        if (ai->nextCoord.y > ai->currentCoord.y && ai->nextCoord.y < ai->y)
        {
            ai->x = ai->nextCoord.x;
            ai->y = ai->nextCoord.y;
            ai->moving = false;
        }
    }
}

void setPath(AI ai, Path path)
{
    ai->path = path;
    ai->pathIndex = 1;
}

void getCurrentPosition(AI ai, int* xOut, int* yOut)
{
    *xOut = ai->currentCoord.x;
    *yOut = ai->currentCoord.y;
}

void getNextPosition(AI ai, int* xOut, int* yOut)
{
    *xOut = ai->nextCoord.x;
    *yOut = ai->nextCoord.y;
}

bool isAIStuck(AI ai)
{
    return ai->path.length == 0;
}

void renderAI(Maze maze, AI ai)
{
    Vector2 startCoord = {290, 10};
    int nodeSize = 700 / maze->size;

    Vector2 pos = {startCoord.x + ai->x * nodeSize + 8, startCoord.y + ai->y * nodeSize + 8};
    DrawTextureEx(ai->texture, pos, 0, (nodeSize-16.f)/32, WHITE);
}
