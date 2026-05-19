#ifndef SOUND_H
#define SOUND_h

#include <SDL2/SDL_mixer.h>

extern Mix_Music *backgroundMusic;
extern char* source;

/**
 * Initializes the SDl2 mixer for using sounds
 */
void initSound();

/**
 * Starts playing the background music on a loop
 */
void playMusic(Mix_Music *backgroundMusic, char* source);

#endif