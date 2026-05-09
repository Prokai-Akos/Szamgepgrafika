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
#include <time.h>

//includes for model loader from gitlab repo
#include "map.h"
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

typedef struct {
    float x;
    float z;
} Tree;

#define maxParticle 600
static Particle particles[maxParticle];

#define maxTrees 200
static Tree forest[maxTrees];

int mapseed = 67;



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

    SDL_Surface* loadedSurf = IMG_Load(filename);
    if (!loadedSurf) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return 0;
    }

    int mode = (loadedSurf->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(loadedSurf, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(loadedSurf);

    if (!surface) return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Mipmap építése (Figyelem: mivel kényszerítettük az ABGR-t, 
    // az OpenGL felé itt fixen GL_RGBA-t küldünk!)
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, surface->w, surface->h, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    // Szűrés beállítása
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

/*
*Used for displaying the help menu
*/
static void drawHelpMenu(TTF_Font* font) {
    //AI was used to help write this
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
        "Help menu",
        "-------------------",
        "- W,A,S,D: movement",
        "- L-CTRL: crouch",
        "- MOUSE: Look around",
        "- F1: help menu",
        "- ESC: exit",
        "- + - : set light level"
    };

    int startY = 100;
    for (int i = 0; i < 8; i++) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* tempSurf = TTF_RenderUTF8_Blended(font, lines[i], white);
        if (!tempSurf) continue;

        int textW = tempSurf->w;
        int textH = tempSurf->h;

        GLuint tex = textToTexture(font, lines[i]);
        
        glBindTexture(GL_TEXTURE_2D, tex);
        
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(100, startY + (i * 40));
            glTexCoord2f(1, 0); glVertex2f(100 + textW, startY + (i * 40));
            glTexCoord2f(1, 1); glVertex2f(100 + textW, startY + textH + (i * 40));
            glTexCoord2f(0, 1); glVertex2f(100, startY + textH + (i * 40));
        glEnd();
        
        SDL_FreeSurface(tempSurf);
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
 * Enables fog and changes clearColor to fog color
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

/**
 * Used for checking the bounding box at the border of the map
 */
static float clamp(float n, float min, float max) {
  if (n < min) return min;
  if (n > max) return max;
  return n;
}

/**
 * Ütközésvizsgálat egy kör alapú objektummal.
 * Figyelembe veszi a guggolást: ha a kamera alacsonyan van, 
 * csak a törzzsel (trunk) ütközik, ha magasan, akkor a lombbal (foliage) is.
 */
static void applyTreeCollision(Camera* cam, float treeX, float treeZ) {
    // Hitbox paraméterek
    float trunkRadius = 0.4f;
    float foliageRadius = 1.8f;    
    float foliageHeightLimit = -0.3f;

    float dx = cam->x - treeX;
    float dz = cam->z - treeZ;
    float distance = sqrtf(dx * dx + dz * dz);

    // melyik hitbox érvényes
    float activeRadius = trunkRadius;
    if (cam->y > foliageHeightLimit) activeRadius = foliageRadius;

    // kamera kilökés
    if (distance < activeRadius && distance > 0.001f) {
        float overlap = activeRadius - distance;
        cam->x += (dx / distance) * overlap;
        cam->z += (dz / distance) * overlap;
    }
}

/**
 * Initializes the position of the trees on the map
 */
static void initTrees(){
    srand(mapseed);
    for (int i = 0; i < maxTrees; i++) {
        forest[i].x = (float)(rand() %  WIDTH);
        forest[i].z = (float)(rand() % HEIGHT);
    }
}

/**
 * Displays the trees
 */
static void drawTrees(Model* tree, GLuint texture, int treeCount, float centerX, float centerZ, Camera* cam) {
    glBindTexture(GL_TEXTURE_2D, texture);

    // Átlátszóság bekapcsolása (hogy ne legyen fekete/szürke kerete a leveleknek)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);

    for(int i = 0; i < maxTrees; i++) {
        glPushMatrix();
            glTranslatef(forest[i].x, -1.1f, forest[i].z);

            // fát elforgatunk egy kicsit, hogy ne nézzen mind ugyanarra
            glRotatef(i * 137.5f, 0, 1, 0); 
            
            draw_model(tree);
        glPopMatrix();
        applyTreeCollision(cam,forest[i].x, forest[i].z);
    }
    
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

/**
 * Generates the box into a random position on the map, 
 * implements checking if the box is picked up already
 */
static bool generateBox(Model* cardboardBox, GLuint cardboardTexture, float boxX, float boxZ, bool boxPickedUp, Camera cam, float lightLevel){
    glBindTexture(GL_TEXTURE_2D, cardboardTexture);
        glColor3f(lightLevel, lightLevel, lightLevel);

        if (!boxPickedUp) {
            float dx = cam.x - boxX;
            float dz = cam.z - boxZ;
            float distance = sqrtf(dx * dx + dz * dz);
            glBindTexture(GL_TEXTURE_2D, cardboardTexture);
            glColor3f(lightLevel, lightLevel, lightLevel);

            glPushMatrix();
                glTranslatef(boxX, -1.0f, boxZ);  
                draw_model(cardboardBox);
            glPopMatrix();

            if (distance < 1.0f) {
                printf("\nBox picked up! Position: (%.2f, %.2f)\n", boxX, boxZ);
                boxPickedUp = true;
            }
        }
    return boxPickedUp;
}

/**
 * Draws the background walls
 */
static void drawBackgroundWalls(GLuint mountainTexture, float lightLevel) {
    float min = 0.0f;
    float max = 80.0f;
    float height = 2.0f;
    float repeat = 20.0f;

    glBindTexture(GL_TEXTURE_2D, mountainTexture);
    glColor3f(lightLevel, lightLevel, lightLevel);

    glBegin(GL_QUADS);
        // HÁTSÓ FAL
        glTexCoord2f(0, 1);      glVertex3f(min, -1.0f, min); // Itt 0 helyett 1 (vagy amekkora a magasság)
        glTexCoord2f(repeat, 1); glVertex3f(max, -1.0f, min);
        glTexCoord2f(repeat, 0); glVertex3f(max, height, min); // Itt 1 helyett 0
        glTexCoord2f(0, 0);      glVertex3f(min, height, min);

        // ELSŐ FAL
        glTexCoord2f(0, 1);      glVertex3f(min, -1.0f, max);
        glTexCoord2f(repeat, 1); glVertex3f(max, -1.0f, max);
        glTexCoord2f(repeat, 0); glVertex3f(max, height, max);
        glTexCoord2f(0, 0);      glVertex3f(min, height, max);

        // BAL OLDALI FAL
        glTexCoord2f(0, 1);      glVertex3f(min, -1.0f, min);
        glTexCoord2f(repeat, 1); glVertex3f(min, -1.0f, max);
        glTexCoord2f(repeat, 0); glVertex3f(min, height, max);
        glTexCoord2f(0, 0);      glVertex3f(min, height, min);

        // JOBB OLDALI FAL
        glTexCoord2f(0, 1);      glVertex3f(max, -1.0f, min);
        glTexCoord2f(repeat, 1); glVertex3f(max, -1.0f, max);
        glTexCoord2f(repeat, 0); glVertex3f(max, height, max);
        glTexCoord2f(0, 0);      glVertex3f(max, height, min);
    glEnd();
}

#endif