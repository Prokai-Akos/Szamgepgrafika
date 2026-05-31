#include "loadTextures.h"

Textures textures;

void initTextures(Textures* textures){
    textures->grass = loadTexture("assets/grass.jpg");
    textures->dirt = loadTexture("assets/dirt.jpg");
    textures->stoneWall = loadTexture("assets/stoneWall.jpg");
    textures->redBrick = loadTexture("assets/brick.jpg");
    textures->greyBrick = loadTexture("assets/grey_brick.jpg");
    textures->doorTexture = loadTexture("assets/door.jpg");
    textures->spruceTexture = loadTexture("assets/tree/tree-nonopaque.png");
    textures->cardboardTexture = loadTexture("assets/box/cardboard.jpg");
    textures->uazTexture = loadTexture("assets/qualityUAZTexture.png");
}

GLuint loadTexture(const char *filename){

    SDL_Surface *loadedSurf = IMG_Load(filename);
    if (!loadedSurf)
    {
        printf("Failed to load image: %s\n", IMG_GetError());
        return 0;
    }

    // SDL_ConvertSurfaceFormat-tal fixen ABGR8888-ra alakítjuk,
    // így az OpenGL oldalon ez mindig GL_RGBA lesz.
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(loadedSurf, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(loadedSurf);

    if (!surface)
        return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Csak a sima alap textúrát töltjük fel (Mipmap generálás nélkül)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    // Mipmap helyett sima LINEAR (lineáris) szűrést kérünk le- és felskálázásnál is
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    SDL_FreeSurface(surface);
    return textureID;
}
