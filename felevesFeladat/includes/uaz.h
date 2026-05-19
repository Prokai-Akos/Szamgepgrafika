#ifndef UAZ_H
#define UAZ_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "obj/include/model.h"
#include "cam.h"
#include "obj/include/load.h"
#include "obj/include/draw.h"

/**
 * Draws the UAZ model
 */
void drawUaz(Model* uaz, GLuint uazTexture, float lightLevel);

/*
* Used to apply a collision box to the UAZ model
* AI was used for writing this
*/
void applyUazCollision(Camera* cam);

#endif