#include "gameHeaders.h"
#include "map.h"

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
        "Leendő játék",
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

    Model tree; // Itt tároljuk a modell adatait a memóriában
    if (load_model(&tree, "assets/tree/tree.obj")) {
        printf("Modell sikeresen betöltve!\n");
        scale_model(&tree, 2.0, 2.0, 2.0); 
    } else printf("Hiba a modell betöltésekor!\n");
    
    //texture loading
    GLuint grass = loadTexture("assets/grass.jpg");
    GLuint dirt = loadTexture("assets/dirt.jpg");
    GLuint spruceTexture = loadTexture("assets/tree/tree-nonopaque.png");
    //enableFog();
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f); //clear sky
    //glClearColor(0.5f,0.5f,0.5f,0.5f);//foggy sky
    initParticles();

    float tileSize = 1.0f;
    float centerX = (WIDTH * tileSize) / 2.0f;
    float centerZ = (HEIGHT * tileSize) / 2.0f;
    Camera cam = {centerX, 0.0f, centerZ, 0.0f, 0.0f}; // map közepi start
    bool need_run = true;
    SDL_Event event;
    const Uint8 *state = SDL_GetKeyboardState(NULL);//állandó gomblenyomásos mozgásű
    bool showHelp = false;
    float lightLevel = 0.5f; //0.0-1.0 között legyen
    int treeCount = 10;

    while (need_run){
        float rad = cam.yaw * (pi / 180.0f);
        float speed = 0.05f; //"walking" speed

        float normalHeight = 0.0f;  // alap eye-level
        float crouchHeight = -0.6f; // gugolós eye-level
        float bobbingHeight = 0.1f; // view bobbingos

        if ((state[SDL_SCANCODE_W] || state[SDL_SCANCODE_S]) && (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])) 
            speed = speed * 0.7071f; // 1 / sqrt(2)
        

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
        if (cam.x < 10.0f || cam.z < 10.0f) {
            enableFog();
            glClearColor(0.5f, 0.5f, 0.5f, 0.5f); 
        } else {
            glDisable(GL_FOG); 
            glClearColor(0.5f, 0.8f, 1.0f, 1.0f); 
        }

        //Collision detection with trees
        applyTreeCollision(&cam, centerX, centerZ - 10.0f);
        for(int i = 0; i < treeCount; i++) applyTreeCollision(&cam, centerX + (i * 4) + 5, centerZ - 5.0f + (i * 2));

        cam.x = clamp(cam.x, 0.5f, (WIDTH * tileSize) - 0.5f);
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

        //fa kirajzolások
        // fix fa
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST);
        glBindTexture(GL_TEXTURE_2D, spruceTexture);
        glEnable(GL_ALPHA_TEST); //ne mosódjon el a fa széle annyira (másképp a clearcolor-t kapja)
        glAlphaFunc(GL_GREATER, 0.5f); 

        glPushMatrix();
            glTranslatef(centerX, -1.0f, centerZ - 10.0f);
            draw_model(&tree);
        glPopMatrix();

        // forest maker
        for(int i = 0; i < treeCount; i++) {
            glPushMatrix();
                glTranslatef(centerX + (i * 4) + 5, -1.1f, centerZ - 5.0f + (i * 2));
                glRotatef(i * 45, 0, 1, 0);
                draw_model(&tree);
            glPopMatrix();
        }

        // Most kapcsoljuk ki, miután az összes fát kirajzoltuk
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_BLEND);

        glBindTexture(GL_TEXTURE_2D, grass);
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

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind so the triangle doesn't get textured

        displayParticles(cam);

        // példa háromszög
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(centerX -1.0f, -1.0f, centerZ - 0.5f);
        glColor3f(0, 1, 0);
        glVertex3f(centerX + 1.0f, -1.5f, centerZ- 0.5f);
        glColor3f(0, 0, 1);
        glVertex3f(centerX, 1.0f, centerZ - 0.5f);
        glEnd();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        printf("X: %6.2f | Y: %6.2f | Z: %6.2f\r", cam.x, cam.y, cam.z);
        fflush(stdout);

        if(showHelp) drawHelpMenu(font);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}