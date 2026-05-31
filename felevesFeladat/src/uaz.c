#include "uaz.h"

void drawUaz(Model* uaz, GLuint uazTexture, float lightLevel, Camera* cam) {
    float uazX = 59.0f;
    float uazZ = 57.0f;

    glBindTexture(GL_TEXTURE_2D, uazTexture);
    glColor3f(lightLevel, lightLevel, lightLevel);
    glPushMatrix();
        glTranslatef(uazX, -0.15f, uazZ);
        glRotatef(90.0f,0,1,0);
        draw_model(uaz);
    glPopMatrix();
    applyUazCollision(cam);
}

void applyUazCollision(Camera* cam) {
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