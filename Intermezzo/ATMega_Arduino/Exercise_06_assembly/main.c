/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file configures pin PC2 as an input on an AVR microcontroller
 *   using direct register manipulation, then enters an infinite loop,
 *   serving as a minimal setup for comparing assembly overhead between
 *   direct register writes and Arduino's pinMode() function.
 */

#include <avr/io.h>
#include <util/delay.h>

/*
Exercise:
    Compare direct register write overhead in asm to pinMode() overhead asm (in ArduionoIDE)
 */


int main(void)
{
    DDRC    &= ~(1 << 2);  //PC2 as input
    while (1) {
       
    }

    return 0;   /* never reached */
}