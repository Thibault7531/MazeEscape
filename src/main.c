#include <stdlib.h>

#include "chemin.h"
#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "Maze.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");

	SearchAndSetResourceDir("resources");

	Maze maze = createRandomMaze(10);
	Coord* path = malloc(maze->size * maze->size * sizeof(Coord));
	int pathLength = trouverCheminAStar(maze, path);
	
	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		DrawFPS(10, 10);

		renderMaze(maze);
		renderPath(maze, path, pathLength);

		EndDrawing();
	}

	destroyMaze(maze);
	CloseWindow();
	return 0;
}
