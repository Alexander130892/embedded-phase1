/*
 * File:    gpio.c
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This file provides GPIO initialization and control functions for
 *   an AVR microcontroller-based device with LEDs (green, blue, red,
 *   heartbeat), buttons (start, cancel), a door sensor, and actuators
 *   (pump, valve, motor, buzzer). It sets up output pins for the LEDs
 *   and actuators, configures input pins with pull-ups for the buttons
 *   and door sensor, and provides simple on/off/toggle functions for
 *   each component.
 */
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include <avr/io.h>

void gpio_init(void)
{
    // Outputs
    LED_GREEN_DDR   |= (1u << LED_GREEN_PIN);
    LED_BLUE_DDR    |= (1u << LED_BLUE_PIN);
    LED_RED_DDR     |= (1u << LED_RED_PIN);
    LED_HB_DDR      |= (1u << LED_HB_PIN);
    PUMP_DDR        |= (1u << PUMP_PIN);
    VALVE_DDR       |= (1u << VALVE_PIN);
    MOTOR_DDR       |= (1u << MOTOR_PIN);
    BUZZER_DDR      |= (1u << BUZZER_PIN);

    // Inputs with pull-ups
    BTN_START_DDR   &= ~(1u << BTN_START_PIN);
    BTN_START_PORT  |=  (1u << BTN_START_PIN);
    BTN_CANCEL_DDR  &= ~(1u << BTN_CANCEL_PIN);
    BTN_CANCEL_PORT |=  (1u << BTN_CANCEL_PIN);
    DOOR_DDR        &= ~(1u << DOOR_PIN);
    DOOR_PORT       |=  (1u << DOOR_PIN);

    // Safe initial output states 
    PUMP_PORT       |=  (1u << PUMP_PIN);   // relay off (active LOW)


}

void ledGreenOn(void){ 
    LED_GREEN_PORT |=  (1u << LED_GREEN_PIN);
}
void ledGreenOff(void){ 
    LED_GREEN_PORT &= ~(1u << LED_GREEN_PIN); 
}
void ledBlueOn(void){ 
    LED_BLUE_PORT  |=  (1u << LED_BLUE_PIN);  
}
void ledBlueOff(void){ 
    LED_BLUE_PORT  &= ~(1u << LED_BLUE_PIN);  
}
void ledRedOn(void){ 
    LED_RED_PORT   |=  (1u << LED_RED_PIN);   
}
void ledRedOff(void){ 
    LED_RED_PORT   &= ~(1u << LED_RED_PIN);   
}
void ledHbToggle(void){ 
    LED_HB_PORT    ^=  (1u << LED_HB_PIN);    
}
bool readStartButton(void){
    return (BTN_START_PIN_REG & (1u << BTN_START_PIN));
}
bool readCancelButton(void){
    return (BTN_CANCEL_PIN_REG & (1u << BTN_CANCEL_PIN));
}
bool readDoor(void){
    return (DOOR_PIN_REG & (1u << DOOR_PIN));
}


