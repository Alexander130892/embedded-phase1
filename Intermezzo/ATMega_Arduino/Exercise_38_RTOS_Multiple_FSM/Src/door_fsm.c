#include "door_fsm.h"
#include "events.h"
#include "timer.h"
#include "hw_uart.h"
#include "gpio.h"
#include <avr/io.h>

static DoorState door_state = DOOR_CLOSED;

DoorState door_fsm_get_state(void){
    return door_state;
}
void door_fsm_init(void) {
    door_state = DOOR_CLOSED;
    LED_PORT |= (1 << LED_B_PIN);   // set initial LED
}

void door_fsm_handle(Event ev){
    switch(door_state){
        case DOOR_CLOSED:   // BLUE LED
            if(ev.type == EV_BUTTON_START){
                // Exit
                LED_PORT &=  ~(1 << LED_B_PIN);   /* Toggle BLUE LED */
                // State change
                door_state = DOOR_OPENING;
                // Entry
                postEvent((Event){EV_DOOR_OPEN, 0});   // notify other FSMs
                LED_PORT |=  (1 << LED_G_PIN);   /* Toggle GREEN LED */
                hw_uart_print("DOOR STATE - Opening");
            }
            break;
        case DOOR_OPENING:  
            if(ev.type == EV_LIMIT_OPEN){
                //Exit
                LED_PORT &=  ~(1 << LED_G_PIN);   /* Toggle GREEN LED */
                // State change
                door_state = DOOR_OPEN;
                // Entry
                LED_PORT |=  (1 << LED_R_PIN);   /* Toggle RED LED */
                sw_timerStart(&timer_autoClose, 5000);
                hw_uart_print("DOOR STATE - Open");
            }
            break;
        case DOOR_OPEN:   
            if(ev.type == EV_BUTTON_START || ev.type == EV_TIMEOUT){
                //Exit
                LED_PORT &=  ~(1 << LED_R_PIN);   /* Toggle RED LED */
                // State change
                door_state = DOOR_CLOSING;
                // Entry
                LED_PORT |=  (1 << LED_G_PIN);   /* Toggle GREEN LED */
                hw_uart_print("DOOR STATE - Closing");
            }
            break;
        case DOOR_CLOSING:
            if(ev.type == EV_LIMIT_CLOSED){
                //Exit
                LED_PORT &=  ~(1 << LED_G_PIN);   /* Toggle GREEN LED */
                // State change
                door_state = DOOR_CLOSED;
                // Entry
                postEvent((Event){EV_DOOR_CLOSED, 0});  // notify other FSMs
                LED_PORT |=  (1 << LED_B_PIN);   /* Toggle BLUE LED */
                hw_uart_print("DOOR STATE - Closed");
            }
            break;
    }
}
void door_fsm_do_actions(void){
    if(sw_timerExpired(&timer_LED)){
        sw_timerStart(&timer_LED, 500);
        switch(door_state){
            case DOOR_CLOSED: 
                LED_PORT ^= (1 << LED_B_PIN);
                break;
            case DOOR_OPENING:  
                LED_PORT ^= (1 << LED_G_PIN);
                break;
            case DOOR_OPEN:   
                LED_PORT ^= (1 << LED_R_PIN);
                break;
            case DOOR_CLOSING:
                LED_PORT ^= (1 << LED_G_PIN);
                break;
        }
    }
}
