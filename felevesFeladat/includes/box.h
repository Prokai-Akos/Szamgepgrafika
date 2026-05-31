#ifndef BOX_H
#define BOX_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "loadModels.h"
#include "loadTextures.h"
#include "cam.h"

typedef struct{
    bool carKeyBoxPickedUP;
    bool clutchBoxPickedUp;
}BoxHandler;

extern float boxX, boxZ;
extern BoxHandler boxHandler;

/**
 * Generates the box in the forrest
 * Random coordinates between 1 and 21
 */
void boxGenerator();

/**
 * Auxiliary function, used only inside drawBoxes
 */
bool generateBox(Model* cardboardBox, GLuint cardboardTexture, float boxX, float boxZ, bool boxPickedUp, Camera* cam, float lightLevel);

/**
 * Initializes the position of the boxes using boxGenerator
 */
void initBoxes();

/**
 * Handles the drawing of the boxes
 */
void drawBoxes();

#endif