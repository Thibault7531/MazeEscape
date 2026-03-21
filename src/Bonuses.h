#pragma once

#include "Maze.h"

void initBonuses(Maze maze);
void updateBonuses(Maze maze, int positionX, int positionY);
void renderBonuses(Maze maze);
void unloadBonuses();
