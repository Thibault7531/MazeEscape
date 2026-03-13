#include <stdlib.h>

#include "Path.h"
#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "Maze.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");

	SearchAndSetResourceDir("resources");

	Maze maze = createRandomMaze(10);
	Path path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze));
	while (path.length == 0)
	{
		destroyMaze(maze);
		maze = createRandomMaze(10);
		path = FindPathAStar(maze, getEntryPointX(maze), getEntryPointY(maze));
	}
	
	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		DrawFPS(10, 10);

		renderMaze(maze);
		renderPath(maze, path);

		EndDrawing();
	}

	destroyMaze(maze);
	CloseWindow();
	return 0;
}
