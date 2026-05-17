
/*
    Notes:
        need timer to seed rand()
    Mapping:
        Button 0    -   D0  -   PD0     //using PD0/PD1 means UART TX/RX is not available
        Button 1    -   D1  -   PD1
        Button 2    -   D2  -   PD2
        Button 3    -   D3  -   PD3
        Led 0       -   D4  -   PD4
        Led 1       -   D5  -   PD5
        Led 2       -   D6  -   PD6
        Led 3       -   D7  -   PD7


    Wiring:
    PDx -- LEDs -- 220 OHm -- GND
    Vcc -- Potmeter(1)-- Potmeter(2) /PB5 -- GND

*/
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

/*
Exercise:
    Connect 4 buttons to 4 bits on same port
    Activate internal pull-ups (active low)
    Illuminate a random LED --> User needs to push the coressponding button
    Check if correct button was pressed
        if yes, continue
        if no, illuminate all leds and restart the game
 */

//   Register map (from ATmega328P datasheet §14):
//  *   DDRD    = Data Direction Register D/C  (1 = output, 0 = input)
//  *   PORTD   = Port D Data Register       (write output value)
//  *   PIND    = Port D Input Pins Register (read input value)

#define MAX_LEN     10          // Max Level
#define SHOW_MS     100000      // how long each LED in sequence is shown
#define GAP_MS      100000      // gap between LEDs in sequence
#define DEBOUNCE    30000       // Avoids multiple accidental reads in a row
#define FLASH_WIN   20000       // Flash_interval

void timer1_init(void);
void flash_all(void);
uint8_t read_button(void);
void play_sequence(uint8_t *seq, uint8_t len);

int main(void)
{
    DDRD &= ~(0x0F);    // bit [3:0] are input
    DDRD |= (0xF0);     // bit [7:4] are output
    PORTD |= (0x0F);    // internal pull-ups activated

    timer1_init();
    uint8_t seeded = 0;
    uint8_t state;
    uint8_t sequence[MAX_LEN];
    uint8_t level = 0;

    for(;;) { // infinite loop
        // add one step to sequence
        sequence[level] = rand() % 4;
        level++;
        // show sequence to player
        play_sequence(sequence, level);
        // read player input
        uint8_t correct = 1;
        for(uint8_t i = 0; i < level; i++) {
            uint8_t btn = read_button();
            if(btn != (1 << sequence[i])) {
                correct = 0;
                break;
            }
        }
        if(!correct) {
            flash_all();
            level = 0;      // reset game
        }
        if(level == MAX_LEN) {
            // winner — flash rapidly
            for(uint8_t i = 0; i < 20; i++) {
                PORTD = 0xF0 | 0x0F;
                for(volatile uint32_t d = 0; d < FLASH_WIN; d++);
                PORTD = 0x0F;
                for(volatile uint32_t d = 0; d < FLASH_WIN; d++);
            }
            level = 0;      // restart
        }
    }
    return 0;
}
void timer1_init(void) {
    TCCR1B |= (1 << CS10);   // no prescaler, timer runs freely
}
void flash_all(void) {
    for(uint8_t i = 0; i < 3; i++) {
        PORTD = 0xFF;
        for(volatile uint32_t d = 0; d < SHOW_MS; d++);
        PORTD = 0x0F;
        for(volatile uint32_t d = 0; d < SHOW_MS; d++);
    }
}
uint8_t read_button(void) {
    static uint8_t seeded = 0;
    if(!seeded){
        srand(TCNT1);
        seeded++;
    }
    while ((PIND & 0x0F) == 0x0F);                              // wait for press
    for(volatile uint32_t d = 0; d < DEBOUNCE; d++);            // debounce
    uint8_t state = (~PIND) & 0x0F;                             // read stable state
    while ((PIND & 0x0F) != 0x0F);                              // wait for release
    for(volatile uint32_t d = 0; d < DEBOUNCE; d++);            // debounce release
    // isolate lowest pressed bit in case of multiple presses
    return state & (-state);
}
void play_sequence(uint8_t *seq, uint8_t len) {
    PORTD = 0x0F;   // all LEDs off, pull-ups kept
    for(uint8_t i = 0; i < len; i++) {
        PORTD = (1 << (seq[i] + 4)) | 0x0F;                 // light LED
        for(volatile uint32_t d = 0; d < SHOW_MS; d++);     // show delay
        PORTD = 0x0F;                                       // LED off
        for(volatile uint32_t d = 0; d < GAP_MS; d++);      // gap between LEDs
    }
}

