#ifndef TEXT_H
#define TEXT_H

#include <stdbool.h>
#include <stdlib.h>
#include "cam.h"
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include "box.h"
#include "gameStates.h"
#include "engine.h"

typedef struct{
    bool startStoryDone;
    bool doorLockedStoryDone;
    bool insideHouseDone;
    bool carKeyBoxPickedupLinesDone;
    bool noClutchDone;
    bool crouchAreaDone;
    bool clutchPickupDone;
    bool carRepaired;
}StoryStates;

typedef struct{
    const char** startStoryLines;
    const char** doorStoryLines;
    const char** crouchSpotStoryLines;
    const char** keyBoxPickupLines;
    const char** doorUnlockLines;
    const char** uazNoClutchLines;
    const char** boxPickedUpLines;
    const char** carRepairedLines;
} StoryLines;

extern StoryStates storyStates;
extern StoryLines storyLines;
extern TTF_Font* font;

extern const char** currentLines;
extern int currentMax;
extern int currentStoryLine;
extern bool storyActive;


/**Initializes TTF and the given font style from assets */
void initFont();

/**Initializes the struct contatining the story states
 * Also initializes the storyLines struct
*/
void initStory();

void updateStoryLogic(bool* storyActive, const char*** currentLines, int* currentMax, int* currentStoryLine);

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