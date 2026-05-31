#include "texts.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>

TTF_Font* font;
StoryStates storyStates;
StoryLines storyLines;

const char** currentLines = NULL;
int currentMax = 0;
int currentStoryLine = 0;
bool storyActive = false; 

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

void initFont(){
    if (TTF_Init() == -1) printf("TTF_Init error: %s\n", TTF_GetError());
    font = TTF_OpenFont("assets/Roboto-VariableFont_wdth,wght.ttf", 24);
    if (!font) printf("Font load error: %s\n", TTF_GetError());
}

void initStory(){
    storyStates.startStoryDone = false;
    storyStates.doorLockedStoryDone = false;
    storyStates.insideHouseDone = false;
    storyStates.carKeyBoxPickedupLinesDone = false;
    storyStates.noClutchDone = false;
    storyStates.crouchAreaDone = false;
    storyStates.clutchPickupDone = false;
    storyStates.carRepaired = false;

    storyLines.startStoryLines = startStoryLines;
    storyLines.doorStoryLines = doorStoryLines;
    storyLines.crouchSpotStoryLines = crouchSpotStoryLines;
    storyLines.keyBoxPickupLines = keyBoxPickupLines;
    storyLines.doorUnlockLines = doorUnlockLines;
    storyLines.uazNoClutchLines = uazNoClutchLines;
    storyLines.boxPickedUpLines = boxPickedUpLines;
    storyLines.carRepairedLines = carRepairedLines;
    
}

void updateStoryLogic(bool* storyActive, const char*** currentLines, int* currentMax, int* currentStoryLine) {
    if (*storyActive) {
        if (currentState == STATE_DOOR_LOCKED && !inFrontOfDoor(cam)) {
            *storyActive = false;
            *currentLines = NULL;
        }
        return; 
    }

    const char** tempLines = NULL;
    int tempMax = 0;

    switch (currentState) {
        case STATE_INTRO:
            if (inStartArea(cam)) {
                tempLines = storyLines.startStoryLines;
                tempMax = 3;
                *storyActive = true;
            }
            break;
        case STATE_DOOR_LOCKED:
            if (inFrontOfDoor(cam) && !storyStates.doorLockedStoryDone) {
                tempLines = storyLines.doorStoryLines;
                tempMax = 2;
                *storyActive = true;
                storyStates.doorLockedStoryDone = true;
            }
            else if (inCrouchArea(cam)) {
                currentState = STATE_CROUCH_DONE;
                tempLines = storyLines.crouchSpotStoryLines;
                tempMax = 2;
                *storyActive = true;
            }
            break;
        case STATE_CROUCH_DONE:
            if (boxHandler.carKeyBoxPickedUP && isInsideCottage(cam)) {
                currentState = STATE_KEY_BOX_PICKUP; 
                tempLines = storyLines.keyBoxPickupLines;
                tempMax = 3;
                *storyActive = true;
            }
            break;
        case STATE_KEY_BOX_PICKUP:
            break;
        case STATE_INSIDE_HOUSE:
            if (nearUAZ(cam)) {
                currentState = STATE_NO_CLUTCH;
                tempLines = storyLines.uazNoClutchLines;
                tempMax = 7;
                *storyActive = true;
            }
            break;
        case STATE_NO_CLUTCH:
            if (boxHandler.clutchBoxPickedUp) {
                currentState = STATE_CLUTCH_PICKUP;
                tempLines = storyLines.boxPickedUpLines;
                tempMax = 3;
                *storyActive = true;
            }
            break;
        case STATE_CLUTCH_PICKUP:
            if (nearUAZ(cam) && !storyStates.carRepaired) {
                currentState = STATE_CAR_REPAIRED;
                tempLines = storyLines.carRepairedLines;
                tempMax = 6;
                *storyActive = true;
                storyStates.carRepaired = true;
            }
            break;
        case STATE_CAR_REPAIRED:
            break;
    }   

    if (*storyActive && tempLines != NULL) {
        *currentLines = tempLines;
        *currentMax = tempMax;
        *currentStoryLine = 0;
    }
}

bool inStartArea(Camera cam){
    return (cam.x > 45 && cam.x < 55 && cam.z >60 && cam.z <80);
}

bool inFrontOfDoor(Camera cam){
    return (cam.x > 52.5 && cam.x < 53.0 && cam.z > 54.0 && cam.z < 56.0);
}

bool inCrouchArea(Camera cam){
    return(cam.x > 51.0 && cam.x < 52.0 && cam.z > 50.0 && cam.z < 52.0);
}

bool insideDoor(Camera cam){
    return (cam.x > 51.5 && cam.x < 55.5 && cam.z > 54.0 && cam.z < 55.5);
}

bool nearUAZ(Camera cam){
    return (cam.x > 57.0 && cam.x < 60.0 && cam.z > 53.0 && cam.z < 60.0);
}

bool isInsideCottage(Camera cam) {
    return (cam.x > 48.5f && cam.x < 52.0f &&
            cam.z > 52.0f && cam.z < 57.5f);
}

GLuint textToTexture(TTF_Font* font, const char* text) {
    if (!font || !text || text[0] == '\0') return 0;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* initialSurf = TTF_RenderUTF8_Blended(font, text, white);
    if (!initialSurf) return 0;

    SDL_Surface* surf = SDL_ConvertSurfaceFormat(initialSurf, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(initialSurf);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surf);
    return textureID;
}

void drawDialogueBox(TTF_Font* font, const char* text) {
    if (!font || !text || text[0] == '\0') return;

    // 1. Átváltás 2D-be
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 2. Fekete háttér sáv alul
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(0, 450);   glVertex2f(800, 450);
        glVertex2f(800, 600); glVertex2f(0, 600);
    glEnd();

    // 3. Szöveg méretének lekérése és rajzolása
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Fix fehér szín, ne hasson rá a lightLevel!

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* tempSurf = TTF_RenderUTF8_Blended(font, text, white);
    if (tempSurf) {
        int textW = tempSurf->w;
        int textH = tempSurf->h;
        SDL_FreeSurface(tempSurf); // Csak a méret kell, a textúrát a textToTexture csinálja

        GLuint tex = textToTexture(font, text);
        if (tex != 0) {
            glBindTexture(GL_TEXTURE_2D, tex);
            
            // Itt a trükk: a kezdőpont fix (50, 500), de a végpont a szöveg szélességétől függ
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex2f(50, 500);
                glTexCoord2f(1, 0); glVertex2f(50 + textW, 500);
                glTexCoord2f(1, 1); glVertex2f(50 + textW, 500 + textH);
                glTexCoord2f(0, 1); glVertex2f(50, 500 + textH);
            glEnd();
            
            glDeleteTextures(1, &tex);
        }
    }

    // 4. Vissza 3D-be
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}