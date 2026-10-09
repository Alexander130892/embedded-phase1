#include "fixed_format.hpp"
#include "uart.hpp"
#include "bsp.hpp"

constinit Uart g_uart2{0x4000'4400u};   // USART2

int main(){
    bsp_init();          // PA2/PA3 → AF7
    g_uart2.init();      // clock, 115200 8N1, RXNE IRQ
    
    // Startup: proves TX + format_fixed on the target
    char num[16];
    format_fixed(num, sizeof num, -150, 2);
    g_uart2.send_string("CppSensorLogger up, format_fixed(-150, 2) = ");
    g_uart2.send_string(num);                 // expect "-1.50"
    g_uart2.send_string("\r\n");

    // Echo: proves RX ISR → ring_buffer → receive_line
    char line[Uart::kMaxLineLen + 1];
    for (;;) {
        if (g_uart2.receive_line(line, sizeof line)) {
            g_uart2.send_string("> ");
            g_uart2.send_string(line);
            g_uart2.send_string("\r\n");
        }
    }
}