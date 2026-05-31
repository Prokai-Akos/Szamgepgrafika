#ifndef ENGINE_H
#define ENGINE_H

#include <math.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include "cam.h"
#include "texts.h"
#include "utils.h"

extern SDL_Window *window;
extern SDL_GLContext gl_context;
extern SDL_Event event;

/**
 * Initializes OpenGL and its components, determines FOV, near and far clip
 */
int initEngine();

/**
 * Used for handling single key press event
 */
void handleEngineEvents(SDL_Event* event, bool* need_run, bool* showHelp, bool* storyActive, int* currentStoryLine, int currentMax, const char*** currentLines, bool* doorLocked);

/**
 * Used for handling continous key presses such as movement
 */
void handleEngineContinuousInput();

/**
 * Renders the scene, draws everything necessary for the scene
 */
void engineSceneRender();

/**
 * Used to handle fog mechanics in the game
 */
void engineFogHandler();

/**
 * Properly shuts the game down
 */
void engineGameEndHandler();

#endif