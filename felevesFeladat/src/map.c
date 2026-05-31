#include "map.h"

float tileSize = 1.0f;
int tileMap[WIDTH][HEIGHT];
GLuint mapDisplayList = 0;

void drawMap() {
    if (mapDisplayList != 0) {
        glCallList(mapDisplayList); // BUMM! Egyetlen hívással kirajzolja a 6400 tile-t
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void displayCoordinatesInTerminal(Camera* cam){
    printf("X: %6.2f | Y: %6.2f | Z: %6.2f\r", cam->x, cam->y, cam->z);
    fflush(stdout);
}

void drawSingleTile(int i, int j) {
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(i * tileSize, -1.0f, j * tileSize);
        glTexCoord2f(1.0f, 0.0f); glVertex3f((i+1) * tileSize, -1.0f, j * tileSize);
        glTexCoord2f(1.0f, 1.0f); glVertex3f((i+1) * tileSize, -1.0f, (j+1) * tileSize);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(i * tileSize, -1.0f, (j+1) * tileSize);
    glEnd();
}

void initMapMesh() {
    FILE* file = fopen("assets/map.txt", "r");
    
    if (!file) {
        printf("Couldn't open file! Generating full grass backup...\n");
        for(int i = 0; i < WIDTH; i++) {
            for(int j = 0; j < HEIGHT; j++) {
                tileMap[i][j] = 0;
            }
        }
    } else {
        for (int j = 0; j < HEIGHT; j++) {
            for (int i = 0; i < WIDTH; i++) {
                int ch;
                do {
                    ch = fgetc(file);
                } while (ch == '\n' || ch == '\r');

                if (ch == EOF) {
                    tileMap[i][j] = 0; 
                } else {
                    tileMap[i][j] = ch - '0'; 
                }
            }
        }
        fclose(file);
        printf("Assets/map.txt sikeresen beolvasva!\n");
    }

    mapDisplayList = glGenLists(1);
    glNewList(mapDisplayList, GL_COMPILE);
        
        glBindTexture(GL_TEXTURE_2D, textures.grass);
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                if (tileMap[i][j] == 0) drawSingleTile(i, j);
            }
        }

        glBindTexture(GL_TEXTURE_2D, textures.dirt);
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                if (tileMap[i][j] == 1) drawSingleTile(i, j);
            }
        }

    glEndList();
}