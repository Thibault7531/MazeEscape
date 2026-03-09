#include "Maze.h"

#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Maze createEmptyMaze(int size)
{
    Maze maze = calloc(1, sizeof(struct Maze));
    maze->size = size;
    maze->nodes = calloc(size * size, sizeof(MazeNode*));
    for (int i = 0; i < size * size; i++)
    {
        maze->nodes[i] = calloc(1, sizeof(MazeNode));
        maze->nodes[i]->walls = 0;
    }
    return maze;
}

Maze createRandomMaze(int size)
{
    Maze maze = createEmptyMaze(size);
    int startAreaX = size/2-1;
    int startAreaY = size/2-1;
    int startAreaSize = size%2 == 0 ? 2 : 3;

    srand(time(NULL));

    addBorders(maze);
    addStartArea(maze);
    addRandomStartAreaExits(maze);
    addRandomExit(maze);

    for (int i = 0; i < pow(maze->size*.65f, 2); i++)
    {
        addRandomWall(maze);
    }

    return maze;
}

void destroyMaze(Maze maze)
{
    for (int i = 0; i < maze->size * maze->size; i++)
    {
        free(maze->nodes[i]);
        maze->nodes[i] = NULL;
    }
    free(maze->nodes);
    free(maze);
}

void addBorders(Maze maze)
{
    for(int x=0; x < maze->size; x++){ // on fait les bordures
        for(int y=0; y < maze->size; y++){
            MazeNode node = getMazeNode(maze, x, y);

            if(y==0){
                setWallTop(node, true);
            }
            if(y == maze->size-1){
                setWallBottom(node, true);
            }
            if(x == 0){
                setWallLeft(node, true);
            }
            if(x == maze->size-1){
                setWallRight(node, true);
            }
        }
    }
}

void addStartArea(Maze maze)
{
    int startAreaX = maze->size/2-1;
    int startAreaY = maze->size/2-1;
    int startAreaSize = maze->size%2 == 0 ? 2 : 3;

    for (int y = 0; y < startAreaSize; y++)
    {
        for (int x = 0; x < startAreaSize; x++)
        {
            MazeNode node = getMazeNode(maze, startAreaX + x, startAreaY + y);
            if (x == 0) setWallLeft(node, true);
            if (x == startAreaSize - 1) setWallRight(node, true);
            if (y == 0) setWallTop(node, true);
            if (y == startAreaSize - 1) setWallBottom(node, true);
        }
    }
}

void addRandomStartAreaExits(Maze maze)
{
    int startAreaX = maze->size/2-1;
    int startAreaY = maze->size/2-1;
    int startAreaSize = maze->size%2 == 0 ? 2 : 3;

    int startPos = rand() % (startAreaSize*4);
    switch (startPos/startAreaSize)
    {
    case 0:
        {
            MazeNode node = getMazeNode(maze, startAreaX + startPos%startAreaSize, startAreaY);
            setWallTop(node, false);
            break;
        }

    case 1:
        {
            MazeNode node = getMazeNode(maze, startAreaX + startAreaSize - 1, startAreaY + startPos%startAreaSize);
            setWallRight(node, false);
            break;
        }

    case 2:
        {
            MazeNode node = getMazeNode(maze, startAreaX + startAreaSize - startPos%startAreaSize - 1, startAreaY + startAreaSize - 1);
            setWallBottom(node, false);
            break;
        }

    case 3:
        {
            MazeNode node = getMazeNode(maze, startAreaX, startAreaY + startAreaSize - startPos%startAreaSize - 1);
            setWallLeft(node, false);
            break;
        }

    default: break;
    }
}

void addRandomExit(Maze maze)
{
    int endPos = rand() % (maze->size*4);
    switch (endPos/maze->size)
    {
    case 0:
        {
            MazeNode node = getMazeNode(maze, endPos%maze->size, 0);
            setWallTop(node, false);
            break;
        }

    case 1:
        {
            MazeNode node = getMazeNode(maze, maze->size - 1, endPos%maze->size);
            setWallRight(node, false);
            break;
        }

    case 2:
        {
            MazeNode node = getMazeNode(maze, maze->size - 1 - endPos%maze->size, maze->size - 1);
            setWallBottom(node, false);
            break;
        }

    case 3:
        {
            MazeNode node = getMazeNode(maze, 0, maze->size - 1 - endPos%maze->size);
            setWallLeft(node, false);
            break;
        }

    default: break;
    }
}

void addRandomWall(Maze maze)
{
    int startAreaX = maze->size/2-1;
    int startAreaY = maze->size/2-1;
    int startAreaSize = maze->size%2 == 0 ? 2 : 3;

    int x = rand() % maze->size;
    int y = rand() % maze->size;
    int side = rand() % 4;
    switch (side)
    {
    case 0:
        {
            while (y == 0 || (x >= startAreaX && x < startAreaX + startAreaSize && y >= startAreaY && y <= startAreaY + startAreaSize) || isWallTop(getMazeNode(maze, x, y)) || isWallBottom(getMazeNode(maze, x, y-1)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            MazeNode node = getMazeNode(maze, x, y);
            setWallTop(node, true);
            break;
        }

    case 1:
        {
            while (x == maze->size - 1 || (x >= startAreaX - 1 && x < startAreaX + startAreaSize && y >= startAreaY && y < startAreaY + startAreaSize) || isWallRight(getMazeNode(maze, x, y)) || isWallLeft(getMazeNode(maze, x+1, y)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            MazeNode node = getMazeNode(maze, x, y);
            setWallRight(node, true);
            break;
        }

    case 2:
        {
            while (y == maze->size - 1 || (x >= startAreaX && x < startAreaX + startAreaSize && y >= startAreaY - 1 && y < startAreaY + startAreaSize) || isWallBottom(getMazeNode(maze, x, y)) || isWallTop(getMazeNode(maze, x, y+1)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            MazeNode node = getMazeNode(maze, x, y);
            setWallBottom(node, true);
            break;
        }

    case 3:
        {
            while (x == 0 || (x >= startAreaX && x <= startAreaX + startAreaSize && y >= startAreaY && y < startAreaY + startAreaSize) || isWallLeft(getMazeNode(maze, x, y)) || isWallRight(getMazeNode(maze, x-1, y)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            MazeNode node = getMazeNode(maze, x, y);
            setWallLeft(node, true);
            break;
        }

    default: break;
    }
}

MazeNode getMazeNode(Maze maze, int x, int y)
{
    assert(x >= 0 && x < maze->size);
    assert(y >= 0 && y < maze->size);
    return maze->nodes[x+y*maze->size];
}

void setNodeWalls(Maze maze, int x, int y, int walls)
{
    assert(x >= 0 && x < maze->size);
    assert(y >= 0 && y < maze->size);
    maze->nodes[x+y*maze->size]->walls = walls;
}

int getMazeSize(Maze maze)
{
    return maze->size;
}

bool isWallTop(MazeNode node)
{
    return node->walls & TOPWALL;
}

bool isWallLeft(MazeNode node)
{
    return node->walls & LEFTWALL;
}

bool isWallRight(MazeNode node)
{
    return node->walls & RIGHTWALL;
}

bool isWallBottom(MazeNode node)
{
    return node->walls & BOTTOMWALL;
}

void setWallTop(MazeNode node, bool enable)
{
    if (enable) node->walls |= TOPWALL;
    else node->walls &= ~TOPWALL;
}

void setWallLeft(MazeNode node, bool enable)
{
    if (enable) node->walls |= LEFTWALL;
    else node->walls &= ~LEFTWALL;
}

void setWallRight(MazeNode node, bool enable)
{
    if (enable) node->walls |= RIGHTWALL;
    else node->walls &= ~RIGHTWALL;
}

void setWallBottom(MazeNode node, bool enable)
{
    if (enable) node->walls |= BOTTOMWALL;
    else node->walls &= ~BOTTOMWALL;
}

void printMaze(Maze maze)
{
    for (int y = 0; y < maze->size; y++)
    {
        for (int x = 0; x < maze->size; x++)
        {
            printf(".");
            if (isWallTop(getMazeNode(maze, x, y))) printf("-");
            else printf(" ");
            printf(".");
        }
        printf("\n");
        for (int x = 0; x < maze->size; x++)
        {
            if (isWallLeft(getMazeNode(maze, x, y))) printf("|");
            else printf(" ");
            printf(" ");
            if (isWallRight(getMazeNode(maze, x, y))) printf("|");
            else printf(" ");
        }
        printf("\n");
        for (int x = 0; x < maze->size; x++)
        {
            printf(".");
            if (isWallBottom(getMazeNode(maze, x, y))) printf("-");
            else printf(" ");
            printf(".");
        }
        printf("\n");
    }
}

void renderMaze(Maze maze)
{
    int nodeSize = 700 / maze->size;
    float lineThickness = .2f * nodeSize;
    Vector2 startCoord = {290, 10};

    for (int y = 0; y < maze->size; y++)
    {
        for (int x = 0; x < maze->size; x++)
        {
            MazeNode node = getMazeNode(maze, x, y);
            if (isWallTop(node))
            {
                Vector2 lineStart = {startCoord.x + x * nodeSize - lineThickness/2, startCoord.y + y * nodeSize};
                Vector2 lineEnd = {lineStart.x + nodeSize + lineThickness, lineStart.y};
                DrawLineEx(lineStart, lineEnd, lineThickness, WHITE);
            }
            if (isWallLeft(node))
            {
                Vector2 lineStart = {startCoord.x + x * nodeSize, startCoord.y + y * nodeSize - lineThickness/2};
                Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize + lineThickness};
                DrawLineEx(lineStart, lineEnd, lineThickness, WHITE);
            }
            if (isWallRight(node))
            {
                Vector2 lineStart = {startCoord.x + (x + 1) * nodeSize, startCoord.y + y * nodeSize - lineThickness/2};
                Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize + lineThickness};
                DrawLineEx(lineStart, lineEnd, lineThickness, WHITE);
            }
            if (isWallBottom(node))
            {
                Vector2 lineStart = {startCoord.x + x * nodeSize - lineThickness/2, startCoord.y + (y + 1) * nodeSize};
                Vector2 lineEnd = {lineStart.x + nodeSize + lineThickness, lineStart.y};
                DrawLineEx(lineStart, lineEnd, lineThickness, WHITE);
            }
        }
    }
}
