#pragma once

#include "Maze.h"

void getRandomPosition(Maze maze, int* x, int* y);

void initBonuses(Maze maze);
void updateBonuses(Maze maze, int positionX, int positionY, bool isAIStuck);
void getRandomBonusPosition(Maze maze, int* x, int* y);
int getBonusPoints();
void renderBonuses(Maze maze);
void renderScore();
void unloadBonuses();
