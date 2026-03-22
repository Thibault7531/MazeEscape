#pragma once

#include "Maze.h"

void getRandomPosition(int size, int* x, int* y);

void initBonuses(Maze maze);
void updateBonuses(Maze maze, int positionX, int positionY);
int getBonusPoints();
void renderBonuses(Maze maze);
void unloadBonuses();
