#include "door_fsm.h"
#include "events.h"
#include "timer.h"
#include "hw_uart.h"
#include "gpio.h"
#include <avr/io.h>

static DoorState door_state = STATE_CLOSED;

DoorState door_fsm_get_state(void){
    return door_state;
}
void door_fsm_init(void) {
    door_state = STATE_CLOSED;
    LED_PORT |= (1 << LED_B_PIN);   // set initial LED
}

void door_fsm_handle(Event ev){
    switch(door_state){
        case STATE_CLOSED:   // BLUE LED
            if(ev.type == EV_BUTTON_START){
                //Exit
                LED_PORT &=  ~(1 << LED_B_PIN);   /* Toggle BLUE LED */
                // State change
                door_state = STATE_OPENING;
                // Entry
                LED_PORT |=  (1 << LED_G_PIN);   /* Toggle GREEN LED */
                hw_uart_queue('O');
                hw_uart_queue('P');
                hw_uart_queue('E');
                hw_uart_queue('N');
                hw_uart_queue('I');
                hw_uart_queue('N');
                hw_uart_queue('G');
                hw_uart_queue('\r');
                hw_uart_queue('\n');
            }
            break;
        case STATE_OPENING:  
            if(ev.type == EV_LIMIT_OPEN){
                //Exit
                LED_PORT &=  ~(1 << LED_G_PIN);   /* Toggle GREEN LED */
                // State change
                door_state = STATE_OPEN;
                // Entry
                LED_PORT |=  (1 << LED_R_PIN);   /* Toggle RED LED */
                sw_timerStart(&timer_autoClose, 5000);
                hw_uart_queue('O');
                hw_uart_queue('P');
                hw_uart_queue('E');
                hw_uart_queue('N');
                hw_uart_queue('\r');
                hw_uart_queue('\n');
            }
            break;
        case STATE_OPEN:   
            if(ev.type == EV_BUTTON_START || ev.type == EV_TIMEOUT){
                //Exit
                LED_PORT &=  ~(1 << LED_R_PIN);   /* Toggle RED LED */
                // State change
                door_state = STATE_CLOSING;
                // Entry
                LED_PORT |=  (1 << LED_G_PIN);   /* Toggle GREEN LED */
                hw_uart_queue('C');
                hw_uart_queue('L');
                hw_uart_queue('O');
                hw_uart_queue('S');
                hw_uart_queue('I');
                hw_uart_queue('N');
                hw_uart_queue('G');
                hw_uart_queue('\r');
                hw_uart_queue('\n');
            }
            break;
        case STATE_CLOSING:
            if(ev.type == EV_LIMIT_CLOSED){
                //Exit
                LED_PORT &=  ~(1 << LED_G_PIN);   /* Toggle GREEN LED */
                // State change
                door_state = STATE_CLOSED;
                // Entry
                LED_PORT |=  (1 << LED_B_PIN);   /* Toggle BLUE LED */
                hw_uart_queue('C');
                hw_uart_queue('L');
                hw_uart_queue('O');
                hw_uart_queue('S');
                hw_uart_queue('E');
                hw_uart_queue('D');
                hw_uart_queue('\r');
                hw_uart_queue('\n');
            }
            break;
    }
}
void door_fsm_do_actions(void){
    if(sw_timerExpired(&timer_LED)){
        sw_timerStart(&timer_LED, 500);
        switch(door_state){
            case STATE_CLOSED: 
                LED_PORT ^= (1 << LED_B_PIN);
                break;
            case STATE_OPENING:  
                LED_PORT ^= (1 << LED_G_PIN);
                break;
            case STATE_OPEN:   
                LED_PORT ^= (1 << LED_R_PIN);
                break;
            case STATE_CLOSING:
                LED_PORT ^= (1 << LED_G_PIN);
                break;
        }
    }
}
