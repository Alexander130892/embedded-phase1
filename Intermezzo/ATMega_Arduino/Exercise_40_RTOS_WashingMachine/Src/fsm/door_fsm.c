#include "door_fsm.h"

static door_state_t door_state = DOOR_CLOSED;

void door_fsm_init(void) {
    door_state = DOOR_CLOSED;
}

door_state_t door_fsm_get_state(void) {
    return door_state;
}

void door_fsm_update(door_state_t new_state) {
    door_state = new_state;
}