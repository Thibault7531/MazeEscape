#include "Menu.h"

#include "AI.h"
#include "Bonuses.h"
#include "Maze.h"
#include "../build/external/raylib-master/src/raylib.h"

Maze MenuMaze;
AI MenuAI;

bool GameShouldStart = false;
bool MouseClicked = false;

void initMenu()
{
    MenuMaze = createMenuMaze();
    initBonuses(MenuMaze);

    int aiX, aiY;
    getRandomPosition(MenuMaze, &aiX, &aiY);
    MenuAI = createAI(aiX, aiY);
}

void renderMenu()
{
    renderMaze(MenuMaze);
    renderBonuses(MenuMaze);
    renderAI(MenuMaze, MenuAI);

    float renderScale = GetRenderHeight() / 720.f;
    int size = MeasureText("Maze Escape", 55 * renderScale);
    float posX = GetRenderWidth()/2 - size/2;
    float posY = 265 * renderScale;
    DrawText("Maze Escape", posX, posY, 55 * renderScale, WHITE);

    int sizeX = 150 * renderScale;
    int sizeY = 60 * renderScale;
    int sizeText = MeasureText("Jouer", 40 * renderScale);
    Rectangle buttonRect = (Rectangle){GetRenderWidth()/2 - sizeX/2, 335 * renderScale, sizeX, sizeY};

    Vector2 mousePos = GetMousePosition();
    bool buttonHovered = mousePos.x > buttonRect.x && mousePos.x < buttonRect.x + buttonRect.width && mousePos.y > buttonRect.y && mousePos.y < buttonRect.y + buttonRect.height;

    DrawRectangleRounded(buttonRect, .25, 1, buttonHovered ? WHITE : DARKGRAY);
    DrawText("Jouer", buttonRect.x + 15*renderScale, 345 * renderScale, 40 * renderScale, buttonHovered ? BLACK : WHITE);

    if (buttonHovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) MouseClicked = true;
    if (MouseClicked && !IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        GameShouldStart = true;
        MouseClicked = false;
    }
}

bool shouldStart()
{
    return GameShouldStart;
}
