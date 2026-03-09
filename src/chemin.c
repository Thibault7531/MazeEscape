#include "chemin.h"

NodeAdjacency getAccessibleNeighbors(Maze maze, int x, int y) {
    NodeAdjacency adj;
    adj.count = 0;
    
    MazeNode current = getMazeNode(maze,x,y);
    if (!isWallTop(current)) {   //on test le haut
        adj.neighbors[adj.count].x=x;
        adj.neighbors[adj.count].y=y-1;
        adj.count++;
    }
    if (!isWallBottom(current)) {  //on test le bas
        adj.neighbors[adj.count].x=x;
        adj.neighbors[adj.count].y=y+1;
        adj.count++;
    }
    if (!isWallLeft(current)) {  //on test la gauche
        adj.neighbors[adj.count].x=x-1;
        adj.neighbors[adj.count].y=y;
        adj.count++;
    }
    if (!isWallRight(current)) {  //on test la droite
        adj.neighbors[adj.count].x=x+1;
        adj.neighbors[adj.count].y=y;
        adj.count++;
    }
    return adj;
}
