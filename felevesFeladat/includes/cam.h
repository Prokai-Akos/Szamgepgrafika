#ifndef CAM_H
#define CAM_H

#include <stdbool.h>

/**
 * Struct used for managing the camera
 */
typedef struct {
    float x, y, z;
    float yaw;
    float pitch;
} Camera;

extern float lightLevel;
extern float currentLight;
extern Camera cam;

/**
 * Initializes the camera, and puts it into starting position
 * Starting position is x = 50.5 z = 79.5
 */
void initCam();

/**
 * Handles the lightlevel inside the game
 */
void lightHandler();

/**
 * Fades out the camera at the end of the game
 */
void endGameCam(bool* need_run);

#endif