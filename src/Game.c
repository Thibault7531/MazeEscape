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
    path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze), getExitPointX(maze), getExitPointY(maze));

    while (path.length == 0)
    {
        destroyMaze(maze);
        maze = createRandomMaze(10);
        path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze), getExitPointX(maze), getExitPointY(maze));
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

        if (x != -1 && y != -1 && side != -1)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !leftMouseDown && WallAmount > 0 && !(getNodeWalls(maze, x, y) & side))
            {
                MazeNode node = getMazeNode(maze, x, y);
                setNodeWall(maze, x, y, true, side);
                int nextX, nextY;
                getNextPosition(ai, &nextX, &nextY);
                path = FindPathAStar(maze, nextX, nextY, getExitPointX(maze), getExitPointY(maze));
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
                path = FindPathAStar(maze, nextX, nextY, getExitPointX(maze), getExitPointY(maze));
                setPath(ai, path);
                rightMouseDown = true;
                WallAmount++;
            }
            if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) rightMouseDown = false;
        }

        getCurrentPosition(ai, &aiX, &aiY);
        if (aiX == getExitPointX(maze) && aiY == getExitPointY(maze)) isGameRunning = false;
    }
}

void renderGame()
{
    renderMaze(maze);
    renderBonuses(maze);
    renderScore();

    float renderScale = GetRenderHeight() / 720.f;
    int posX = GetRenderWidth() - MeasureText(TextFormat("Temps : %.1f s", Timer), 20 * renderScale) - 10 * renderScale;
    DrawText(TextFormat("Temps : %.1f s", Timer),posX,20 * renderScale,20 * renderScale, WHITE);
    posX = GetRenderWidth() - MeasureText(TextFormat("Murs : %d", WallAmount), 20 * renderScale) - 10 * renderScale;
    DrawText(TextFormat("Murs : %d", WallAmount),posX,100 * renderScale,20 * renderScale, WHITE);

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

    if (!isGameRunning)
    {
        posX = GetRenderWidth() / 2 - MeasureText("GAME OVER", 50*renderScale) / 2;
        int posY = GetRenderHeight() / 2 - 50*renderScale;
        DrawText("GAME OVER", posX, posY, 50*renderScale, WHITE);

        Rectangle buttonRect = (Rectangle){GetRenderWidth()/2 - 100*renderScale, GetRenderHeight()/2 + 50*renderScale, 200*renderScale, 50*renderScale};
        bool buttonHovered = mouseX > buttonRect.x && mouseX < buttonRect.x + buttonRect.width && mouseY > buttonRect.y && mouseY < buttonRect.y + buttonRect.height;
        DrawRectangleRounded(buttonRect, .25, 1, buttonHovered ? WHITE : DARKGRAY);
        posX = GetRenderWidth()/2 - MeasureText("Rejouer", 40*renderScale) / 2;
        DrawText("Rejouer", posX, buttonRect.y + 8*renderScale, 40*renderScale, buttonHovered ? BLACK : WHITE);
        if (buttonHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) leftMouseDown = true;
        if (buttonHovered && leftMouseDown && !IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            initGame();
            leftMouseDown = false;
        }
        if (leftMouseDown && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) leftMouseDown = false;
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
