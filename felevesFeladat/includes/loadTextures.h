#ifndef LOAD_TEXT_H
#define LOAD_TEXT_H

#include <GL/gl.h>
#include <SDL2/SDL.h>

extern GLuint grass;
extern GLuint dirt;
extern GLuint stoneWall;
extern GLuint redBrick;
extern GLuint greyBrick;
extern GLuint doorTexture;
extern GLuint spruceTexture;
extern GLuint cardboardTexture;
extern GLuint uazTexture;
extern GLuint tireTexture;

/**
 * Initializes and loads all the necessary textures
 */
void initTextures();

/**
 * Loads the texture from a given file
 */
GLuint loadTexture(const char *filename);

#endif