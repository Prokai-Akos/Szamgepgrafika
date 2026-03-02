#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct {
    unsigned int R;
    unsigned int G;
    unsigned int B;
} Color;

typedef struct{
    float startingPoint[2];
    float endPoint[2];
    Color color;
}Line;

int main(int argc, char* argv[]){ 

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("The first window",
                                SDL_WINDOWPOS_CENTERED, //x
                                SDL_WINDOWPOS_CENTERED, //y
                                800, // width
                                600, //height
                                SDL_WINDOW_SHOWN); //látni se ártana
                                
    SDL_bool quit = SDL_FALSE;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = SDL_TRUE;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}