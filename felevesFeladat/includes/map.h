#ifndef MAP_H
#define MAP_H

#define WIDTH 80
#define HEIGHT 80

#include "loadTextures.h"
#include "cam.h"

extern float tileSize;
extern int tileMap[WIDTH][HEIGHT];

/**
 * Draws the map, tilesize can be set in map.c, also textures the map
 */
void drawMap();

/**
 * Displayes the coordinates in the terminal, constantly changes
 */
void displayCoordinatesInTerminal(Camera* cam);


#endif