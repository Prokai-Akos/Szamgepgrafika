#include "backgroundWalls.h"

void drawBackgroundWalls(GLuint mountainTexture, float lightLevel) {
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


float clamp(float n, float min, float max) {
  if (n < min) return min;
  if (n > max) return max;
  return n;
}
