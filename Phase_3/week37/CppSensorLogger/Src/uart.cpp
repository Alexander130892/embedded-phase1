#include "uart.hpp"
#include <cstring> 

namespace {

// --- Register addresses outside the USART block (RM0390) ---------------------
constexpr std::uintptr_t kRccApb1enr = 0x4002'3800u + 0x40u;   // RCC_APB1ENR
constexpr std::uintptr_t kNvicIser1  = 0xE000'E100u + 0x04u;   // NVIC_ISER1 (IRQs 32..63)

// --- Bit positions -----------------------------------------------------------
constexpr std::uint32_t kRccApb1enrUsart2En = 1u << 17;   // USART2 clock enable
constexpr std::uint32_t kNvicUsart2Bit      = 1u << 6;    // USART2 = IRQ 38 → ISER1 bit 6

constexpr std::uint32_t kSrRxne  = 1u << 5;    // receive data register not empty
constexpr std::uint32_t kSrTxe   = 1u << 7;    // transmit data register empty

constexpr std::uint32_t kCr1Re     = 1u << 2;  // receiver enable
constexpr std::uint32_t kCr1Te     = 1u << 3;  // transmitter enable
constexpr std::uint32_t kCr1Rxneie = 1u << 5;  // RXNE interrupt enable
constexpr std::uint32_t kCr1Ue     = 1u << 13; // USART enable

constexpr std::uint32_t kBrr115200 = 0x8B;     // 16 MHz HSI → 115200 baud (from Week 17)
constexpr std::uint32_t kTxTimeout = 100'000;  // TXE poll limit (Week 17 MAX_TIMEOUT)

inline volatile std::uint32_t& reg(std::uintptr_t addr)
{
    return *reinterpret_cast<volatile std::uint32_t*>(addr);
}

} 

// Hardware setup — called once from main, after clocks and GPIO (bsp_init)
void Uart::init()
{
    // Enable the USART2 peripheral clock
    reg(kRccApb1enr) |= kRccApb1enrUsart2En;
    // Set the baud rate (BRR)
    regs().BRR     = kBrr115200;
    [[maybe_unused]] const std::uint32_t rb = regs().BRR;   // read-back: completes the write
    // Eenable transmitter, receiver and RXNE interrupt (CR1)
    regs().CR1     |= (kCr1Re | kCr1Te | kCr1Rxneie);
    // TEnable USART2 in the NVIC (ISER1)
    reg(kNvicIser1) |= kNvicUsart2Bit;
    // Enable the USART last (CR1 UE) — last to make sure uarts is properly configured
    regs().CR1     |= kCr1Ue;
}
// TX — polled
bool Uart::send_char(char c)
{
    // wait until SR.TXE is set, giving up after kTxTimeout polls → return false
    // write c to DR → return true
    std::uint32_t timer=0;
    while(!(regs().SR & kSrTxe)){
        timer++;
        if(timer > kTxTimeout){
            return false;
        }
    }
    regs().DR = static_cast<std::uint8_t>(c); //avoids signed/unsigned problems
    return true;
}

bool Uart::send_string(const char* s)
{
    // Send_char() each character until '\0'; stop and return false on the first failure
    while(*s != '\0'){
        if(!send_char(*s++)) {
            return false;
        }
    }
    return true;
}
// RX — ISR side (producer). Runs in interrupt context: keep it short.
void Uart::on_irq()
{
    // if SR.RXNE is set:
    //     read DR (this also clears RXNE)
    //     push the byte into rx_
    //     if the push failed: count it in dropped_
    // Note: `dropped_++` on a volatile is deprecated in C++20 (-Wvolatile).
    //       Write it as  dropped_ = dropped_ + 1;
    if(regs().SR & kSrRxne){
        const auto byte = static_cast<std::uint8_t>(regs().DR);   // read DR once: low 8 bits, clears RXNE
        if (!rx_.push(static_cast<char>(byte))) {
            dropped_ = dropped_ + 1;
        }
    }
}

extern "C" void USART2_IRQHandler(void)
{
    extern Uart g_uart2;   // the one instance, defined in main.cpp
    g_uart2.on_irq();
}

// RX — main side (consumer). Non-blocking; call every super-loop iteration.
bool Uart::receive_line(char* out, std::size_t cap)
{
    char c{};
    while (rx_.pop(&c)) {

        // --- Ordinary character: append, or mark the line as too long --------
        if (c != '\r' && c != '\n') {
            if (line_len_ < kMaxLineLen) {
                line_[line_len_++] = c;
            } else {
                line_overflow_ = true;          // keep dropping until the line ends
            }
            continue;
        }

        // --- End of line ----------------------------------------------------
        const bool        discard = line_overflow_ || (line_len_ + 1 > cap);
        const std::size_t len     = line_len_;

        line_len_      = 0;                     // every line end starts a fresh line
        line_overflow_ = false;

        if (len == 0 || discard) {
            continue;                           // empty line ("\r\n") or rejected line
        }

        std::memcpy(out, line_.data(), len);    // copy into the caller's buffer
        out[len] = '\0';
        return true;                            // one line per call; rest stays in rx_
    }
    return false;                               // rx_ drained, no complete line yet
}