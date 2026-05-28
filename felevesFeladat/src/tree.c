#include "tree.h"

Tree forest[maxTrees];

void initTrees(){
    srand(mapseed);
    for (int i = 0; i < maxTrees; i++) {
        forest[i].x = (float)(rand() %  WIDTH);
        forest[i].z = (float)(rand() % HEIGHT);
    }
}

void drawTrees(Model* tree, GLuint texture, int treeCount, float centerX, float centerZ, Camera* cam) {
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

void applyTreeCollision(Camera* cam, float treeX, float treeZ) {
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