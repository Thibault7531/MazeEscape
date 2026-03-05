#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "Maze.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");

	SearchAndSetResourceDir("resources");
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	Maze maze = createMaze(10);
	MazeNode* node = getMazeNode(maze, 0, 0);
	setWallTop(node, true);
	setWallLeft(node, true);
	setWallRight(node, true);
	setWallBottom(node, true);
	printMaze(maze);
	destroyMaze(maze);
	
	// game loop
	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);
		DrawText("Hello Raylib", 200,200,20,WHITE);
		DrawTexture(wabbit, 400, 200, WHITE);
		DrawFPS(10, 10);

		EndDrawing();
	}

	UnloadTexture(wabbit);

	CloseWindow();
	return 0;
}
