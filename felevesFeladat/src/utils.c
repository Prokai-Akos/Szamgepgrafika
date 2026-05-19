#include "utils.h"

int mapseed = 67;

double pi = 3.14159265358979323846;

Wall houseWalls[NUM_HOUSE_WALLS] = {
    {48.0f, 48.5f, 51.0f, 58.0f}, // hátsó
    {48.0f, 52.5f, 57.5f, 58.0f}, //bal oldali
    {48.0f, 52.5f, 51.5f, 52.0f}, // Első fal (ajtóig)
    {52.0f, 52.5f, 52.0f, 54.5f}, // Jobb oldali fal A (ajtó előtt)
    {52.0f, 52.5f, 55.5f, 58.0f}  // Jobb oldali fal B (ajtó után)
};
Wall doorCollision = {52.0f, 52.5f, 54.5f, 55.5f};//zárt ajtóhoz

void drawHelpMenu(TTF_Font* font) {
    //AI was used to help write this
    // 1. Switch to 2D
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
        "- + - , F5,F6: set light level",
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

void enableFog(){
    GLfloat fogColor[4] = {0.5f,0.5f,0.5f,0.5f};
    glClearColor(0.5f,0.5f,0.5f,0.5f);
    glEnable(GL_FOG);
    glFogf(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 1.0f);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_END, 5.0f);
}

void applyWallCollision(Camera* cam, Wall w) {
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

bool generateBox(Model* cardboardBox, GLuint cardboardTexture, float boxX, float boxZ, bool boxPickedUp, Camera cam, float lightLevel){
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

void drawCottage(Model* walls, Model* roof, Model* door, GLuint wallTexture, GLuint roofTexture, GLuint doorTexture, float lightLevel){
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
        glTranslatef(cottageX, -0.99f, cottageZ);  //0.99f a-fighting miatt
        draw_model(walls);
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, roofTexture);
    glPushMatrix();
        glTranslatef(cottageX, 1.02f, cottageZ);  //hogy ne clipeljen be a tető a plafonba
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