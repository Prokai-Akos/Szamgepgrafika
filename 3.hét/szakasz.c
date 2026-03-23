#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct {
    unsigned int R;
    unsigned int G;
    unsigned int B;
} Color;

typedef struct{
    int startingPoint[2];
    int endPoint[2];
    Color color;
}Line;

int main(int argc, char* argv[]){ 

    int x;
    int y;
    Line line;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Szakasz window",
                                SDL_WINDOWPOS_CENTERED, //x
                                SDL_WINDOWPOS_CENTERED, //y
                                800, // width
                                600, //height
                                SDL_WINDOW_SHOWN); //látni se ártana
                                
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_bool quit = SDL_FALSE;
    SDL_Event event;
    int clickCount = 0;
    int MAX_LINE_COUNT = 5;
    int lineCount = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = SDL_TRUE;
            }
            switch (event.type)
            {
            case SDL_MOUSEBUTTONUP:
                SDL_GetMouseState(&x, &y);
    
                if (clickCount == 0) {
                    line.startingPoint[0] = x;
                    line.startingPoint[1] = y;
                    
                    line.endPoint[0] = x;
                    line.endPoint[1] = y;
                    
                    clickCount = 1;
                    printf("Start set: %d, %d\n", x, y);
                }
                else {
                    line.endPoint[0] = x;
                    line.endPoint[1] = y;
                    
                    clickCount = 0; 
                    printf("End set: %d, %d\n", x, y);
                    lineCount++;
                }

                break;
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer,line.startingPoint[0], line.startingPoint[1], line.endPoint[0], line.endPoint[1]);
        SDL_RenderPresent(renderer);


    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}