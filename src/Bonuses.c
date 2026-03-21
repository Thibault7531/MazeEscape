#include "Bonuses.h"

#include "raylib.h"
#include <string.h>

Texture Egg;
Texture Flower;

int BonusNumber = 4;
int oeufsX[4], oeufsY[4], fleursX[4], fleursY[4];
int Points = 0;

void initBonuses(Maze maze)
{
    Egg = LoadTexture("oeuf-de-paques.png");
    Flower = LoadTexture("fleur.png");

    memset(oeufsX, 0, 4 * sizeof(int));
    memset(oeufsY, 0, 4 * sizeof(int));
    memset(fleursX, 0, 4 * sizeof(int));
    memset(fleursY, 0, 4 * sizeof(int));

    for (int i = 0; i < BonusNumber; i++) {
        oeufsX[i] = GetRandomValue(0, maze->size - 1);
        oeufsY[i] = GetRandomValue(0, maze->size - 1);
        fleursX[i] = GetRandomValue(0, maze->size - 1);
        fleursY[i] = GetRandomValue(0, maze->size - 1);
    }

    Points = 0;
}

void updateBonuses(Maze maze, int positionX, int positionY)
{
    for (int i = 0; i < BonusNumber; i++) {
        // Si sur un oeuf
        if (positionX == oeufsX[i] && positionY == oeufsY[i]) {
            Points += 10;
            oeufsX[i] = GetRandomValue(0, maze->size - 1);
            oeufsY[i] = GetRandomValue(0, maze->size - 1);
        }
        // Si sur une fleur
        if (positionX == fleursX[i] && positionY == fleursY[i]) {
            Points += 5;
            fleursX[i] = GetRandomValue(0, maze->size - 1);
            fleursY[i] = GetRandomValue(0, maze->size - 1);
        }
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
            startCoord.x + oeufsX[i] * nodeSize + centerOffset,
            startCoord.y + oeufsY[i] * nodeSize + centerOffset,
            objSize, objSize
        };
        DrawTexturePro(Egg, (Rectangle){0,0,(float)Egg.width,(float)Egg.height}, destOeuf, (Vector2){0,0}, 0.0f, WHITE);

        // Dessiner les fleurs
        Rectangle destFleur = {
            startCoord.x + fleursX[i] * nodeSize + centerOffset,
            startCoord.y + fleursY[i] * nodeSize + centerOffset,
            objSize, objSize
        };
        DrawTexturePro(Flower, (Rectangle){0,0,(float)Flower.width,(float)Flower.height}, destFleur, (Vector2){0,0}, 0.0f, WHITE);
    }
}

void unloadBonuses()
{
    UnloadTexture(Egg);
    UnloadTexture(Flower);
}
