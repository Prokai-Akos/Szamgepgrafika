#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

//includes for model loader from gitlab repo
#include "obj/include/draw.h"
#include "obj/include/info.h"
#include "obj/include/load.h"
#include "obj/include/model.h"
#include "obj/include/transform.h"


/**
 * Kamera kezeléshez struct
 */
typedef struct {
    float x, y, z;
    float yaw;
    float pitch;
} Camera;

/**
 * Particle kezelő struct 
 * */
typedef struct {
    float x, y, z;
    float vy;//speed
    float life;//lifetime
} Particle;

#define maxParticle 600
static Particle particles[maxParticle];


/**
 * Taken from M_PI definition of math.h, using M_PI works
 * but the file has an error since it only gets replaced at compile time,
 * i didnt want to see the error so i just did it this way 
 */
static double pi = 3.14159265358979323846;


/**
 * texture loader, static so if i add it to more files the compiler wont kill itself
 */
static GLuint loadTexture(const char* filename) {
    SDL_Surface* surface = IMG_Load(filename);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters (Wrapping and Filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Upload data to GPU
    int mode = (surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// Force OpenGL to read pixels byte-by-byte, ignoring 4-byte row alignment
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);
    return textureID;
}

static GLuint textToTexture(TTF_Font* font, const char* text) {
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

static void drawHelpMenu(TTF_Font* font) {
    // 1. Switch to 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 2. Draw Dark Overlay
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(50, 50); glVertex2f(750, 50);
        glVertex2f(750, 550); glVertex2f(50, 550);
    glEnd();

    // 3. Render Lines
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    const char* lines[] = {
        "HASZNALATI UTMUTATO",
        "-------------------",
        "- W,A,S,D: Mozgas",
        "- L-CTRL: Gugolas",
        "- EGER: Nezegetes",
        "- F1: Sugo bezarasa",
        "- ESC: Kilepes"
    };

    int startY = 100;
    for (int i = 0; i < 7; i++) {
        GLuint tex = textToTexture(font, lines[i]);
        
        // Use a standard height for lines, or get w/h from surface if preferred
        glBindTexture(GL_TEXTURE_2D, tex);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(100, startY + (i * 40));
            glTexCoord2f(1, 0); glVertex2f(400, startY + (i * 40));
            glTexCoord2f(1, 1); glVertex2f(400, startY + 35 + (i * 40));
            glTexCoord2f(0, 1); glVertex2f(100, startY + 35 + (i * 40));
        glEnd();
        
        glDeleteTextures(1, &tex);
    }

    // 4. Restore 3D
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

/**
 * Blindness effect-nek felhasználható lesz majd, esetleg a kuplungért be kell menni
 * egy házba ahol sötét lesz és ott megkeresni sötétben?
 */
static void enableFog(){
    GLfloat fogColor[4] = {0.5f,0.5f,0.5f,0.5f};
    glClearColor(0.5f,0.5f,0.5f,0.5f);
    glEnable(GL_FOG);
    glFogf(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 1.0f);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_END, 5.0f);
}

/**
 * Initializes particles
 */
static void initParticles(){
    for (int i = 0; i < maxParticle; i++){
        particles[i].x = (rand() % 20) - 10;
        particles[i].y = (rand() % 10);
        particles[i].z = (rand() % 20) - 10;
        particles[i].vy = -0.09f; // Lefelé eső (pl. eső vagy hó)
        particles[i].life = (float)rand() / RAND_MAX;
    }
};

/**
 * Displayes the particles, takes a cam so particles are generated even when the camera moves
 * and it always generates around the camera even when it moves
 */
static void displayParticles(Camera cam){
    glDisable(GL_TEXTURE_2D);
    glPointSize(5.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // A részecskék ne takarják ki egymást a mélységtesztben

    glBegin(GL_POINTS);
    for(int i = 0; i < maxParticle; i++) {
        if (particles[i].life > 0) {
            particles[i].y += particles[i].vy;
            particles[i].life -= 0.005f;

            //glColor4f(1.0f, 1.0f, 1.0f, particles[i].life);//szürke köd particle?
            glColor4f(0.2f, 0.4f, 1.0f, particles[i].life);//kék esőhöz
            glVertex3f(particles[i].x, particles[i].y,  particles[i].z);

            // Ha meghalt vagy leesett, a KAMERA KÖRÉ tegyük vissza
            if(particles[i].life <= 0 || particles[i].y < -1.0f) {
                particles[i].life = 1.0f;
                particles[i].y = cam.y + 5.0f; // Magasról essen
                
                // A kamera X és Z koordinátája köré rakjuk +- 20 egységgel
                particles[i].x = cam.x + (rand() % 40) - 20;
                particles[i].z = cam.z + (rand() % 40) - 20;
            }
        }
    }
    glEnd();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

#endif