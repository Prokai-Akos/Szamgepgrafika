#include "loadModels.h"

Model tree;
Model cardboardBox;
Model house;
Model roof;
Model door;
Model uaz;

GLuint loadTexture(const char* filename) {

SDL_Surface* loadedSurf = IMG_Load(filename);
    if (!loadedSurf) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return 0;
    }

    // SDL_ConvertSurfaceFormat-tal fixen ABGR8888-ra alakítjuk,
    // így az OpenGL oldalon ez mindig GL_RGBA lesz.
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(loadedSurf, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(loadedSurf);

    if (!surface) return 0;

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

int loadModels(){
    if (!load_model(&tree, "assets/tree/tree.obj")){
        printf("Failed to load tree model!\n");
        return 1;
    }
    else scale_model(&tree, 2.0, 2.0, 2.0); 

    if(!load_model(&cardboardBox, "assets/box/carboard-box.obj")) {
        printf("Failed to load cardboard box model!\n");
        return 1;
    }
    else scale_model(&cardboardBox, 0.2, 0.2, 0.2); 

    if(!load_model(&house,"assets/house/house_walls.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&house, 1.0, 1.0, 1.0); 

    if(!load_model(&roof,"assets/house/roof.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&roof, 1.0, 1.0, 0.85); 
    
    if(!load_model(&door,"assets/house/door.obj")){
        printf("Failed to load cottage model!\n");
        return 1;
    }
    else scale_model(&door, 0.3, 0.3, 0.3);

    if(!load_model(&uaz, "assets/uaz/highQualityUAZ.obj")){
        printf("Failed to load uaz model!\n");
        return 1;
    }
    else scale_model(&uaz, 2.0, 2.0, 2.0); 

    return 0;
}
