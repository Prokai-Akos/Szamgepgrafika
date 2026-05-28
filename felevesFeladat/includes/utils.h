#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "backgroundWalls.h"
#include "cam.h"
#include "gameStates.h"
#include "loadModels.h"
#include "map.h"
#include "particles.h"
#include "sound.h"
#include "texts.h"
#include "tree.h"
#include "uaz.h"
#include "loadTextures.h"

//includes for model loader from gitlab repo
#include "obj/include/draw.h"
#include "obj/include/info.h"
#include "obj/include/load.h"
#include "obj/include/model.h"
#include "obj/include/transform.h"

typedef struct {
    float startX,endX;
    float startZ, endZ;
} Wall;

extern int mapseed;

#define NUM_HOUSE_WALLS 5
extern Wall houseWalls[NUM_HOUSE_WALLS];
extern Wall doorCollision;//zárt ajtóhoz

/**
 * Taken from M_PI definition of math.h, using M_PI works
 * but the file has an error since it only gets replaced at compile time,
 * i didnt want to see the error so i just did it this way 
 */
extern double pi;


/*
*Used for displaying the help menu
*/
void drawHelpMenu(TTF_Font* font);

/**
 * Enables fog and changes clearColor to fog color
 */
void enableFog();

/**
 * Applies the collision box for the house walls
 * leaves a gap for the door so it can be accessed
 */
void applyWallCollision(Camera* cam, Wall w);

/**
 * Generates the box into a random position on the map, 
 * implements checking if the box is picked up already
 */
bool generateBox(Model* cardboardBox, GLuint cardboardTexture, float boxX, float boxZ, bool boxPickedUp, Camera cam, float lightLevel);


/**
 * Draws the entire cottage, walls, roof, door
 * AI was used here
 */
void drawCottage(Model* walls, Model* roof, Model* door, GLuint wallTexture, GLuint roofTexture, GLuint doorTexture, float lightLevel);

#endif