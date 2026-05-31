#include "includes/utils.h"

int main(int argc, char *argv[])
{    
    // Engine inicializálása
    if(initEngine() == 0) printf("Engine successfully initialized!\n");
      
    while (need_run){
        while (SDL_PollEvent(&event)) 
            handleEngineEvents(&event, &need_run, &showHelp, &storyActive, &currentStoryLine, currentMax, &currentLines, &doorLocked);

        handleEngineContinuousInput();
        
        if (currentState == STATE_CAR_REPAIRED && storyStates.carRepaired && !storyActive) endGameCam(&need_run);

        lightHandler();
        engineFogHandler();
        applyBorderCollision();
        engineSceneRender();

        updateStoryLogic(&storyActive, &currentLines, &currentMax, &currentStoryLine);        
        if (storyActive && currentLines != NULL) drawDialogueBox(font, currentLines[currentStoryLine]);

        if(showHelp) drawHelpMenu(font);
        SDL_GL_SwapWindow(window);
    }
    if (currentState == STATE_CAR_REPAIRED) SDL_Delay(2000); 
    engineGameEndHandler();

    return 0;
}