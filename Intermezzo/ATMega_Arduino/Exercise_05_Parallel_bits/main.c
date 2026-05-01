/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This AVR microcontroller program configures pin PC2 as an input
 *   with internal pull-up resistor to detect active-low button
 *   presses, though the LED toggle functionality inside the button
 *   detection logic is commented out and not implemented.
 */

#include <avr/io.h>
#include <util/delay.h>

/*
Exercise:
    Configure PC2 as input w/ internal pull-up
    Write an logic-test to check if button has been pressed
    Active-low
 */


int main(void)
{
    DDRC    &= ~(1 << 2);  //PC as input
    PORTC   |= (1 << 2);   //activate internall-pull up

    while (1) {
       if(!(PINC & (1 << 2))){
         //Toggle Led  
       }
    }

    return 0;   /* never reached */
}