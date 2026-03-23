#include <stdlib.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "AI.h"
#include "Bonuses.h"
#include "Game.h"
#include "Maze.h"
#include "Path.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");
	InitAudioDevice();

	SearchAndSetResourceDir("resources");

	Music music = LoadMusicStream("musique_fond.wav");
	PlayMusicStream(music);
	music.looping=true;

	initGame();
	
	// game loop
	while (!WindowShouldClose())
	{
		UpdateMusicStream(music);
		updateGame();

		BeginDrawing();
		{
			ClearBackground((Color){ 154, 205, 50, 255 });
			renderGame();
			DrawFPS(10, 10);
		}
		EndDrawing();
	}

	unloadGame();
	UnloadMusicStream(music);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
