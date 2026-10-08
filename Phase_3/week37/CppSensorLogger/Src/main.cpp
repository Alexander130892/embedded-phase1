#include "fixed_format.hpp"
#include "uart.hpp"

constinit Uart g_uart2{0x4000'4400u};   // USART2

volatile std::size_t g_sink;            // temporary size harness

int main()
{
    char buf[16];
    g_sink = format_fixed(buf, sizeof buf, -150, 2);
    for (;;) {}
}