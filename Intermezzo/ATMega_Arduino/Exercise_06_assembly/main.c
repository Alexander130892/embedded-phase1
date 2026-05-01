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