
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
//  *   DDRD  = Data Direction Register D/C  (1 = output, 0 = input)
//  *   PORTD = Port D Data Register       (write output value)
//  *   PINC = Port C Input Pins Register (read input value)

#define MAX_LEN     5       // Max Level
#define SHOW_MS     100000    // how long each LED in sequence is shown
#define GAP_MS      100000    // gap between LEDs in sequence

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
                for(volatile uint32_t d = 0; d < 20000; d++);
                PORTD = 0x0F;
                for(volatile uint32_t d = 0; d < 20000; d++);
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
    while ((PIND & 0x0F) == 0x0F);                         // wait for press
    for(volatile uint32_t d = 0; d < 5000; d++);           // debounce
    uint8_t state = (~PIND) & 0x0F;                        // read stable state
    while ((PIND & 0x0F) != 0x0F);                         // wait for release
    for(volatile uint32_t d = 0; d < 5000; d++);           // debounce release

    // isolate lowest pressed bit in case of multiple presses
    return state & (-state);
}
void play_sequence(uint8_t *seq, uint8_t len) {
    PORTD = 0x0F;   // all LEDs off, pull-ups kept
    for(uint8_t i = 0; i < len; i++) {
        PORTD = (1 << (seq[i] + 4)) | 0x0F;                // light LED
        for(volatile uint32_t d = 0; d < SHOW_MS; d++);    // show delay
        PORTD = 0x0F;                                       // LED off
        for(volatile uint32_t d = 0; d < GAP_MS; d++);     // gap between LEDs
    }
}

/* 
asm dissection

fast write x: only 4 CPU cycles needed, but, very dedicated (not flexible)
compiler used sbi directly, single instruction for the set. The 4 cycles include the and (check bool), breq (branch), sbi (set bit), ret
00000080 <fastWrite7>:
  80:	88 23       	and	r24, r24
  82:	11 f0       	breq	.+4      	; 0x88 <.L2>
  84:	5f 9a       	sbi	0x0b, 7	; 11
  86:	08 95       	ret

6 CPU cycles needed (including error checking)
no sbi/cbi here because the pin number isn't known at compile time — it's a variable. 
sbi/cbi require a compile-time constant bit number, so the compiler falls back to a read-modify-write sequence: in (read PORTD), compute mask, XOR, out (write back).
--> flexibility costs instructions.

0000008c <fastToggle>:
  8c:	83 70       	andi	r24, 0x03	; 3
  8e:	9b b1       	in	r25, 0x0b	; 11
  90:	8c 5f       	subi	r24, 0xFC	; 252
  92:	21 e0       	ldi	r18, 0x01	; 1
  94:	30 e0       	ldi	r19, 0x00	; 0
  96:	01 c0       	rjmp	.+2      	; 0x9a <.L2^B2>


from Arduino IDE:
Lots more instructions
--> maximum flexibility, at cost of raw speed

void pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t bit = digitalPinToBitMask(pin);
1e6:  e2 e9    ldi   r30, 0x92     ; load lookup table address
1e8:  f0 e0    ldi   r31, 0x00
1ea:  24 91    lpm   r18, Z        ; load bit mask from Flash

    uint8_t port = digitalPinToPort(pin);
1ec:  ee e7    ldi   r30, 0x7E
1ee:  f0 e0    ldi   r31, 0x00
1f0:  84 91    lpm   r24, Z        ; load port from Flash

    if (port == NOT_A_PIN) return;
1f2:  88 23    and   r24, r24
1f4:  99 f0    breq  .+38          ; bail if invalid pin

    reg = portModeRegister(port);  ; calculate DDR address
1f6:  90 e0    ldi   r25, 0x00
1f8:  88 0f    add   r24, r24
1fa:  99 1f    adc   r25, r25
...
        uint8_t oldSREG = SREG;
210:  8f b7    in    r24, 0x3f     ; save interrupt state
        cli();
212:  f8 94    cli                 ; disable interrupts
        *reg |= bit;
214:  ec 91    ld    r30, X        ; read DDR
216:  e2 2b    or    r30, r18      ; set bit
218:  ec 93    st    X, r30        ; write DDR
        SREG = oldSREG;
21a:  8f bf    out   0x3f, r24     ; restore interrupts

*/