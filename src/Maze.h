#pragma once

#include <stdbool.h>
#include <raylib.h>
#define TOPWALL (1<<3)
#define LEFTWALL (1<<2)
#define RIGHTWALL (1<<1)
#define BOTTOMWALL (1<<0)

struct MazeNode
{
    unsigned int walls;
};
typedef struct MazeNode* MazeNode;

struct Maze
{
    int size;
    MazeNode* nodes;
    int entryPointX, entryPointY, entryPointSide;
    int exitPointX, exitPointY, exitPointSide;
};
typedef struct Maze* Maze;

Maze createEmptyMaze(int size);
Maze createRandomMaze(int size);
void destroyMaze(Maze maze);

void addBorders(Maze maze);
void addStartArea(Maze maze);
void addRandomStartAreaExits(Maze maze);
void addRandomExit(Maze maze);
void addRandomWall(Maze maze);

MazeNode getMazeNode(Maze maze, int x, int y);
void setNodeWallsUnsafe(Maze maze, int x, int y, int node);
int getNodeWalls(Maze maze, int x, int y);
void setNodeWall(Maze maze, int x, int y, bool enable, int wall);
int getMazeSize(Maze maze);
int getEntryPointX(Maze maze);
int getEntryPointY(Maze maze);
int getEntryPointSide(Maze maze);
int getExitPointX(Maze maze);
int getExitPointY(Maze maze);
int getExitPointSide(Maze maze);

bool isWallTop(MazeNode node);
bool isWallLeft(MazeNode node);
bool isWallRight(MazeNode node);
bool isWallBottom(MazeNode node);
void setWallTop(MazeNode node, bool enable);
void setWallLeft(MazeNode node, bool enable);
void setWallRight(MazeNode node, bool enable);
void setWallBottom(MazeNode node, bool enable);

void printMaze(Maze maze);
void renderMaze(Maze maze);