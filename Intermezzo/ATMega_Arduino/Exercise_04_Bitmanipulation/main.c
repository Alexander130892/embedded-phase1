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