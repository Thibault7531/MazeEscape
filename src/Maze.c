#include "Maze.h"

#include <assert.h>
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

    srand(time(NULL));

    for(int x=0; x < maze->size; x++){ // on fait les bordures
        for(int y=0; y < maze->size; y++){
            MazeNode* node = getMazeNode(maze, x, y);

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

    for(int i=0; i < 3; i++){
        int x = rand() % maze->size;
        int y = rand() % maze->size;
        MazeNode* node = getMazeNode(maze, x, y);
        setWallTop(node, true);

        x = rand() % maze->size;
        y = rand() % maze->size;
        node = getMazeNode(maze, x, y);
        setWallBottom(node, true);

        x = rand() % maze->size;
        y = rand() % maze->size;
        node = getMazeNode(maze, x, y);
        setWallLeft(node, true);

        x = rand() % maze->size;
        y = rand() % maze->size;
        node = getMazeNode(maze, x, y);
        setWallRight(node, true);
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

MazeNode* getMazeNode(Maze maze, int x, int y)
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

bool isWallTop(MazeNode* node)
{
    return node->walls & TOPWALL;
}

bool isWallLeft(MazeNode* node)
{
    return node->walls & LEFTWALL;
}

bool isWallRight(MazeNode* node)
{
    return node->walls & RIGHTWALL;
}

bool isWallBottom(MazeNode* node)
{
    return node->walls & BOTTOMWALL;
}

void setWallTop(MazeNode* node, bool enable)
{
    if (enable) node->walls |= TOPWALL;
    else node->walls &= ~TOPWALL;
}

void setWallLeft(MazeNode* node, bool enable)
{
    if (enable) node->walls |= LEFTWALL;
    else node->walls &= ~LEFTWALL;
}

void setWallRight(MazeNode* node, bool enable)
{
    if (enable) node->walls |= RIGHTWALL;
    else node->walls &= ~RIGHTWALL;
}

void setWallBottom(MazeNode* node, bool enable)
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