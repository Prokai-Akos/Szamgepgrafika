#ifndef PARTICLES_H
#define PARTICLES_H

#include "cam.h"
#include <GL/gl.h>
#include <stdlib.h>

/**
 * Particle struct
 * */
typedef struct {
    float x, y, z;
    float vy;//speed
    float life;//lifetime
} Particle;

#define maxParticle 600
static Particle particles[maxParticle];

/**
 * Initializes particles so they can be displayed later on
 */
void initParticles();

/**
 * Displayes the particles, takes a cam so particles are generated even when the camera moves
 * and it always generates around the camera even when it moves
 */
void displayParticles(Camera cam);
#endif