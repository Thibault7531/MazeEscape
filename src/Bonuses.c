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

void getRandomPosition(int size, int* x, int* y)
{
    *x = GetRandomValue(0, size - 1);
    *y = GetRandomValue(0, size - 1);
    while (*x >= size / 2 - 1 && *x < size / 2 + 1 && *y >= size / 2 - 1 && *y < size / 2 + 1)
    {
        *x = GetRandomValue(0, size - 1);
        *y = GetRandomValue(0, size - 1);
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
        getRandomPosition(maze->size, &EggX[i], &EggY[i]);
        getRandomPosition(maze->size, &FlowerX[i], &FlowerY[i]);
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
            getRandomPosition(maze->size, &EggX[i], &EggY[i]);
        }
        // Si sur une fleur
        if (positionX == FlowerX[i] && positionY == FlowerY[i]) {
            Points += 5;
            FlowerX[i] = GetRandomValue(0, maze->size - 1);
            FlowerY[i] = GetRandomValue(0, maze->size - 1);
            getRandomPosition(maze->size, &FlowerX[i], &FlowerY[i]);
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
    int nodeSize = 700 / maze->size;
    Vector2 startCoord = {290, 10};

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

int getBonusPoints()
{
    return Points;
}

void unloadBonuses()
{
    UnloadTexture(Egg);
    UnloadTexture(Flower);
}
