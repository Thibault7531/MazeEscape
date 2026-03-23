#include "Game.h"

#include "AI.h"
#include "Bonuses.h"
#include "Maze.h"
#include "Path.h"
#include "../build/external/raylib-master/src/raylib.h"

double StartTime;
float Timer;

bool leftMouseDown = false;
bool rightMouseDown = false;

Maze maze;
Path path;
AI ai;

void initGame()
{
    StartTime = GetTime();
    Timer = 0;

    maze = createRandomMaze(10);
    path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze));

    while (path.length == 0)
    {
        destroyMaze(maze);
        maze = createRandomMaze(10);
        path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze));
    }
    ai = createAI(getEntryPointX(maze), getEntryPointY(maze));
    setPath(ai, path);

    initBonuses(maze);

    leftMouseDown = false;
    rightMouseDown = false;
}

void updateGame()
{
    Timer=(float)(GetTime()-StartTime);

    float deltaTime = GetFrameTime();
    updateAI(maze, ai, deltaTime);

    int aiX;
    int aiY;
    getCurrentPosition(ai, &aiX, &aiY);
    updateBonuses(maze, aiX, aiY, isAIStuck(ai));

    int mouseX, mouseY;
    mouseX = GetMouseX();
    mouseY = GetMouseY();
    int x, y;
    GetMazeCoordsFromScreenCoords(mouseX, mouseY, maze, &x, &y);
    int side = GetWallSideFromScreenCoords(mouseX, mouseY, maze);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !leftMouseDown)
    {
        MazeNode node = getMazeNode(maze, x, y);
        setNodeWall(maze, x, y, true, side);
        int nextX, nextY;
        getNextPosition(ai, &nextX, &nextY);
        path = FindPathAStar(maze, nextX, nextY);
        setPath(ai, path);
        leftMouseDown = true;
    }
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) leftMouseDown = false;

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !rightMouseDown)
    {
        MazeNode node = getMazeNode(maze, x, y);
        setNodeWall(maze, x, y, false, side);
        int nextX, nextY;
        getNextPosition(ai, &nextX, &nextY);
        path = FindPathAStar(maze, nextX, nextY);
        setPath(ai, path);
        rightMouseDown = true;
    }
    if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) rightMouseDown = false;
}

void renderGame()
{
    DrawText(TextFormat("TEMPS : %.1f s", Timer),1100,20,20, WHITE);

    renderMaze(maze);
    renderBonuses(maze);

    renderPath(maze, path);
    renderAI(maze, ai);

    int mouseX, mouseY;
    mouseX = GetMouseX();
    mouseY = GetMouseY();
    int x, y;
    GetMazeCoordsFromScreenCoords(mouseX, mouseY, maze, &x, &y);
    int side = GetWallSideFromScreenCoords(mouseX, mouseY, maze);

    if (x != -1 && y != -1)
    {
        MazeNode node = getMazeNode(maze, x, y);
        if (node->walls & side) renderWall(maze, x, y, side, (Color){255, 0, 0, 122});
        else renderWall(maze, x, y, side, (Color){255, 255, 255, 122});
    }
}

void unloadGame()
{
    unloadBonuses();
    destroyMaze(maze);
}
