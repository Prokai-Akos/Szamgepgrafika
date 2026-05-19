#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <stdlib.h>
#include "cam.h"
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>

extern bool startStoryDone;
extern bool doorLockedStoryDone;
extern bool insideHouseDone;
extern bool carKeyBoxPickedupLinesDone;
extern bool noClutchDone;
extern bool crouchAreaDone;
extern bool clutchPickupDone;
extern bool carRepaired;

extern const char** currentLines;
extern int currentMax;

extern const char* startStoryLines[];
extern const char* doorStoryLines[];
extern const char* crouchSpotStoryLines[];
extern const char* keyBoxPickupLines[];
extern const char* doorUnlockLines[];
extern const char* uazNoClutchLines[];
extern const char* boxPickedUpLines[];
extern const char* carRepairedLines[];

/*
*Checks if the player is in the starting area, used for displaying the staring story text
*/
bool inStartArea(Camera cam);

/**
 * Checks if the player is in front of the door, used for displaying the door locked text
 */
bool inFrontOfDoor(Camera cam);

/**
 * Checks if the player is in the are where crouching can be used to bug inside the house
 */
bool inCrouchArea(Camera cam);
/**
 * Checks if the camera is inside the house near the door, used for displaying
 * "door unlocked" text
 */
bool insideDoor(Camera cam);

/**
 * Checks if the camera is near the UAZ, used for both the "no clutch" and 
 * for the game ending scene
 */
bool nearUAZ(Camera cam);

/**
 * Check if the camera is inside the house
 */
bool isInsideCottage(Camera cam);


GLuint textToTexture(TTF_Font* font, const char* text);

/**
 * Draws the dialogue box, used for displaying story segments as the game progresses.
 */
void drawDialogueBox(TTF_Font* font, const char* text);

#endif