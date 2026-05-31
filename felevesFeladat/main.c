#include "includes/utils.h"

int main(int argc, char *argv[])
{    
    // Engine inicializálása
    if(initEngine() == 0) printf("Engine successfully initialized!\n");
      
    while (need_run){
        while (SDL_PollEvent(&event)) 
            handleEngineEvents(&event, &need_run, &showHelp, &storyActive, &currentStoryLine, currentMax, &currentLines, &doorLocked);
        
        handleEngineContinuousInput();
        
        // JÁTÉK VÉGE: Az új állapotgép alapján nézzük, nem a régi storyStates-szel!
        if (currentState == STATE_CAR_REPAIRED && storyStates.carRepaired && !storyActive) {
            endGameCam(&need_run);
        }

        lightHandler();
        engineFogHandler();
        applyBorderCollision();
        
        // Meghívjuk a paraméter nélküli renderelést
        engineSceneRender();

        // SZTORI LOGIKA: Kiszedtük a doorLocked-ot a végéről, mert a texts.c már nem használja!
        updateStoryLogic(&storyActive, &currentLines, &currentMax, &currentStoryLine);
        
        if (storyActive && currentLines != NULL) drawDialogueBox(font, currentLines[currentStoryLine]);

        if(showHelp) drawHelpMenu(font);
        SDL_GL_SwapWindow(window);
    }
    
    // Ha vége a játéknak, várunk 2 másodpercet
    if (currentState == STATE_CAR_REPAIRED) SDL_Delay(2000); 

    engineGameEndHandler();

    return 0;
}