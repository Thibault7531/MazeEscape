#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1920, 1080, "MazeEscape");

	SearchAndSetResourceDir("resources");
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
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
