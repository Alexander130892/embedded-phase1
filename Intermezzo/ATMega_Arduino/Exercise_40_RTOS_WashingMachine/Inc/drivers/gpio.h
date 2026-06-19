/*
 * File:    gpio.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file defines GPIO pin mappings and provides a driver
 *   API for an AVR microcontroller-based washing machine controller,
 *   managing input buttons (start/cancel/door), status LEDs
 *   (green/blue/red/heartbeat), actuators (pump/valve/motor/buzzer),
 *   and an ADC water level sensor.
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