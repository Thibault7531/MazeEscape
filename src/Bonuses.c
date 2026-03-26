#include "Bonuses.h"

#include "raylib.h"
#include <string.h>

#include "../build/external/raylib-master/src/raylib.h"

Texture Egg;
Texture Flower;

int BonusNumber = 4;
int EggX[4], EggY[4], FlowerX[4], FlowerY[4];
int Points = 0;

double LastPenalty = 0;

void getRandomPosition(Maze maze, int* x, int* y)
{
    *x = GetRandomValue(0, maze->size - 1);
    *y = GetRandomValue(0, maze->size - 1);
    while (*x >= maze->startAreaX && *x < maze->startAreaX + maze->startAreaSizeX && *y >= maze->startAreaY && *y < maze->startAreaY + maze->startAreaSizeY)
    {
        *x = GetRandomValue(0, maze->size - 1);
        *y = GetRandomValue(0, maze->size - 1);
    }
}

void initBonuses(Maze maze)
{
    Egg = LoadTexture("oeuf-de-paques.png");
    Flower = LoadTexture("fleur.png");

    memset(EggX, 0, 4 * sizeof(int));
    memset(EggY, 0, 4 * sizeof(int));
    memset(FlowerX, 0, 4 * sizeof(int));
    memset(FlowerY, 0, 4 * sizeof(int));

    for (int i = 0; i < BonusNumber; i++) {
        getRandomPosition(maze, &EggX[i], &EggY[i]);
        getRandomPosition(maze, &FlowerX[i], &FlowerY[i]);
    }

    Points = 0;
    LastPenalty = 0;
}

void updateBonuses(Maze maze, int positionX, int positionY, bool isAIStuck)
{
    for (int i = 0; i < BonusNumber; i++) {
        // Si sur un oeuf
        if (positionX == EggX[i] && positionY == EggY[i]) {
            Points += 10;
            getRandomPosition(maze, &EggX[i], &EggY[i]);
        }
        // Si sur une fleur
        if (positionX == FlowerX[i] && positionY == FlowerY[i]) {
            Points += 5;
            getRandomPosition(maze, &FlowerX[i], &FlowerY[i]);
        }
    }

    if (isAIStuck && GetTime() - LastPenalty > .5f)
    {
        Points -= 1;
        LastPenalty = GetTime();
    }
}

void renderBonuses(Maze maze)
{
    int nodeSize = (GetRenderHeight() - 50) / maze->size;
    Vector2 startCoord = {GetRenderWidth()/2 - nodeSize*(maze->size/2.f), 25};

    float scale = 0.6f; // 0.6 = 60% de la case. Augmente à 0.8 pour plus gros.
    float objSize = nodeSize * scale;
    float centerOffset = (nodeSize - objSize) / 2.0f;
    for (int i = 0; i < BonusNumber; i++) {
        // Dessiner les oeufs
        Rectangle destOeuf = {
            startCoord.x + EggX[i] * nodeSize + centerOffset,
            startCoord.y + EggY[i] * nodeSize + centerOffset,
            objSize, objSize
        };
        DrawTexturePro(Egg, (Rectangle){0,0,(float)Egg.width,(float)Egg.height}, destOeuf, (Vector2){0,0}, 0.0f, WHITE);

        // Dessiner les fleurs
        Rectangle destFleur = {
            startCoord.x + FlowerX[i] * nodeSize + centerOffset,
            startCoord.y + FlowerY[i] * nodeSize + centerOffset,
            objSize, objSize
        };
        DrawTexturePro(Flower, (Rectangle){0,0,(float)Flower.width,(float)Flower.height}, destFleur, (Vector2){0,0}, 0.0f, WHITE);
    }
}

void renderScore()
{
    float renderScale = GetRenderHeight()/720.f;
    int posX = GetRenderWidth() - MeasureText(TextFormat("SCORE : %d", Points), 20 * renderScale) - 10 * renderScale;
    DrawText(TextFormat("SCORE : %d", Points), posX, 50 * renderScale, 20 * renderScale, GetTime() - LastPenalty > .25f ? WHITE : RED);
}

int getBonusPoints()
{
    return Points;
}

void unloadBonuses()
{
    UnloadTexture(Egg);
    UnloadTexture(Flower);
}
