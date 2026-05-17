#include "gameStates.h"

typedef enum{
    STATE_INTRO,
    STATE_DOOR_LOCKED,
    STATE_CROUCH_DONE,
    STATE_INSIDE_HOUSE,
    STATE_BOX1_PICKUP,
    STATE_NO_CLUTCH,
    STATE_BOX2_PICKUP,
    STATE_CAR_REPAIRED,
    STATE_COUNT
} GameState;

GameState currentState = STATE_INTRO;