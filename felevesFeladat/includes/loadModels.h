#ifndef LOAD_MODELS_H
#define LOAD_MODELS_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <SDL2/SDL_image.h>

#include "obj/include/model.h"
#include "obj/include/draw.h"
#include "obj/include/load.h"
#include "obj/include/transform.h"

typedef struct{
    Model tree;
    Model cardboardBox;
    Model house;
    Model roof;
    Model door;
    Model uaz;
}Models;

extern Models models;

/**
 * texture loader
 */
GLuint loadTexture(const char* filename);

/**
 * Loads all the models in the game
 */
int loadModels();

#endif