#include "engine.h"

SDL_Window *window;
SDL_GLContext gl_context;
SDL_Event event;

int initEngine(){

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow(
        "Countryside adventure",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL
    );

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // 1 = Enable VSync, 0 = Disable
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // FOV, Aspect Ratio, Near Clip, Far Clip
    float fov = 45.0f;
    float aspect = 800.0f / 600.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;

    // Kiszámoljuk a vetítési ablak határait a látószög (FOV) alapján
    float fH = tanf(fov / 360.0f * 3.14159265f) * zNear;
    float fW = fH * aspect;
    // 3D perspektíva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    glClearColor(0.2f, 0.45f, 0.8f, 1.0f); // Sötétebb, telítettebb kék

    //initialize everything else too
    initTextures(&textures);
    initParticles();
    initTrees();
    initSound();
    initFont();
    initStory();
    initBoxes();
    initMapMesh();
    playMusic(backgroundMusic,source);
    if(loadModels() == 0) printf("All models loaded!\n");
    boxGenerator();
    initCam();
}

void handleEngineEvents(SDL_Event* event, bool* need_run, bool* showHelp, bool* storyActive, int* currentStoryLine, int currentMax, const char*** currentLines, bool* doorLocked) {
    if (event->type == SDL_QUIT) {
        *need_run = false;
        return;
    }

    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            *need_run = false;
            return;
        }

        if (event->key.keysym.sym == SDLK_SPACE) {
            if (*storyActive && *currentLines != NULL) {
                if (*currentStoryLine < currentMax - 1) {
                    (*currentStoryLine)++;
                } else {
                    switch (currentState) {
                        case STATE_INTRO:         
                            currentState = STATE_DOOR_LOCKED; 
                            break;
                        case STATE_DOOR_LOCKED:   
                            break;
                        case STATE_CROUCH_DONE:   
                            break;
                        case STATE_KEY_BOX_PICKUP:
                            currentState = STATE_INSIDE_HOUSE; 
                            *doorLocked = false;
                            break;
                        case STATE_INSIDE_HOUSE:  
                            break;
                        case STATE_NO_CLUTCH:     
                            break;
                        case STATE_CLUTCH_PICKUP: 
                            break;
                        case STATE_CAR_REPAIRED:
                            if (storyStates.carRepaired) {
                                Mix_FadeOutMusic(3000);
                            }
                            break;
                        default: break;
                    }
                    
                    *storyActive = false;
                    *currentStoryLine = 0;
                    *currentLines = NULL;
                }
            }
        }

        if (currentState != STATE_CAR_REPAIRED) {
            switch (event->key.keysym.sym) {
                case SDLK_F1:
                    *showHelp = !(*showHelp);
                    if (*showHelp) SDL_SetRelativeMouseMode(SDL_FALSE); 
                    else SDL_SetRelativeMouseMode(SDL_TRUE);
                    break;
                case SDLK_PLUS:
                case SDLK_KP_PLUS:
                case SDLK_F6:
                    lightLevel += 0.05f;
                    if (lightLevel > 1.0f) lightLevel = 1.0f;
                    break;
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                case SDLK_F5:
                    lightLevel -= 0.05f;
                    if (lightLevel < 0.0f) lightLevel = 0.0f;
                    break;
            }
        }
    }

    if (currentState != STATE_CAR_REPAIRED && event->type == SDL_MOUSEMOTION) {
        float sensitivity = 0.2f;
        cam.yaw += event->motion.xrel * sensitivity;
        cam.pitch += event->motion.yrel * sensitivity;

        if (cam.pitch > 89.0f)  cam.pitch = 89.0f;
        if (cam.pitch < -89.0f) cam.pitch = -89.0f;
    }
}

void handleEngineContinuousInput() {
    // Ha a játék véget ért, nem lehet mozogni
    if (currentState == STATE_CAR_REPAIRED) return;

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    float rad = cam.yaw * (3.14159265f / 180.0f);
    float speed = 0.05f; 

    float normalHeight = 0.0f;  
    float crouchHeight = -0.6f; 

    // Gugolás kezelése
    if (state[SDL_SCANCODE_LCTRL]) {
        cam.y = crouchHeight; 
        speed = speed * 0.5f;
    } else {
        cam.y = normalHeight; 
    }

    // Átlós sebesség
    if ((state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S]) && (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])) {
        speed = speed * 0.7071f; 
    }

    // WASD mozgás
    if (state[SDL_SCANCODE_W]) {
        cam.x += sinf(rad) * speed;
        cam.z -= cosf(rad) * speed;
    }
    if (state[SDL_SCANCODE_S]) {
        cam.x -= sinf(rad) * speed;
        cam.z += cosf(rad) * speed;
    }
    if (state[SDL_SCANCODE_A]) {
        cam.x -= cosf(rad) * speed;
        cam.z -= sinf(rad) * speed;
    }
    if (state[SDL_SCANCODE_D]) {
        cam.x += cosf(rad) * speed;
        cam.z += sinf(rad) * speed;
    }
}

void engineSceneRender(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(cam.pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(cam.yaw, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, -cam.y, 0.0f); 
    glTranslatef(-cam.x, 0.0f, -cam.z); 
    glColor3f(currentLight, currentLight, currentLight);

    drawBackgroundWalls(textures.stoneWall, currentLight);
    drawCottage(&models.house, &models.roof, &models.door, textures.greyBrick, textures.redBrick, textures.doorTexture, currentLight);
    drawUaz(&models.uaz, textures.uazTexture, currentLight, &cam);
    drawTrees(&models.tree, textures.spruceTexture, &cam);
    drawBoxes();
    
    if (doorLocked) applyWallCollision(&cam, doorCollision);

    drawMap();
    displayParticles(cam);
    displayCoordinatesInTerminal(&cam);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void engineFogHandler(){
    if (cam.x < 30.0f || cam.z < 30.0f) {
        enableFog();
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f); 
    } 
    else if (currentState != STATE_CAR_REPAIRED) {
        glDisable(GL_FOG); 
        glClearColor(0.4f * currentLight, 0.5f * currentLight, 0.6f * currentLight, 1.0f); 
    }
}

void engineGameEndHandler(){
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}