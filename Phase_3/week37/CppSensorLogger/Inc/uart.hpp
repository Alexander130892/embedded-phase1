#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "ring_buffer.hpp"

// Vector-table entry. Declared here with C linkage so the friend
// declaration below refers to this unmangle) function.
extern "C" void USART2_IRQHandler(void);

// USART register block, STM32F446 RM0390  (offsets 0x00..0x18).
struct UsartRegs {
    volatile std::uint32_t SR;    // 0x00 status
    volatile std::uint32_t DR;    // 0x04 data
    volatile std::uint32_t BRR;   // 0x08 baud rate
    volatile std::uint32_t CR1;   // 0x0C control 1
    volatile std::uint32_t CR2;   // 0x10 control 2
    volatile std::uint32_t CR3;   // 0x14 control 3
    volatile std::uint32_t GTPR;  // 0x18 guard time / prescaler
};
static_assert(sizeof(UsartRegs) == 0x1C, "UsartRegs must match the hardware layout");

// Interrupt-driven UART receiver + polled transmitter.
// Lifecycle:
//   constinit Uart g_uart2{0x4000'4400u};  // constexpr ctor: stores address only, no startup code
//   bsp_init();                            // board: GPIO pin mux (PA2/PA3, AF7) — not this class
//   g_uart2.init();                        // hardware: RCC clock, BRR, CR1, NVIC
// RX path:  USART2_IRQHandler -> on_irq() -> rx_.push()        (producer, ISR)
//           receive_line()    -> rx_.pop() -> line_ -> out     (consumer, main)
class Uart {
public:
    static constexpr std::size_t kRxBufferSize = 64;  // ring buffer slots (capacity 63)
    static constexpr std::size_t kMaxLineLen   = 63;  // longest accepted line, excl. '\0'

    constexpr explicit Uart(std::uintptr_t base) : base_{base} {}

    // Not copyable: one object per hardware peripheral.
    Uart(const Uart&)            = delete;
    Uart& operator=(const Uart&) = delete;

    void init();                                  // enable clock, 115200 8N1, RXNE interrupt, NVIC

    bool send_char(char c);                       // false = TXE timeout
    bool send_string(const char* s);              // false = TXE timeout (rest not sent)

    // Non-blocking. Drains all bytes the ISR has received so far.
    // Returns true when a complete line ('\r' or '\n' terminated) was copied to
    // `out` as a C string. A line longer than kMaxLineLen, or one that does not
    // fit in `cap`, is discarded entirely and never returned. Empty lines are skipped.
    bool receive_line(char* out, std::size_t cap);

    std::uint32_t dropped_bytes() const { 
        return dropped_; 
    }  // RX ring buffer overflows

private:
    friend void ::USART2_IRQHandler(void);        // only the ISR may call on_irq()
    void on_irq();                                // RXNE: read DR, push into rx_

    UsartRegs& regs() const { 
        return *reinterpret_cast<UsartRegs*>(base_); 
    }
    std::uintptr_t                       base_;              // peripheral base address
    ring_buffer<char, kRxBufferSize>     rx_{};              // ISR -> main
    std::array<char, kMaxLineLen + 1>    line_{};            // line being assembled (main only)
    std::size_t                          line_len_{0};       // replaces static index
    bool                                 line_overflow_{false};  // current line too long -> discard
    volatile std::uint32_t               dropped_{0};        // written by ISR, read by main
};