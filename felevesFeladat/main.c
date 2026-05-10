#include "gameHeaders.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) printf("TTF_Init error: %s\n", TTF_GetError());
    TTF_Font* font = TTF_OpenFont("assets/Roboto-VariableFont_wdth,wght.ttf", 24); // Make sure you have a .ttf file!
    if (!font) printf("Font load error: %s\n", TTF_GetError());

    SDL_Window *window = SDL_CreateWindow(
        "Countryside adventure",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // 1 = Enable VSync, 0 = Disable
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // 3D perspektíva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // FOV, Aspect Ratio, Near Clip, Far Clip
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    //audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) printf("SDL_mixer error: %s\n", Mix_GetError());
    Mix_Music *backgroundMusic = Mix_LoadMUS("assets/sound/geoffharvey-creepy-hollow-369570.mp3");
    if (!backgroundMusic) printf("Failed to load music: %s\n", Mix_GetError());
    Mix_PlayMusic(backgroundMusic, -1); //-1 a looped play

    //texture loading
    GLuint grass = loadTexture("assets/grass.jpg");
    GLuint dirt = loadTexture("assets/dirt.jpg");
    GLuint stoneWall = loadTexture("assets/stoneWall.jpg");
    GLuint redBrick = loadTexture("assets/brick.jpg");
    GLuint greyBrick = loadTexture("assets/grey_brick.jpg");
    GLuint doorTexture = loadTexture("assets/door.jpg");
    GLuint spruceTexture = loadTexture("assets/tree/tree-nonopaque.png");
    GLuint cardboardTexture = loadTexture("assets/box/cardboard.jpg");
    GLuint uazTexture = loadTexture("assets/qualityUAZTexture.png");
    GLuint tireTexture = loadTexture("assets/darkgrey.png");
    
    //glClearColor(0.5f, 0.8f, 1.0f, 1.0f); //clear sky
    glClearColor(0.2f, 0.45f, 0.8f, 1.0f); // Sötétebb, telítettebb kék
    initParticles();
    initTrees();
    if(loadModels() == 0) printf("All models loaded!\n");

    float tileSize = 1.0f;
    float centerX = (WIDTH * tileSize) / 2.0f;
    float centerZ = (HEIGHT * tileSize) / 2.0f;
    float startX = 50.5;
    float startZ = 79.5;
    Camera cam = {startX, 0.0f, startZ, 0.0f, 0.0f};// map széli start
    bool need_run = true;
    SDL_Event event;
    const Uint8 *state = SDL_GetKeyboardState(NULL);//állandó gomblenyomásos mozgásű
    bool showHelp = false;
    float lightLevel = 0.5f; //0.0-1.0 között legyen
    int treeCount = 10;
    float boxX, boxZ;
    bool clutchBoxPickedUp = false;
    bool carKeyBoxPickedUP = false;
    bool doorLocked = true;
    srand(time(NULL));
    boxX = (float)(rand() % 20) + 1.0f; // 1 és 21 közötti random koordináta
    boxZ = (float)(rand() % 20) + 1.0f; 
    printf(" Box generated: X: %.2f, Z: %.2f\n", boxX, boxZ);
    bool storyActive = false; 
    int currentStoryLine = 0;
    int currentMax = 0;
    const char** currentLines = NULL;

    bool startStoryDone = false;
    bool doorLockedStoryDone = false;

    while (need_run){
        float rad = cam.yaw * (pi / 180.0f);
        float speed = 0.05f; //"walking" speed

        float normalHeight = 0.0f;  // alap eye-level
        float crouchHeight = -0.6f; // gugolós eye-level
        float bobbingHeight = 0.1f; // view bobbingos
        
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT)
                need_run = false;

            if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym){
                case SDLK_ESCAPE:
                    need_run = false;
                    break;
                case SDLK_F1:
                    showHelp = !showHelp;
                    if (showHelp) SDL_SetRelativeMouseMode(SDL_FALSE); 
                    else SDL_SetRelativeMouseMode(SDL_TRUE);
                    break;
                case SDLK_PLUS:
                case SDLK_KP_PLUS:
                        lightLevel += 0.05f;
                    if (lightLevel > 1.0f) lightLevel = 1.0f;
                    break;
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    lightLevel -= 0.05f;
                    if(lightLevel < 0) lightLevel = 0;
                    break;
                case SDLK_SPACE:
                //used for handling storylines, so we dont display the same thing twice
                 if (storyActive && currentLines != NULL) {
                    if (currentStoryLine < currentMax - 1) {
                        currentStoryLine++;
                    } else {
                        if(currentLines == startStoryLines) startStoryDone = true;
                        if(currentLines == doorStoryLines) doorLockedStoryDone = true;
                        if(currentLines == crouchSpotStoryLines) crouchAreaDone = true;
                        if(currentLines == keyBoxPickupLines) carKeyBoxPickedupLinesDone = true;
                        if(currentLines == doorUnlockLines) insideHouseDone = true;
                        if(currentLines == uazNoClutchLines) noClutchDone = true;
                        if(currentLines == boxPickedUpLines) clutchPickupDone = true;
                        if(currentLines == carRepairedLines) carRepaired = true;
                        
                        storyActive = false;
                        currentStoryLine = 0;
                        currentLines = NULL; // Reseteljük a pointert
                    }
                }
                }
            }

            if (event.type == SDL_MOUSEMOTION){
                float sensitivity = 0.2f;
                cam.yaw += event.motion.xrel * sensitivity;
                cam.pitch += event.motion.yrel * sensitivity;

                if (cam.pitch > 89.0f)
                    cam.pitch = 89.0f;
                if (cam.pitch < -89.0f)
                   cam.pitch = -89.0f;
            }
        }

        //gugolás implementáció
        if (state[SDL_SCANCODE_LCTRL]){
            cam.y = crouchHeight; 
            speed = speed * 0.5;
        } 
        else cam.y = normalHeight;

        //wasd mozgás
        if (state[SDL_SCANCODE_W]){
            cam.x += sinf(rad) * speed;
            cam.z -= cosf(rad) * speed;
        }
        if (state[SDL_SCANCODE_S]){
            cam.x -= sinf(rad) * speed;
            cam.z += cosf(rad) * speed;
        }
        if (state[SDL_SCANCODE_A]){
            cam.x -= cosf(rad) * speed;
            cam.z -= sinf(rad) * speed;
        }
        if (state[SDL_SCANCODE_D]){
            cam.x += cosf(rad) * speed;
            cam.z += sinf(rad) * speed;
        }
        //diagonális mozgás
        if ((state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S]) && (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])) 
            speed = speed * 0.7071f; // 1 / sqrt(2)
        if (cam.x < 30.0f || cam.z < 30.0f) {
            enableFog();
            glClearColor(0.5f, 0.5f, 0.5f, 0.5f); 
        } else {
            glDisable(GL_FOG); 
            //glClearColor(0.5f, 0.8f, 1.0f, 1.0f); 
            glClearColor(0.4f *lightLevel, 0.5f*lightLevel, 0.6f*lightLevel, 1.0f); // Sötétebb, telítettebb kék
        }

        //collisions
        for (int i = 0; i < NUM_HOUSE_WALLS; i++) applyWallCollision(&cam, houseWalls[i]);
        cam.x = clamp(cam.x, 0.5f, (WIDTH * tileSize) - 0.5f); //border collisions
        cam.z = clamp(cam.z, 0.5f, (HEIGHT * tileSize) - 0.5f);

        // Scene render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // kamera transzformációk
        glRotatef(cam.pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(cam.yaw, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, -cam.y, 0.0f); // Csak a magasság eltolása
        glTranslatef(-cam.x, 0.0f, -cam.z); // Csak a síkbeli mozgás
        glColor3f(lightLevel, lightLevel, lightLevel);

        drawBackgroundWalls(stoneWall, lightLevel);
        drawCottage(&house,&roof, &door,greyBrick,redBrick,doorTexture,lightLevel);
        drawUaz(&uaz, uazTexture, lightLevel);
        applyUazCollision(&cam);

        drawTrees(&tree, spruceTexture, treeCount, centerX, centerZ, &cam);
        clutchBoxPickedUp = generateBox(&cardboardBox, cardboardTexture, boxX, boxZ, clutchBoxPickedUp, cam, lightLevel);
        carKeyBoxPickedUP = generateBox(&cardboardBox, cardboardTexture, 49.5, 54.5, carKeyBoxPickedUP, cam, lightLevel);

        if(isInsideCottage(cam)) doorLocked = false;
        if (doorLocked) applyWallCollision(&cam, doorCollision);

        glColor3f(lightLevel, lightLevel, lightLevel);//adjust color based on lightlevel

        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                //tileMap alapján választja a textúrát
                if (tileMap[i][j] == 0)      glBindTexture(GL_TEXTURE_2D, grass);
                else if (tileMap[i][j] == 1) glBindTexture(GL_TEXTURE_2D, dirt);

                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(i * tileSize, -1.0f, j * tileSize);
                    glTexCoord2f(1.0f, 0.0f); glVertex3f((i+1) * tileSize, -1.0f, j * tileSize);
                    glTexCoord2f(1.0f, 1.0f); glVertex3f((i+1) * tileSize, -1.0f, (j+1) * tileSize);
                    glTexCoord2f(0.0f, 1.0f); glVertex3f(i * tileSize, -1.0f, (j+1) * tileSize);
                glEnd();
            }
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        displayParticles(cam);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        printf("X: %6.2f | Y: %6.2f | Z: %6.2f\r", cam.x, cam.y, cam.z);
        fflush(stdout);

        //Decides which storyline we are on
        if (!storyActive) {
            const char** tempLines = NULL;
            int tempMax = 0;

            if (!startStoryDone && inStartArea(cam)) {
                tempLines = startStoryLines;
                tempMax = 3;
            } 
            else if (!doorLockedStoryDone && doorLocked && inFrontOfDoor(cam)) {
                tempLines = doorStoryLines;
                tempMax = 2;
            }
            else if(!crouchAreaDone && doorLocked && inCrouchArea(cam)){
                tempLines = crouchSpotStoryLines;
                tempMax = 2;
            }
            else if(!carKeyBoxPickedupLinesDone && isInsideCottage(cam)){
                tempLines = keyBoxPickupLines;
                tempMax = 3;
            }
            else if(!insideHouseDone && !doorLocked && insideDoor(cam)){
                tempLines = doorUnlockLines;
                tempMax = 2;
            }
            else if(!noClutchDone && insideHouseDone && nearUAZ(cam)){
                tempLines = uazNoClutchLines;
                tempMax = 7;
            }
            else if(!clutchPickupDone && noClutchDone && clutchBoxPickedUp){
                tempLines = boxPickedUpLines;
                tempMax = 3;
            }
            else if(!carRepaired && clutchPickupDone && nearUAZ(cam)){
                tempLines = carRepairedLines;
                tempMax = 6;
            }

            if (tempLines != NULL) {
                currentLines = tempLines;
                currentMax = tempMax;
                currentStoryLine = 0; // Reseteljük az elejére
                storyActive = true;
            }
        } 
        else {
            if (currentLines == doorStoryLines && !inFrontOfDoor(cam)) {
                storyActive = false;
                currentLines = NULL;
            }
        }

        if (storyActive && currentLines != NULL) drawDialogueBox(font, currentLines[currentStoryLine]);

        if(showHelp) drawHelpMenu(font);
        SDL_GL_SwapWindow(window);
    }

    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}