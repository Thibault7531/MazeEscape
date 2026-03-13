#include "Path.h"

#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "../build/external/raylib-master/src/raylib.h"


typedef struct {
    int g;         
    int f;          
    int px, py;     
    bool ouvert;    
    bool ferme;     
} NodeIA;


int distance(int x1, int y1, int x2, int y2)
{
    return abs(x1-x2)+abs(y1-y2);
}

Path FindPathAStar(Maze maze, int fromX, int fromY)
{
    NodeIA* grid = malloc(sizeof(NodeIA) * getMazeSize(maze) * getMazeSize(maze));
    
    for(int x = 0; x < getMazeSize(maze); x++) {
        for(int y = 0; y < getMazeSize(maze); y++) {
            grid[x+y*getMazeSize(maze)]=(NodeIA){INT_MAX,INT_MAX,-1,-1,false,false};
        }
    }

    grid[fromX+fromY*getMazeSize(maze)].g=0;
    grid[fromX+fromY*getMazeSize(maze)].f=distance(fromX,fromY, getExitPointX(maze),getEntryPointY(maze));
    grid[fromX+fromY*getMazeSize(maze)].ouvert=true;

    while(true) {
        int currentX=-1,currentY=-1;
        int minF=INT_MAX;

        for(int x = 0; x < getMazeSize(maze); x++) {
            for(int y = 0; y < getMazeSize(maze); y++) {
                if (grid[x+y*getMazeSize(maze)].ouvert && grid[x+y*getMazeSize(maze)].f<minF)
                {
                    minF=grid[x+y*getMazeSize(maze)].f;
                    currentX=x;
                    currentY=y;
                }
            }
        }

        if(currentX==-1){
            free(grid);
            return (Path){0, NULL};
        } 
        if(currentX==getExitPointX(maze) && currentY == getExitPointY(maze)){
            break;
        }

        grid[currentX+currentY*getMazeSize(maze)].ouvert=false;
        grid[currentX+currentY*getMazeSize(maze)].ferme=true;

        int walls = getNodeWalls(maze, currentX, currentY);

        int dx[]={0, 0, -1, 1};
        int dy[]={-1, 1, 0, 0};
        int flags[]={TOPWALL, BOTTOMWALL, LEFTWALL, RIGHTWALL};

        for(int i=0; i<4; i++) {
            if(!(walls & flags[i])) {
                int nx = currentX + dx[i];
                int ny = currentY + dy[i];

                if(nx >= 0 && nx < getMazeSize(maze) && ny >= 0 && ny < getMazeSize(maze) && !grid[nx+ny*getMazeSize(maze)].ferme) {
                    int nouveauG = grid[currentX+currentY*getMazeSize(maze)].g + 1;

                    if(nouveauG < grid[nx+ny*getMazeSize(maze)].g) {
                        grid[nx+ny*getMazeSize(maze)].px = currentX;
                        grid[nx+ny*getMazeSize(maze)].py = currentY;
                        grid[nx+ny*getMazeSize(maze)].g = nouveauG;
                        grid[nx+ny*getMazeSize(maze)].f =nouveauG+distance(nx, ny, getExitPointX(maze), getExitPointY(maze));
                        grid[nx+ny*getMazeSize(maze)].ouvert = true;
                    }
                }
            }
        }
    }

    Path path = {0, NULL};
    path.length = grid[getExitPointX(maze)+getExitPointY(maze)*getMazeSize(maze)].g + 1;
    path.path = malloc(sizeof(Coord) * path.length);

    int tempX = getExitPointX(maze);
    int tempY = getExitPointY(maze);

    for(int i = path.length - 1; i >= 0; i--) {
        path.path[i] = (Coord){tempX, tempY};
        int nextX = grid[tempX+tempY*getMazeSize(maze)].px;
        int nextY = grid[tempX+tempY*getMazeSize(maze)].py;
        tempX = nextX;
        tempY = nextY;
    }

    free(grid);
    return path;
}

void renderPath(Maze maze, Path path)
{
    if (path.length == 0) return;

    Coord currentCoord = path.path[0];
    int nodeSize = 700 / maze->size;
    float lineThickness = .2f * nodeSize;
    Vector2 startCoord = {290, 10};

    for (int i = 1; i < path.length; i++)
    {
        Coord nextCoord = path.path[i];
        Vector2 lineStart = {startCoord.x + (currentCoord.x + .5f) * nodeSize, startCoord.y + (currentCoord.y + .5f) * nodeSize};
        Vector2 lineEnd = {startCoord.x + (nextCoord.x + .5f) * nodeSize, startCoord.y + (nextCoord.y + .5f) * nodeSize};
        DrawLineEx(lineStart, lineEnd, lineThickness, RED);
        currentCoord = path.path[i];
    }
}
