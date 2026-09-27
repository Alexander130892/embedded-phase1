#include "gpio_pin.hpp"
#include "../common/stm32f446re.h"

using LedPin = GpioPin<GPIOA_BASE_ADDR, 5>;

extern "C" void pa5_init_output(void) {
    // enable clock is still raw register access — not part of GpioPin's scope
    LedPin::set_mode(Mode::Output);
}

extern "C" void pa5_high(void){ 
    LedPin::high(); 
}
extern "C" void pa5_low(void){ 
    LedPin::low(); 
}