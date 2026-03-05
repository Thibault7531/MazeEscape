#pragma once

#include <stdbool.h>

#define TOPWALL (1<<3)
#define LEFTWALL (1<<2)
#define RIGHTWALL (1<<1)
#define BOTTOMWALL (1<<0)

typedef struct MazeNode
{
    unsigned int walls;
} MazeNode;

struct Maze
{
    int size;
    MazeNode** nodes;
};
typedef struct Maze* Maze;

Maze createMaze(int size);
void destroyMaze(Maze maze);
MazeNode* getMazeNode(Maze maze, int x, int y);
void setNodeWalls(Maze maze, int x, int y, int node);

bool isWallTop(MazeNode* node);
bool isWallLeft(MazeNode* node);
bool isWallRight(MazeNode* node);
bool isWallBottom(MazeNode* node);
void setWallTop(MazeNode* node, bool enable);
void setWallLeft(MazeNode* node, bool enable);
void setWallRight(MazeNode* node, bool enable);
void setWallBottom(MazeNode* node, bool enable);

void printMaze(Maze maze);