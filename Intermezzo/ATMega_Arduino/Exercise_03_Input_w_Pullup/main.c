#include <avr/io.h>
#include <util/delay.h>

/*
Exercise:
    Write 1010 to PD2..PD5 in one operation
 */


int main(void)
{
    PORTD   = (PORTD & (0xF << 2)) | (0xA << 2);      
    while (1) {
       
    }

    return 0;   /* never reached */
}