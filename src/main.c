#include <stdlib.h>

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <time.h>

#include "AI.h"
#include "Maze.h"
#include "Path.h"

Texture2D textureOeuf;
Texture2D textureFleur;


int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 720, "MazeEscape");
	InitAudioDevice();

	SearchAndSetResourceDir("resources");
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	textureOeuf = LoadTexture("oeuf-de-paques.png"); 
	textureFleur = LoadTexture("fleur.png");

	Music music=LoadMusicStream("musique_fond.wav");
	PlayMusicStream(music);
	music.looping=true;

	double startTime=GetTime();
	float timer=0.0f;
	int cpt=0; 
	int nbobjets=4;
	int oeufsX[4],oeufsY[3], fleursX[3], fleursY[3];

	Maze maze = createRandomMaze(10);
	for (int i = 0; i < nbobjets; i++) {
        oeufsX[i] = GetRandomValue(0, maze->size - 1);
        oeufsY[i] = GetRandomValue(0, maze->size - 1);
        fleursX[i] = GetRandomValue(0, maze->size - 1);
        fleursY[i] = GetRandomValue(0, maze->size - 1);
	}

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
		UpdateMusicStream(music);
		timer=(float)(GetTime()-startTime);

		float deltaTime = GetFrameTime();
		updateAI(maze, ai, deltaTime);
////////////////
        int playerX = (int)(ai->x + 0.5f); 
        int playerY = (int)(ai->y + 0.5f);

        for (int i = 0; i < nbobjets; i++) {
            // Si sur un oeuf
            if (playerX == oeufsX[i] && playerY == oeufsY[i]) {
                cpt += 10;
                oeufsX[i] = GetRandomValue(0, maze->size - 1);
                oeufsY[i] = GetRandomValue(0, maze->size - 1);
            }
            // Si sur une fleur
            if (playerX == fleursX[i] && playerY == fleursY[i]) {
                cpt += 5;
                fleursX[i] = GetRandomValue(0, maze->size - 1);
                fleursY[i] = GetRandomValue(0, maze->size - 1);
            }
        }

///////////
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
		DrawText(TextFormat("SCORE : %d", cpt), 1100, 50, 20, RAYWHITE);
		DrawText(TextFormat("TEMPS : %.1f s",timer),1100,20,20,WHITE);

		DrawFPS(10, 10);

		renderMaze(maze);

		int nodeSize = 700 / maze->size;
        Vector2 startCoord = {290, 10};

float scale = 0.6f; // 0.6 = 60% de la case. Augmente à 0.8 pour plus gros.
            float objSize = nodeSize * scale;
            float centerOffset = (nodeSize - objSize) / 2.0f;
			for (int i = 0; i < nbobjets; i++) {
            // Dessiner les oeufs
            Rectangle destOeuf = { 
                startCoord.x + oeufsX[i] * nodeSize + centerOffset, 
                startCoord.y + oeufsY[i] * nodeSize + centerOffset, 
                objSize, objSize 
            };
            DrawTexturePro(textureOeuf, (Rectangle){0,0,(float)textureOeuf.width,(float)textureOeuf.height}, destOeuf, (Vector2){0,0}, 0.0f, WHITE);

            // Dessiner les fleurs
            Rectangle destFleur = { 
                startCoord.x + fleursX[i] * nodeSize + centerOffset, 
                startCoord.y + fleursY[i] * nodeSize + centerOffset, 
                objSize, objSize 
            };
            DrawTexturePro(textureFleur, (Rectangle){0,0,(float)textureFleur.width,(float)textureFleur.height}, destFleur, (Vector2){0,0}, 0.0f, WHITE);
        }

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
	UnloadTexture(textureOeuf);
	UnloadTexture(textureFleur);
	UnloadMusicStream(music);
	CloseAudioDevice();
	destroyMaze(maze);
	CloseWindow();
	return 0;
}
