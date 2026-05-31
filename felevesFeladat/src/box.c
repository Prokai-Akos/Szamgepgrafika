#include "box.h"

float boxX, boxZ;
BoxHandler boxHandler;

void boxGenerator(){
    srand(time(NULL));
    boxX = (float)(rand() % 20) + 1.0f; // 1 és 21 közötti random koordináta
    boxZ = (float)(rand() % 20) + 1.0f; 
    printf(" Box generated: X: %.2f, Z: %.2f\n", boxX, boxZ);
}

bool generateBox(Model* cardboardBox, GLuint cardboardTexture, float boxX, float boxZ, bool boxPickedUp, Camera* cam, float lightLevel){
    if (!boxPickedUp) {
        float time = SDL_GetTicks() / 1000.0f;     
        float bobbing = sinf(time * 3.0f) * 0.15f; 
        float rotation = time * 60.0f;            

        glBindTexture(GL_TEXTURE_2D, cardboardTexture);
        glColor3f(lightLevel, lightLevel, lightLevel);

        float dx = cam->x - boxX;
        float dz = cam->z - boxZ;
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

void initBoxes(){
    boxHandler.carKeyBoxPickedUP = false;
    boxHandler.clutchBoxPickedUp = false;
}

void drawBoxes(){
    boxHandler.carKeyBoxPickedUP = generateBox(&models.cardboardBox, textures.cardboardTexture, 49.5, 54.5, boxHandler.carKeyBoxPickedUP, &cam, currentLight);
    boxHandler.clutchBoxPickedUp = generateBox(&models.cardboardBox, textures.cardboardTexture, boxX, boxZ, boxHandler.clutchBoxPickedUp, &cam, currentLight);
}

