#include <stdlib.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "AI.h"
#include "Bonuses.h"
#include "Game.h"
#include "Maze.h"
#include "Menu.h"
#include "Path.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(1280, 720, "MazeEscape");
	InitAudioDevice();

	SearchAndSetResourceDir("resources");

	Music music = LoadMusicStream("musique_fond.wav");
	PlayMusicStream(music);
	music.looping=true;

	bool inMenu = true;

	initMenu();
	
	// game loop
	while (!WindowShouldClose())
	{
		if (inMenu && shouldStart())
		{
			initGame();
			inMenu = false;
		}

		UpdateMusicStream(music);
		updateGame();

		BeginDrawing();
		{
			ClearBackground((Color){ 154, 205, 50, 255 });
			if (inMenu) renderMenu();
			if (!inMenu) renderGame();
			DrawFPS(10, 10);
		}
		EndDrawing();
	}

	if (shouldStart()) unloadGame();
	UnloadMusicStream(music);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
