#include <stdlib.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <time.h>

#include "AI.h"
#include "Maze.h"
#include "Path.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");

	SearchAndSetResourceDir("resources");
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	Maze maze = createRandomMaze(10);
	Path path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze));
	while (path.length == 0)
	{
		destroyMaze(maze);
		maze = createRandomMaze(10);
		path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze));
	}
	AI ai = createAI(getEntryPointX(maze), getEntryPointY(maze), wabbit);
	setPath(ai, path);

	bool leftMouseDown = false;
	bool rightMouseDown = false;
	
	// game loop
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		updateAI(maze, ai, deltaTime);

		int mouseX, mouseY;
		mouseX = GetMouseX();
		mouseY = GetMouseY();

		int x, y;
		GetMazeCoordsFromScreenCoords(mouseX, mouseY, maze, &x, &y);
		int side = GetWallSideFromScreenCoords(mouseX, mouseY, maze);

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !leftMouseDown)
		{
			MazeNode node = getMazeNode(maze, x, y);
			setNodeWall(maze, x, y, true, side);
			int nextX, nextY;
			getNextPosition(ai, &nextX, &nextY);
			path = FindPathAStar(maze, nextX, nextY);
			setPath(ai, path);
			leftMouseDown = true;
		}
		if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) leftMouseDown = false;

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !rightMouseDown)
		{
			MazeNode node = getMazeNode(maze, x, y);
			setNodeWall(maze, x, y, false, side);
			int nextX, nextY;
			getNextPosition(ai, &nextX, &nextY);
			path = FindPathAStar(maze, nextX, nextY);
			setPath(ai, path);
			rightMouseDown = true;
		}
		if (!IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) rightMouseDown = false;

		BeginDrawing();

		ClearBackground(BLACK);

		DrawFPS(10, 10);

		renderMaze(maze);
		renderPath(maze, path);
		renderAI(maze, ai);

		if (x != -1 && y != -1)
		{
			MazeNode node = getMazeNode(maze, x, y);
			if (node->walls & side) renderWall(maze, x, y, side, (Color){255, 0, 0, 122});
			else renderWall(maze, x, y, side, (Color){255, 255, 255, 122});
		}

		EndDrawing();
	}

	UnloadTexture(wabbit);

	destroyMaze(maze);
	CloseWindow();
	return 0;
}
