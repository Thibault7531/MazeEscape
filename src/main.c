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

	bool leftMouseDown = false;
	bool rightMouseDown = false;
	
	// game loop
	while (!WindowShouldClose())
	{
		UpdateMusicStream(music);
		timer=(float)(GetTime()-startTime);

		float deltaTime = GetFrameTime();
		updateAI(maze, ai, deltaTime);

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
		DrawText(TextFormat("TEMPS : %.1f s",timer),1100,20,20,WHITE);

		DrawFPS(10, 10);

		renderMaze(maze);

		int nodeSize = 700 / maze->size;
        Vector2 startCoord = {290, 10};

float scale = 0.6f; // 0.6 = 60% de la case. Augmente à 0.8 pour plus gros.
            float objSize = nodeSize * scale;
            float centerOffset = (nodeSize - objSize) / 2.0f;

            for (int y = 0; y < maze->size; y++) {
                for (int x = 0; x < maze->size; x++) {
                    int posX = startCoord.x + x * nodeSize;
                    int posY = startCoord.y + y * nodeSize;

                    if ((x == getEntryPointX(maze) && y == getEntryPointY(maze)) || 
                        (x == getExitPointX(maze) && y == getExitPointY(maze))) continue;

                    int decideur = (x * 11 + y * 17) % 30; 

                    if (decideur == 1) {
                        // Dessin de l'oeuf redimensionné
                        Rectangle source = { 0.0f, 0.0f, (float)textureOeuf.width, (float)textureOeuf.height };
                        Rectangle dest = { posX + centerOffset, posY + centerOffset, objSize, objSize };
                        DrawTexturePro(textureOeuf, source, dest, (Vector2){0,0}, 0.0f, WHITE);
                    } 
                    else if (decideur == 2) {
                        // Dessin de la fleur redimensionnée
                        Rectangle source = { 0.0f, 0.0f, (float)textureFleur.width, (float)textureFleur.height };
                        Rectangle dest = { posX + centerOffset, posY + centerOffset, objSize, objSize };
                        DrawTexturePro(textureFleur, source, dest, (Vector2){0,0}, 0.0f, WHITE);
            }
        }
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
	destroyMaze(maze);
	CloseWindow();
	UnloadMusicStream(music);
	CloseAudioDevice();
	return 0;
}
