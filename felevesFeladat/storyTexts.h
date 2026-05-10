#ifndef TEXT_H
#define TEXT_H

#include "gameHeaders.h"

bool startStoryDone = false;
bool doorLockedStoryDone = false;
bool insideHouseDone = false;
bool carKeyBoxPickedupLinesDone = false;
bool noClutchDone = false;
bool crouchAreaDone = false;
bool clutchPickupDone = false;
bool carRepaired = false;

const char** currentLines = NULL;
int currentMax = 0;

const char* startStoryLines[] = {
    "Welcome to the Countryside Adventure! Press f1 for controls.",
    "You got lost and see a house with a vehicle next to it.",
    "You want to investigate the house.",
};

const char* doorStoryLines[] = {
    "The door seems locked...",
    "Perhaps there is another way to get inside"
};

const char* crouchSpotStoryLines[] = {
    "That tree looks very intresting..",
    "I wonder if we can achieve anything by crouching?"
};

const char* keyBoxPickupLines[] = {
    "You picked up a box, it has the car key in it!",
    "You breathe a sigh of relief, a way to escape!",
    "Lets try the door from inside now?"
};

const char* doorUnlockLines[] = {
    "You have unlocked the door!",
    "Lets take a look at the UAZ now",
};

const char* uazNoClutchLines[] = {
    "You try to start the car....",
    "The engine comes alive, but something is wrong",
    "One pedal just falls in...",
    "It appears the car doesnt have a clutch",
    "There might be one laying around in this forest...",
    "You decide to go and try to find it",
    "But beware of the quickly setting fog..."
};

const char* boxPickedUpLines[] = {
    "You picked up the clutch, good job!",
    "Now head back and fix the car",
    "Good luck in the fog!"
};

const char* carRepairedLines[] = {
    "You take some time to fix the car...",
    "...",
    "......",
    ".........",
    "After a while you manage to do it!",
    "Congratulations, freedom awaits!"
};

/*
*Checks if the player is in the starting area, used for displaying the staring story text
*/
static bool inStartArea(Camera cam){
    return (cam.x > 45 && cam.x < 55 && cam.z >60 && cam.z <80);
}

/**
 * Checks if the player is in front of the door, used for displaying the door locked text
 */
static bool inFrontOfDoor(Camera cam){
    return (cam.x > 52.5 && cam.x < 53.0 && cam.z > 54.0 && cam.z < 56.0);
}

/**
 * Checks if the player is in the are where crouching can be used to bug inside the house
 */
static bool inCrouchArea(Camera cam){
    return(cam.x > 51.0 && cam.x < 52.0 && cam.z > 50.0 && cam.z < 52.0);
}

/**
 * Checks if the camera is inside the house near the door, used for displaying
 * "door unlocked" text
 */
static bool insideDoor(Camera cam){
    return (cam.x > 51.5 && cam.x < 55.5 && cam.z > 54.0 && cam.z < 55.5);
}

/**
 * Checks if the camera is near the UAZ, used for both the "no clutch" and 
 * for the game ending scene
 */
static bool nearUAZ(Camera cam){
    return (cam.x > 57.0 && cam.x < 60.0 && cam.z > 53.0 && cam.z < 60.0);
}

#endif