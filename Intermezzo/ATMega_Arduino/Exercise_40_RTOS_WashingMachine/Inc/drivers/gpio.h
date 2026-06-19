/*
 * File:    gpio.h
 * Author:  Alexander130892
 * Date:    15-6-2026
 *
 * Description:
 *   Pin mapping and GPIO driver interface for the washing machine
 *   project. All hardware pin assignments are defined here.
 *   Peripherals are mapped according to the hardware specification.
 *
 *   Mapping:
 *       D2  / PD2   Start button        (INPUT_PULLUP, active low)
 *       D3  / PD3   Cancel button       (INPUT_PULLUP, active low)
 *       D4  / PD4   Door contact        (INPUT_PULLUP, active low)
 *       D5  / PD5   Status LED GREEN    (OUTPUT)
 *       D6  / PD6   Status LED BLUE     (OUTPUT)
 *       A0  / PC0   Water level sensor  (ADC input)
 *       D8  / PB0   Relay               (OUTPUT, active high)
 *       D9  / PB1   Servo               OUTPUT
 *       D10 / PB2   Motor               (OUTPUT, PWM or on/off)
 *       D11 / PB3   Buzzer              (OUTPUT, PWM via OC2A)
 *       D12 / PB4   Status LED RED        (OUTPUT)
 *       D13 / PB5   Internal LED - HB   (OUTPUT)
 */

#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

/* ----- Input pins ----------------------------------------- */
#define BTN_START_DDR       DDRD
#define BTN_START_PORT      PORTD
#define BTN_START_PIN_REG   PIND
#define BTN_START_PIN       PD2

#define BTN_CANCEL_DDR      DDRD
#define BTN_CANCEL_PORT     PORTD
#define BTN_CANCEL_PIN_REG  PIND
#define BTN_CANCEL_PIN      PD3

#define DOOR_DDR            DDRD
#define DOOR_PORT           PORTD
#define DOOR_PIN_REG        PIND
#define DOOR_PIN            PD4

/* ----- Status LEDs ---------------------------------------- */
#define LED_GREEN_DDR       DDRD
#define LED_GREEN_PORT      PORTD
#define LED_GREEN_PIN       PD5     // D5  — running states 

#define LED_BLUE_DDR        DDRD
#define LED_BLUE_PORT       PORTD
#define LED_BLUE_PIN        PD6     // D6  — idle/done      

#define LED_RED_DDR         DDRB
#define LED_RED_PORT        PORTB
#define LED_RED_PIN         PB4     // D12 — error          

#define LED_HB_DDR          DDRB
#define LED_HB_PORT         PORTB
#define LED_HB_PIN          PB5     // D13 — heartbeat   
#define LED_PORT            PORTB

/* ----- Actuator pins -------------------------------------- */
#define PUMP_DDR            DDRB
#define PUMP_PORT           PORTB
#define PUMP_PIN            PB0     // D8  — relay module, active LOW

#define VALVE_DDR           DDRB    // D9  — servo via Timer1 OC1A
#define VALVE_PORT          PORTB
#define VALVE_PIN           PB1

#define MOTOR_DDR           DDRB
#define MOTOR_PORT          PORTB
#define MOTOR_PIN           PB2     /* D10 — L298N ENA */

#define BUZZER_DDR          DDRB    // D11 — Timer2 OC2A
#define BUZZER_PORT         PORTB
#define BUZZER_PIN          PB3

/* ----- ADC ------------------------------------------------ */
#define WATER_LEVEL_CH      0       // A0
#define WATER_LEVEL_THRESH  512u    // 2.5V midpoint 

/* ----- Driver API ----------------------------------------- */
void    gpio_init(void);

bool    readStartButton(void);
bool    readCancelButton(void);
bool    readDoor(void);             // true = door closed 

void ledGreenOn(void);
void ledGreenOff(void);
void ledBlueOn(void);
void ledBlueOff(void);
void ledRedOn(void);
void ledRedOff(void);
void ledHbToggle(void); 

#endif /* GPIO_H */