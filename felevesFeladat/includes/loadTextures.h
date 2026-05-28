#ifndef LOAD_TEXTURES_H
#define LOAD_TEXTURES_H

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
    GLuint grass;
    GLuint dirt;
    GLuint stoneWall;
    GLuint redBrick;
    GLuint greyBrick;
    GLuint doorTexture;
    GLuint spruceTexture;
    GLuint cardboardTexture;
    GLuint uazTexture;
}Textures;

extern Textures textures;

/**
 * Individual texture loader
 */
GLuint loadTexture(const char* filename);

/**
 * Initializes and loads all the necessary game textures
 */
void initTextures(Textures* textures);



#endif