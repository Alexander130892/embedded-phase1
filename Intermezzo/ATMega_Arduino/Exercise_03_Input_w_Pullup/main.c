/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This AVR microcontroller program sets pins PD2 through PD5 to the
 *   binary pattern 1010 (hexadecimal A) in a single operation, then
 *   enters an infinite loop.
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