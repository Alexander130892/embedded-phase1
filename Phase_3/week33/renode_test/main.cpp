#include <cstdint>
#include "ringbuffer_constexpr.hpp"   // or wherever your class template lives

// RCC and GPIOA base addresses + the two registers you need — you know these
// from Phase 1. Fill in:

//for Peripheral clocks
#define RCC_BASE_ADDR      	0x40023800

#define RCC_AHB1ENR_OFFSET 	0x30		// GPIOA
//for GPIOA
#define GPIOA_BASE_ADDR   	0x40020000
#define GPIOA_MODER_OFFSET	0x00 		// write 01 for correct PA pin TBC
#define GPIOA_ODR_OFFSET	0x14 		// to correct pin TBC
#define GPIOA_BSRR_OFFSET	0x18 		
// Bits 31:16 Bits 15:0 BRy: Port x reset bit y (y = 0..15)
// These bits are write-only and can be accessed in word, half-word or byte mode. A read to
// these bits returns the value 0x0000.
// 0: No action on the corresponding ODRx bit
// 1: Resets the corresponding ODRx bit
// Note: If both BSx and BRx are set, BSx has priority.
// BSy: Port x set bit y (y= 0..15)
// These bits are write-only and can be accessed in word, half-word or byte mode. A read to
// these bits returns the value 0x0000.
// 0: No action on the corresponding ODRx 


// ring_buffer<uint32_t, 16> rb;   // proves the C++ object actually initializes

static void delay(volatile uint32_t count) {
    while (count--) { __asm__("nop"); }
}

int main() {
    rb.push(0xDEAD);   // touch the object so it's not optimized to nothing
    uint32_t out;
    rb.pop(&out);

    //Clock init GPIOA/GPIO
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)) 	|= 0x1; //01 for GPIOA
    //Init LED on pin PA5
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOA_MODER_OFFSET))	|= (0x1 << 2*5);
    
    constexpr uint32_t PA5_SET   = (1u << 5);         // BSy: set pin 5
    constexpr uint32_t PA5_RESET = (1u << (5 + 16));  // BRy: reset pin 5

    uint8_t toggle = 0;
    while (true) {
        if (toggle) {
            *((volatile uint32_t*)(GPIOA_BASE_ADDR + GPIOA_BSRR_OFFSET)) = PA5_SET;
        } else {
            *((volatile uint32_t*)(GPIOA_BASE_ADDR + GPIOA_BSRR_OFFSET)) = PA5_RESET;
        }
        toggle = !toggle;
        delay(500000);
    }
}