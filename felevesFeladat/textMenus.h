#ifndef HELP_MENU_H
#define HELP_MENU_H

#include "gameHeaders.h"

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
        "- + - : set light level"
    };

    int startY = 100;
    for (int i = 0; i < 8; i++) {
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

#endif