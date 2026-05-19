#include "sound.h"

Mix_Music *backgroundMusic;
char* source = "assets/sound/geoffharvey-creepy-hollow-369570.mp3";


void initSound(){
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) printf("SDL_mixer error: %s\n", Mix_GetError());
    Mix_Init(MIX_INIT_MP3);

}

void playMusic(Mix_Music *backgroundMusic, char* source){
    backgroundMusic = Mix_LoadMUS(source);
    if (!backgroundMusic) printf("Failed to load music: %s\n", Mix_GetError());
    else Mix_PlayMusic(backgroundMusic, -1); //-1 a looped play
}