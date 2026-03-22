#include <stdlib.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "AI.h"
#include "Bonuses.h"
#include "Maze.h"
#include "Path.h"

int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");
	InitAudioDevice();

	SearchAndSetResourceDir("resources");
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	Music music = LoadMusicStream("musique_fond.wav");
	PlayMusicStream(music);
	music.looping=true;

	double startTime=GetTime();
	float timer=0.0f;

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

	initBonuses(maze);

	bool leftMouseDown = false;
	bool rightMouseDown = false;
	
	// game loop
	while (!WindowShouldClose())
	{
		UpdateMusicStream(music);
		timer=(float)(GetTime()-startTime);

		float deltaTime = GetFrameTime();
		updateAI(maze, ai, deltaTime);

        int playerX = (int)(ai->x + 0.5f); 
        int playerY = (int)(ai->y + 0.5f);
		updateBonuses(maze, playerX, playerY);

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

		ClearBackground((Color){ 154, 205, 50, 255 });
		DrawText(TextFormat("SCORE : %d", getBonusPoints()), 1100, 50, 20, RAYWHITE);
		DrawText(TextFormat("TEMPS : %.1f s",timer),1100,20,20,WHITE);

		DrawFPS(10, 10);

		renderMaze(maze);
		renderBonuses(maze);

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

	unloadBonuses();
	destroyMaze(maze);

	UnloadTexture(wabbit);
	UnloadMusicStream(music);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
