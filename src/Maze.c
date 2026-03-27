#include "Maze.h"

#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Texture2D Grass;

Maze createEmptyMaze(int size)
{
    if (!IsTextureValid(Grass))
    {
        Image image = LoadImage("GRASS+.png");
        ImageCrop(&image, (Rectangle){8 * 16, 2 * 16, 16, 16});
        Grass = LoadTextureFromImage(image);
        UnloadImage(image);
    }

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

    maze->startAreaX = maze->size/2-1;
    maze->startAreaY = maze->size/2-1;
    maze->startAreaSizeX = maze->size%2 == 0 ? 2 : 3;
    maze->startAreaSizeY = maze->size%2 == 0 ? 2 : 3;

    addBorders(maze);
    addStartArea(maze);
    addRandomStartAreaExits(maze);
    addRandomExit(maze);

    for (int i = 0; i < pow(maze->size*.75f, 2); i++)
    {
        addRandomWall(maze);
    }

    return maze;
}

Maze createMenuMaze()
{
    Maze maze = createEmptyMaze(20);

    srand(time(NULL));

    maze->startAreaX = 4;
    maze->startAreaY = 6;
    maze->startAreaSizeX = 12;
    maze->startAreaSizeY = 8;

    addBorders(maze);
    addStartArea(maze);
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
    for (int y = 0; y < maze->startAreaSizeY; y++)
    {
        for (int x = 0; x < maze->startAreaSizeX; x++)
        {
            if (x == 0) setNodeWall(maze, maze->startAreaX + x, maze->startAreaY + y, true, LEFTWALL);
            if (x == maze->startAreaSizeX - 1) setNodeWall(maze, maze->startAreaX + x, maze->startAreaY + y, true, RIGHTWALL);
            if (y == 0) setNodeWall(maze, maze->startAreaX + x, maze->startAreaY + y, true, TOPWALL);
            if (y == maze->startAreaSizeY - 1) setNodeWall(maze, maze->startAreaX + x, maze->startAreaY + y, true, BOTTOMWALL);
        }
    }
}

void addMenuArea(Maze maze, int x, int y, int sizeX, int sizeY)
{
    for (int y2 = 0; y2 < sizeY; y2++)
    {
        for (int x2 = 0; x2 < sizeX; x2++)
        {
            if (x2 == 0) setNodeWall(maze, x + x2, y + y2, true, LEFTWALL);
            if (x2 == sizeX - 1) setNodeWall(maze, x + x2, y + y2, true, RIGHTWALL);
            if (y2 == 0) setNodeWall(maze, x + x2, y + y2, true, TOPWALL);
            if (y2 == sizeY - 1) setNodeWall(maze, x + x2, y + y2, true, BOTTOMWALL);
        }
    }
}

void addRandomStartAreaExits(Maze maze)
{
    int startPos = rand() % (maze->startAreaSizeX*4);
    switch (startPos/maze->startAreaSizeX)
    {
    case 0:
        {
            maze->entryPointX = maze->startAreaX + startPos%maze->startAreaSizeX;
            maze->entryPointY = maze->startAreaY;
            maze->entryPointSide = TOPWALL;
            break;
        }

    case 1:
        {
            maze->entryPointX = maze->startAreaX + maze->startAreaSizeX - 1;
            maze->entryPointY = maze->startAreaY + startPos%maze->startAreaSizeX;
            maze->entryPointSide = RIGHTWALL;
            break;
        }

    case 2:
        {
            maze->entryPointX = maze->startAreaX + maze->startAreaSizeX - startPos%maze->startAreaSizeX - 1;
            maze->entryPointY = maze->startAreaY + maze->startAreaSizeX - 1;
            maze->entryPointSide = BOTTOMWALL;
            break;
        }

    case 3:
        {
            maze->entryPointX = maze->startAreaX;
            maze->entryPointY = maze->startAreaY + maze->startAreaSizeX - 1 - startPos%maze->startAreaSizeX;
            maze->entryPointSide = LEFTWALL;
            break;
        }

    default: return;
    }

    setNodeWall(maze, maze->entryPointX, maze->entryPointY, false, maze->entryPointSide);
}

void addRandomExit(Maze maze)
{
    int endPos = rand() % (maze->size*4);
    switch (endPos/maze->size)
    {
    case 0:
        {
            maze->exitPointX = endPos%maze->size;
            maze->exitPointY = 0;
            maze->exitPointSide = TOPWALL;
            break;
        }

    case 1:
        {
            maze->exitPointX = maze->size - 1;
            maze->exitPointY = endPos%maze->size;
            maze->exitPointSide = RIGHTWALL;
            break;
        }

    case 2:
        {
            maze->exitPointX = maze->size - 1 - endPos%maze->size;
            maze->exitPointY = maze->size - 1;
            maze->exitPointSide = BOTTOMWALL;
            break;
        }

    case 3:
        {
            maze->exitPointX = 0;
            maze->exitPointY = maze->size - 1 - endPos%maze->size;
            maze->exitPointSide = LEFTWALL;
            break;
        }

    default: break;
    }
    setNodeWall(maze, maze->exitPointX, maze->exitPointY, false, maze->exitPointSide);
}

void addRandomWall(Maze maze)
{
    int x = rand() % maze->size;
    int y = rand() % maze->size;
    int side = rand() % 4;
    switch (side)
    {
    case 0:
        {
            while (y == 0 || (x >= maze->startAreaX && x < maze->startAreaX + maze->startAreaSizeX && y >= maze->startAreaY && y <= maze->startAreaY + maze->startAreaSizeY) || isWallTop(getMazeNode(maze, x, y)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            setNodeWall(maze, x, y, true, TOPWALL);
            break;
        }

    case 1:
        {
            while (x == maze->size - 1 || (x >= maze->startAreaX - 1 && x < maze->startAreaX + maze->startAreaSizeX && y >= maze->startAreaY && y < maze->startAreaY + maze->startAreaSizeY) || isWallRight(getMazeNode(maze, x, y)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            setNodeWall(maze, x, y, true, RIGHTWALL);
            break;
        }

    case 2:
        {
            while (y == maze->size - 1 || (x >= maze->startAreaX && x < maze->startAreaX + maze->startAreaSizeX && y >= maze->startAreaY - 1 && y < maze->startAreaY + maze->startAreaSizeY) || isWallBottom(getMazeNode(maze, x, y)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            setNodeWall(maze, x, y, true, BOTTOMWALL);
            break;
        }

    case 3:
        {
            while (x == 0 || (x >= maze->startAreaX && x <= maze->startAreaX + maze->startAreaSizeX && y >= maze->startAreaY && y < maze->startAreaY + maze->startAreaSizeY) || isWallLeft(getMazeNode(maze, x, y)))
            {
                x = rand() % maze->size;
                y = rand() % maze->size;
            }
            setNodeWall(maze, x, y, true, LEFTWALL);
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

void setNodeWallsUnsafe(Maze maze, int x, int y, int walls)
{
    assert(x >= 0 && x < maze->size);
    assert(y >= 0 && y < maze->size);
    maze->nodes[x+y*maze->size]->walls = walls;
}

int getNodeWalls(Maze maze, int x, int y)
{
    assert(x >= 0 && x < maze->size);
    assert(y >= 0 && y < maze->size);
    return maze->nodes[x+y*maze->size]->walls;
}

void setNodeWall(Maze maze, int x, int y, bool enable, int wall)
{
    if (enable)
    {
        MazeNode node = getMazeNode(maze, x, y);
        setNodeWallsUnsafe(maze, x, y, node->walls | wall);

        switch (wall)
        {
        case TOPWALL:
            {
                if (y > 0) setNodeWallsUnsafe(maze, x, y-1, getMazeNode(maze, x, y-1)->walls | BOTTOMWALL);
                break;
            }

        case BOTTOMWALL:
            {
                if (y < maze->size - 1) setNodeWallsUnsafe(maze, x, y+1, getMazeNode(maze, x, y+1)->walls | TOPWALL);
                break;
            }

        case LEFTWALL:
            {
                if (x > 0) setNodeWallsUnsafe(maze, x-1, y, getMazeNode(maze, x-1, y)->walls | RIGHTWALL);
                break;
            }

        case RIGHTWALL:
            {
                if (x < maze->size - 1) setNodeWallsUnsafe(maze, x+1, y, getMazeNode(maze, x+1, y)->walls | LEFTWALL);
                break;
            }

        default: assert(false);
        }
    }
    else
    {
        MazeNode node = getMazeNode(maze, x, y);
        setNodeWallsUnsafe(maze, x, y, node->walls & ~wall);

        switch (wall)
        {
        case TOPWALL:
            {
                if (y > 0) setNodeWallsUnsafe(maze, x, y-1, getMazeNode(maze, x, y-1)->walls & ~BOTTOMWALL);
                break;
            }

        case BOTTOMWALL:
            {
                if (y < maze->size - 1) setNodeWallsUnsafe(maze, x, y+1, getMazeNode(maze, x, y+1)->walls & ~TOPWALL);
                break;
            }

        case LEFTWALL:
            {
                if (x > 0) setNodeWallsUnsafe(maze, x-1, y, getMazeNode(maze, x-1, y)->walls & ~RIGHTWALL);
                break;
            }

        case RIGHTWALL:
            {
                if (x < maze->size - 1) setNodeWallsUnsafe(maze, x+1, y, getMazeNode(maze, x+1, y)->walls & ~LEFTWALL);
                break;
            }

        default: assert(false);
        }
    }
}

int getMazeSize(Maze maze)
{
    return maze->size;
}

int getEntryPointX(Maze maze)
{
    return maze->entryPointX;
}

int getEntryPointY(Maze maze)
{
    return maze->entryPointY;
}

int getEntryPointSide(Maze maze)
{
    return maze->entryPointSide;
}

int getExitPointX(Maze maze)
{
    return maze->exitPointX;
}

int getExitPointY(Maze maze)
{
    return maze->exitPointY;
}

int getExitPointSide(Maze maze)
{
    return maze->exitPointSide;
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
    float nodeSize = (float)(GetRenderHeight() - 50) / maze->size;
    float lineThickness = .2f * nodeSize;
    float doorThickness = .1f * nodeSize;
    Vector2 startCoord = {GetRenderWidth()/2 - nodeSize*(maze->size/2.f), 25};

    for (int y = 0; y < maze->size; y++)
    {
        for (int x = 0; x < maze->size; x++)
        {
            DrawTexturePro(Grass, (Rectangle){0, 0, Grass.width, Grass.height}, (Rectangle){startCoord.x + x * nodeSize, startCoord.y + y * nodeSize, nodeSize, nodeSize}, (Vector2){0,0}, 0, WHITE);
        }
    }

    for (int y = 0; y < maze->size; y++)
    {
        for (int x = 0; x < maze->size; x++)
        {
            MazeNode node = getMazeNode(maze, x, y);
            if (isWallTop(node))
            {
                Vector2 lineStart = {startCoord.x + x * nodeSize - lineThickness/2, startCoord.y + y * nodeSize};
                Vector2 lineEnd = {lineStart.x + nodeSize + lineThickness, lineStart.y};
                DrawLineEx(lineStart, lineEnd, lineThickness, DARKGREEN);
            }
            if (isWallLeft(node))
            {
                Vector2 lineStart = {startCoord.x + x * nodeSize, startCoord.y + y * nodeSize - lineThickness/2};
                Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize + lineThickness};
                DrawLineEx(lineStart, lineEnd, lineThickness, DARKGREEN);
            }
            if (isWallRight(node))
            {
                Vector2 lineStart = {startCoord.x + (x + 1) * nodeSize, startCoord.y + y * nodeSize - lineThickness/2};
                Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize + lineThickness};
                DrawLineEx(lineStart, lineEnd, lineThickness, DARKGREEN);
            }
            if (isWallBottom(node))
            {
                Vector2 lineStart = {startCoord.x + x * nodeSize - lineThickness/2, startCoord.y + (y + 1) * nodeSize};
                Vector2 lineEnd = {lineStart.x + nodeSize + lineThickness, lineStart.y};
                DrawLineEx(lineStart, lineEnd, lineThickness, DARKGREEN);
            }
        }
    }

    switch (maze->entryPointSide)
    {
    case TOPWALL:
        {
            Vector2 lineStart = {startCoord.x + maze->entryPointX * nodeSize, startCoord.y + maze->entryPointY * nodeSize};
            Vector2 lineEnd = {lineStart.x + nodeSize, lineStart.y};
            DrawLineEx(lineStart, lineEnd, doorThickness, BROWN);
            break;
        }

    case RIGHTWALL:
        {
            Vector2 lineStart = {startCoord.x + (maze->entryPointX+1) * nodeSize, startCoord.y + maze->entryPointY * nodeSize};
            Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize};
            DrawLineEx(lineStart, lineEnd, doorThickness, BROWN);
            break;
        }

    case BOTTOMWALL:
        {
            Vector2 lineStart = {startCoord.x + maze->entryPointX * nodeSize, startCoord.y + (maze->entryPointY+1) * nodeSize};
            Vector2 lineEnd = {lineStart.x + nodeSize, lineStart.y};
            DrawLineEx(lineStart, lineEnd, doorThickness, BROWN);
            break;
        }

    case LEFTWALL:
        {
            Vector2 lineStart = {startCoord.x + maze->entryPointX * nodeSize, startCoord.y + maze->entryPointY * nodeSize};
            Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize};
            DrawLineEx(lineStart, lineEnd, doorThickness, BROWN);
            break;
        }

    default: break;
    }

    switch (maze->exitPointSide)
    {
    case TOPWALL:
        {
            Vector2 lineStart = {startCoord.x + maze->exitPointX * nodeSize, startCoord.y + maze->exitPointY * nodeSize};
            Vector2 lineEnd = {lineStart.x + nodeSize, lineStart.y};
            DrawLineEx(lineStart, lineEnd, doorThickness, RED);
            break;
        }

    case RIGHTWALL:
        {
            Vector2 lineStart = {startCoord.x + (maze->exitPointX+1) * nodeSize, startCoord.y + maze->exitPointY * nodeSize};
            Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize};
            DrawLineEx(lineStart, lineEnd, doorThickness, RED);
            break;
        }

    case BOTTOMWALL:
        {
            Vector2 lineStart = {startCoord.x + maze->exitPointX * nodeSize, startCoord.y + (maze->exitPointY+1) * nodeSize};
            Vector2 lineEnd = {lineStart.x + nodeSize, lineStart.y};
            DrawLineEx(lineStart, lineEnd, doorThickness, RED);
            break;
        }

    case LEFTWALL:
        {
            Vector2 lineStart = {startCoord.x + maze->exitPointX * nodeSize, startCoord.y + maze->exitPointY * nodeSize};
            Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize};
            DrawLineEx(lineStart, lineEnd, doorThickness, RED);
            break;
        }
    }
    
}

void renderWall(Maze maze, int x, int y, int side, Color color)
{
    float nodeSize = (GetRenderHeight() - 50) / maze->size;
    float lineThickness = .2f * nodeSize;
    Vector2 startCoord = {GetRenderWidth()/2 - nodeSize*(maze->size/2.f), 25};

    switch (side)
    {
    case TOPWALL:
        {
            Vector2 lineStart = {startCoord.x + x * nodeSize - lineThickness/2, startCoord.y + y * nodeSize};
            Vector2 lineEnd = {lineStart.x + nodeSize + lineThickness, lineStart.y};
            DrawLineEx(lineStart, lineEnd, lineThickness, color);
            break;
        }

    case RIGHTWALL:
        {
            Vector2 lineStart = {startCoord.x + (x+1) * nodeSize, startCoord.y + y * nodeSize - lineThickness/2};
            Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize + lineThickness};
            DrawLineEx(lineStart, lineEnd, lineThickness, color);
            break;
        }

    case BOTTOMWALL:
        {
            Vector2 lineStart = {startCoord.x + x * nodeSize - lineThickness/2, startCoord.y + (y+1) * nodeSize};
            Vector2 lineEnd = {lineStart.x + nodeSize + lineThickness, lineStart.y};
            DrawLineEx(lineStart, lineEnd, lineThickness, color);
            break;
        }

    case LEFTWALL:
        {
            Vector2 lineStart = {startCoord.x + x * nodeSize, startCoord.y + y * nodeSize - lineThickness/2};
            Vector2 lineEnd = {lineStart.x, lineStart.y + nodeSize + lineThickness};
            DrawLineEx(lineStart, lineEnd, lineThickness, color);
            break;
        }

    default: break;
    }
}

void GetMazeCoordsFromScreenCoords(int x, int y, Maze maze, int* xOut, int* yOut)
{
    float nodeSize = (GetRenderHeight() - 50) / maze->size;
    Vector2 startCoord = {GetRenderWidth()/2 - nodeSize*(maze->size/2.f), 25};

    *xOut = (x - startCoord.x) / nodeSize;
    *yOut = (y - startCoord.y) / nodeSize;

    if (*xOut < 0 || *yOut < 0 || *xOut >= maze->size || *yOut >= maze->size)
    {
        *xOut = -1;
        *yOut = -1;
    }
}

int GetWallSideFromScreenCoords(int x, int y, Maze maze)
{
    float nodeSize = (GetRenderHeight() - 50) / maze->size;
    Vector2 startCoord = {GetRenderWidth()/2 - nodeSize*(maze->size/2.f), 25};

    int xOut = (x - startCoord.x) / nodeSize;
    int yOut = (y - startCoord.y) / nodeSize;

    int xNode = x - startCoord.x - xOut * nodeSize;
    int yNode = y - startCoord.y - yOut * nodeSize;

    if (xNode < 0 || yNode < 0 || xNode >= nodeSize || yNode >= nodeSize)
    {
        return -1;
    }

    if (abs(xNode - nodeSize/2) > abs(yNode - nodeSize/2))
    {
        return xNode < nodeSize/2 ? LEFTWALL : RIGHTWALL;
    }
    else
    {
        return yNode < nodeSize/2 ? TOPWALL : BOTTOMWALL;
    }
}
