#ifndef TREE_H
#define TREE_H

#include "obj/include/model.h"
#include <GL/gl.h>
#include "cam.h"
#include <math.h>
#include "utils.h"

typedef struct {
    float x;
    float z;
} Tree;

#define maxTrees 200
extern Tree forest[maxTrees];

/**
 * Initializes the position of the trees on the map
 */
void initTrees();

/**
 * Displays the trees, also applies collisions to them
 */
void drawTrees(Model* tree, GLuint texture, Camera* cam);
    
/**
 * Ütközésvizsgálat egy kör alapú objektummal.
 * Figyelembe veszi a guggolást: ha a kamera alacsonyan van, 
 * csak a törzzsel (trunk) ütközik, ha magasan, akkor a lombbal (foliage) is.
 */
void applyTreeCollision(Camera* cam, float treeX, float treeZ);

#endif