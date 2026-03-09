#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "Maze.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");

	SearchAndSetResourceDir("resources");
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	Maze maze = createRandomMaze(9);
	
	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		DrawFPS(10, 10);

		renderMaze(maze);

		EndDrawing();
	}

	destroyMaze(maze);

	UnloadTexture(wabbit);

	CloseWindow();
	return 0;
}
