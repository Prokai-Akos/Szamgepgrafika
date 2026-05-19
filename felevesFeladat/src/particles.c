#include "particles.h"

void initParticles(){
    for (int i = 0; i < maxParticle; i++){
        particles[i].x = (rand() % 20) - 10;
        particles[i].y = (rand() % 10);
        particles[i].z = (rand() % 20) - 10;
        particles[i].vy = -0.09f; // Lefelé eső (pl. eső vagy hó)
        particles[i].life = (float)rand() / RAND_MAX;
    }
};

void displayParticles(Camera cam){
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