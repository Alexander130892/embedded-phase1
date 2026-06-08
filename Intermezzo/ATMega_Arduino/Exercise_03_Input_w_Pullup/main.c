/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    7-6-2026
 *
 * Description:
 *   This AVR microcontroller program writes the binary pattern 1010 to
 *   pins PD2 through PD5 on PORTD using bitwise operations, then
 *   enters an infinite loop. The operation preserves the state of all
 *   other pins on PORTD while only modifying the target 4-bit range.
 */
#include <avr/io.h>
#include <util/delay.h>

/*
Exercise:
    Write 1010 to PD2..PD5 in one operation
 */


int main(void)
{
    PORTD   = (PORTD & ~(0xF << 2)) | (0xA << 2);      
    while (1) {
       
    }

    return 0;   /* never reached */
}