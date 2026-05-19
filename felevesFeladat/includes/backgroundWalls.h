#ifndef BGWALLS_H
#define BGWALLS_H

#include <GL/gl.h>

/**
 * Draws the background walls
 * AI was used here
 */
void drawBackgroundWalls(GLuint mountainTexture, float lightLevel);

/**
 * Used for checking the bounding box at the border of the map
 */
float clamp(float n, float min, float max);


#endif
