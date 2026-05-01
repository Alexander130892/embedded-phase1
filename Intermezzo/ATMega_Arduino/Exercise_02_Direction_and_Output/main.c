/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This AVR microcontroller program configures pins PB5 and PB4 as
 *   outputs, sets PB5 high and PB4 low, then enters an infinite loop.
 *   It demonstrates basic GPIO pin configuration and control using
 *   bitmask operations.
 */

#include <avr/io.h>
#include <util/delay.h>

/*
Exercise:
    Write code to configure PB5 and 4 as output.
    Set PB5 and Reset PB4.
    Show used mask in binary
 */


int main(void)
{
    DDRB    |= 0x30;    //0011 0000
    PORTB   |= 0x20;    //0010 0000
    PORTB   &= ~(0x10); //1110 1111
    while (1) {
       
    }

    return 0;   /* never reached */
}