#include "cam.h"

float lightLevel = 0.5f; //0.0-1.0 között legyen
float endFade = 1.0f;//ez felesleges globalnak!
float currentLight;
Camera cam;

void initCam(){
    float startX = 50.5;
    float startZ = 79.5;
    cam.x = startX;
    cam.y = 0.0f;
    cam.z = startZ;
    cam.yaw = 0.0f;
    cam.pitch = 0.0f;
}

void lightHandler(){
    currentLight = lightLevel * endFade;
}

void endGameCam(bool* need_run){
    cam.x = 58.0f;
    cam.z = 56.0f;
    cam.y = 1.5f;     
    cam.pitch = 25.0f; 
    cam.yaw = 30.0f;  
    if(endFade > 0.0f) endFade -=0.005f;
    if(endFade < 0.01f) *need_run = false;
}