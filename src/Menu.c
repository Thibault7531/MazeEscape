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

    DrawText("Maze Escape", 442, 265, 60, WHITE);

    Vector2 mousePos = GetMousePosition();
    bool buttonHovered = mousePos.x > 565 && mousePos.x < 715 && mousePos.y > 335 && mousePos.y < 400;

    Rectangle buttonRect = (Rectangle){565, 335, 150, 60};
    DrawRectangleRounded(buttonRect, .25, 1, buttonHovered ? WHITE : DARKGRAY);
    DrawText("Jouer", 580, 345, 40, buttonHovered ? BLACK : WHITE);

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
