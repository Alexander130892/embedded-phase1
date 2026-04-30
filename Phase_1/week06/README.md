## gpio_blink
Bare-metal LED blink and button read on NUCLEO-F446RE.
No HAL, no CMSIS. Direct register writes only.

PA5 → LD2 (LED)
PC13 → B1 (user button, active low)

Build: make
Flash: OpenOCD or ST-Link