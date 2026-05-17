#ifndef PARTICLES_H
#define PARTICLES_H

#include "gameHeaders.h"

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
static void initParticles();

/**
 * Displayes the particles, takes a cam so particles are generated even when the camera moves
 * and it always generates around the camera even when it moves
 */
static void displayParticles(Camera cam);
#endif