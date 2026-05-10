#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "map.h"

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

#include "storyTexts.h"

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

typedef struct {
    float startX,endX;
    float startZ, endZ;
} Wall;

#define maxParticle 600
static Particle particles[maxParticle];

#define maxTrees 200
static Tree forest[maxTrees];

int mapseed = 67;

#define NUM_HOUSE_WALLS 5
static Wall houseWalls[NUM_HOUSE_WALLS] = {
    {48.0f, 48.5f, 51.0f, 58.0f}, // hátsó
    {48.0f, 52.5f, 57.5f, 58.0f}, //bal oldali
    {48.0f, 52.5f, 51.5f, 52.0f}, // Első fal (ajtóig)
    {52.0f, 52.5f, 52.0f, 54.5f}, // Jobb oldali fal A (ajtó előtt)
    {52.0f, 52.5f, 55.5f, 58.0f}  // Jobb oldali fal B (ajtó után)
};
static Wall doorCollision = {52.0f, 52.5f, 54.5f, 55.5f};//zárt ajtóhoz


/**
 * Taken from M_PI definition of math.h, using M_PI works
 * but the file has an error since it only gets replaced at compile time,
 * i didnt want to see the error so i just did it this way 
 */
static double pi = 3.14159265358979323846;

Model tree;
Model cardboardBox;
Model house;
Model roof;
Model door;
Model uaz;

static int loadModels(){
    if (!load_model(&tree, "assets/tree/tree.obj")){
        printf("Failed to load tree model!\n");
        return 1;
    }
    else scale_model(&tree, 2.0, 2.0, 2.0); 

    if(!load_model(&cardboardBox, "assets/box/carboard-box.obj")) {
        printf("Failed to load cardboard box model!\n");
        return 1;
    }
    else scale_model(&cardboardBox, 0.2, 0.2, 0.2); 

    if(!load_model(&house,"assets/house/house_walls.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&house, 1.0, 1.0, 1.0); 

    if(!load_model(&roof,"assets/house/roof.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&roof, 1.0, 1.0, 0.85); 
    
    if(!load_model(&door,"assets/house/door.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&door, 0.3, 0.3, 0.3);

    if(!load_model(&uaz, "assets/uaz/highQualityUAZ.obj")){
        printf("Failed to load uaz model!\n");
        return 1;
    }
    else scale_model(&uaz, 2.0, 2.0, 2.0); 

    return 0;
}

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
        "- + - : set light level",
        "- SPACE: progress dialogue"
    };

    int startY = 100;
    for (int i = 0; i < 9; i++) {
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

static void drawDialogueBox(TTF_Font* font, const char* text) {
    if (!font || !text || text[0] == '\0') return;

    // 1. Átváltás 2D-be
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
 * Applies the collision box for the house walls
 * leaves a gap for the door so it can be accessed
 */
static void applyWallCollision(Camera* cam, Wall w) {
    float buffer = 0.2f;
    
    if (cam->x > w.startX - buffer && cam->x < w.endX + buffer &&
        cam->z > w.startZ - buffer && cam->z < w.endZ + buffer) {
        
        // Kiszámoljuk, melyik irányból vagyunk a legközelebb a széléhez (behatolás mértéke)
        float overlapXmin = cam->x - (w.startX - buffer);
        float overlapXmax = (w.endX + buffer) - cam->x;
        float overlapZmin = cam->z - (w.startZ - buffer);
        float overlapZmax = (w.endZ + buffer) - cam->z;

        // Megkeressük a legkisebb overlap-et (merre a legkönnyebb kitolni)
        float minOverlap = overlapXmin;
        int direction = 0; // 0: X min, 1: X max, 2: Z min, 3: Z max

        if (overlapXmax < minOverlap) { minOverlap = overlapXmax; direction = 1; }
        if (overlapZmin < minOverlap) { minOverlap = overlapZmin; direction = 2; }
        if (overlapZmax < minOverlap) { minOverlap = overlapZmax; direction = 3; }

        // Kilökés a megfelelő irányba
        switch (direction) {
            case 0: cam->x -= minOverlap; break;
            case 1: cam->x += minOverlap; break;
            case 2: cam->z -= minOverlap; break;
            case 3: cam->z += minOverlap; break;
        }
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
    if (!boxPickedUp) {
        float time = SDL_GetTicks() / 1000.0f;     
        float bobbing = sinf(time * 3.0f) * 0.15f; 
        float rotation = time * 60.0f;            

        glBindTexture(GL_TEXTURE_2D, cardboardTexture);
        glColor3f(lightLevel, lightLevel, lightLevel);

        float dx = cam.x - boxX;
        float dz = cam.z - boxZ;
        float distance = sqrtf(dx * dx + dz * dz);

        glPushMatrix();
            glTranslatef(boxX, -0.8f + bobbing, boxZ);  
            glRotatef(rotation, 0, 1, 0); 
            draw_model(cardboardBox);
        glPopMatrix();

        if (distance < 1.0f) boxPickedUp = true;
    }
    return boxPickedUp;
}

/**
 * Draws the background walls
 * AI was used here
 */
static void drawBackgroundWalls(GLuint mountainTexture, float lightLevel) {
    float min = 0.0f;
    float max = 80.0f;
    float height = 3.0f;
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

/**
 * Draws the entire cottage, walls, roof, door
 * AI was used here
 */
static void drawCottage(Model* walls, Model* roof, Model* door, GLuint wallTexture, GLuint roofTexture, GLuint doorTexture, float lightLevel){
    float cottageX = 50.5;
    float cottageZ = 55;
    float repeatCount = 3.0f; 

    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glColor3f(lightLevel, lightLevel, lightLevel);

    // Átváltunk textúra módba, hogy módosítsuk a koordinátákat
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glScalef(repeatCount, repeatCount, 1.0f); // Itt skálázzuk fel az ismétlődést

    glMatrixMode(GL_MODELVIEW); // Visszaváltunk a rajzoláshoz
    glPushMatrix();
        glTranslatef(cottageX, -1.0f, cottageZ);  
        draw_model(walls);
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, roofTexture);
    glPushMatrix();
        glTranslatef(cottageX, 1.0f, cottageZ);  
        draw_model(roof);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, doorTexture);
    glPushMatrix();
        glTranslatef(cottageX+2, -1.0f, cottageZ);  
        draw_model(door);
    glPopMatrix();


    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

static bool isInsideCottage(Camera cam) {
    return (cam.x > 48.5f && cam.x < 52.0f &&
            cam.z > 52.0f && cam.z < 57.5f);
}

/**
 * Draws the UAZ model
 */
static void drawUaz(Model* uaz, GLuint uazTexture, float lightLevel) {
    float uazX = 59.0f;
    float uazZ = 57.0f;

    glBindTexture(GL_TEXTURE_2D, uazTexture);
    glColor3f(lightLevel, lightLevel, lightLevel);
    glPushMatrix();
        glTranslatef(uazX, -0.15f, uazZ);
        glRotatef(90.0f,0,1,0);
        draw_model(uaz);
    glPopMatrix();

}

/*
* Used to apply a collision box to the UAZ model
* AI was used for writing this
*/
static void applyUazCollision(Camera* cam) {
    float minX = 58.0f;
    float maxX = 60.0f;
    float minZ = 54.5f;
    float maxZ = 59.5f;
    
    float buffer = 0.1f;
    minX -= buffer; maxX += buffer;
    minZ -= buffer; maxZ += buffer;

    if (cam->x > minX && cam->x < maxX && cam->z > minZ && cam->z < maxZ) {
        
        float distLeft   = cam->x - minX;
        float distRight  = maxX - cam->x;
        float distBack   = cam->z - minZ;
        float distFront  = maxZ - cam->z;

        float minDist = distLeft;
        int direction = 0; // 0: Bal, 1: Jobb, 2: Hátul, 3: Elöl

        if (distRight < minDist) { minDist = distRight; direction = 1; }
        if (distBack < minDist)  { minDist = distBack;  direction = 2; }
        if (distFront < minDist) { minDist = distFront; direction = 3; }

        // Kilökés a legközelebbi határhoz
        switch (direction) {
            case 0: cam->x = minX; break;
            case 1: cam->x = maxX; break;
            case 2: cam->z = minZ; break;
            case 3: cam->z = maxZ; break;
        }
    }
}

#endif