#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>
#include <stdio.h>

//Kamera állapota
typedef struct {
    float x, y, z;
    float yaw;
} Camera;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "3D Floating Camera",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // 3D perspektíva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // FOV, Aspect Ratio, Near Clip, Far Clip
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    Camera cam = {0.0f, 0.0f, 5.0f, 0.0f}; // 5 egységgel távolabbi start
    bool need_run = true;
    SDL_Event event;

    while (need_run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) need_run = false;
            
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: need_run = false; break;
                    // Move
                    case SDLK_w: cam.z -= 0.2f; break;
                    case SDLK_s: cam.z += 0.2f; break;
                    case SDLK_a: cam.x -= 0.2f; break;
                    case SDLK_d: cam.x += 0.2f; break;
                    // Look
                    case SDLK_q: cam.yaw -= 2.0f; break;
                    case SDLK_e: cam.yaw += 2.0f; break;
                }
            }
        }

        // Scene render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glLoadIdentity();

        // kamera transzformációk
        glRotatef(cam.yaw, 0, 1, 0);   
        glTranslatef(-cam.x, -cam.y, -cam.z);

        // padló
        glBegin(GL_LINES);
        glColor3f(0.5f, 0.5f, 0.5f);
        for(float i = -10; i <= 10; i += 1.0f) {
            glVertex3f(i, -1.0f, -10.0f); glVertex3f(i, -1.0f, 10.0f);
            glVertex3f(-10.0f, -1.0f, i); glVertex3f(10.0f, -1.0f, i);
        }
        glEnd();

        // példa háromszög
        glBegin(GL_TRIANGLES);
            glColor3f(1, 0, 0); glVertex3f(-1, -1, 0);
            glColor3f(0, 1, 0); glVertex3f( 1, -1, 0);
            glColor3f(0, 0, 1); glVertex3f( 0,  1, 0);
        glEnd();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}