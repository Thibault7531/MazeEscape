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
	
	// game loop
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		updateAI(maze, ai, deltaTime);

		BeginDrawing();

		ClearBackground(BLACK);

		DrawFPS(10, 10);

		renderMaze(maze);
		renderPath(maze, path);
		renderAI(maze, ai);

		EndDrawing();
	}

	UnloadTexture(wabbit);

	destroyMaze(maze);
	CloseWindow();
	return 0;
}
