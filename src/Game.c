#include "Game.h"

#include "AI.h"
#include "Bonuses.h"
#include "Maze.h"
#include "Path.h"
#include "../build/external/raylib-master/src/raylib.h"

bool isGameRunning = false;

double StartTime;
float Timer;

int WallAmount = 0;
double LastWallAdded = 0;

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

    WallAmount = 5;
    LastWallAdded = GetTime();

    isGameRunning = true;
}

void updateGame()
{
    if (isGameRunning)
    {
        Timer=(float)(GetTime()-StartTime);

        float deltaTime = GetFrameTime();
        updateAI(maze, ai, deltaTime);

        int aiX;
        int aiY;
        getCurrentPosition(ai, &aiX, &aiY);
        updateBonuses(maze, aiX, aiY, isAIStuck(ai));

        if (GetTime() - LastWallAdded > 5)
        {
            WallAmount++;
            LastWallAdded = GetTime();
        }
        if (WallAmount >= 5)
        {
            WallAmount = 5;
            LastWallAdded = GetTime();
        }

        int mouseX, mouseY;
        mouseX = GetMouseX();
        mouseY = GetMouseY();
        int x, y;
        GetMazeCoordsFromScreenCoords(mouseX, mouseY, maze, &x, &y);
        int side = GetWallSideFromScreenCoords(mouseX, mouseY, maze);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !leftMouseDown && WallAmount > 0 && !(getNodeWalls(maze, x, y) & side))
        {
            MazeNode node = getMazeNode(maze, x, y);
            setNodeWall(maze, x, y, true, side);
            int nextX, nextY;
            getNextPosition(ai, &nextX, &nextY);
            path = FindPathAStar(maze, nextX, nextY);
            setPath(ai, path);
            leftMouseDown = true;
            WallAmount--;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) leftMouseDown = false;

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !rightMouseDown && (getNodeWalls(maze, x, y) & side))
        {
            MazeNode node = getMazeNode(maze, x, y);
            setNodeWall(maze, x, y, false, side);
            int nextX, nextY;
            getNextPosition(ai, &nextX, &nextY);
            path = FindPathAStar(maze, nextX, nextY);
            setPath(ai, path);
            rightMouseDown = true;
            WallAmount++;
        }
        if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) rightMouseDown = false;

        getCurrentPosition(ai, &aiX, &aiY);
        if (aiX == getExitPointX(maze) && aiY == getExitPointY(maze)) isGameRunning = false;
    }
}

void renderGame(Texture2D tex)
{
    DrawText(TextFormat("TEMPS : %.1f s", Timer),1100,20,20, WHITE);
    DrawText(TextFormat("WALLS : %d", WallAmount),1100,100,20, WHITE);

    renderMaze(maze,tex);
    renderBonuses(maze);
    renderScore();

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
        else renderWall(maze, x, y, side, (WallAmount > 0 && isGameRunning) ? (Color){255, 255, 255, 122} : (Color){255, 0, 0, 122});
    }
}

bool isRunning()
{
    return isGameRunning;
}

void unloadGame()
{
    unloadBonuses();
    destroyMaze(maze);
}
