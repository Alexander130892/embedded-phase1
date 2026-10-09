#include "bsp.hpp"

#include <cstdint>

namespace {

constexpr std::uintptr_t kRccAhb1enr = 0x4002'3800u + 0x30u;   // RCC_AHB1ENR
constexpr std::uintptr_t kGpioaModer = 0x4002'0000u + 0x00u;   // GPIOA_MODER
constexpr std::uintptr_t kGpioaAfrl  = 0x4002'0000u + 0x20u;   // GPIOA_AFRL (pins 0..7)

constexpr std::uint32_t kRccAhb1enrGpioaEn = 1u << 0;
constexpr std::uint32_t kModerAltFunc      = 0b10;   // 2 bits per pin
constexpr std::uint32_t kAfUsart2          = 7;      // AF7, 4 bits per pin

inline volatile std::uint32_t& reg(std::uintptr_t addr)
{
    return *reinterpret_cast<volatile std::uint32_t*>(addr);
}

// Helper that puts one pin into alternate-function mode with a given AF
void set_alt_func(unsigned pin, std::uint32_t af) { 
    if (pin > 7) { return; }   // AFRL covers pins 0..7 only; pins 8..15 are in AFRH
    reg(kGpioaModer) &= ~(0x3u << pin*2);
    reg(kGpioaModer) |= kModerAltFunc << pin*2;

    reg(kGpioaAfrl) &= ~(0xFu << pin*4);
    reg(kGpioaAfrl) |= (af) << pin*4;
}

} // namespace

void bsp_init()
{
    // Enable the GPIOA clock (RCC_AHB1ENR)
    reg(kRccAhb1enr) |= kRccAhb1enrGpioaEn ;
    [[maybe_unused]] const std::uint32_t rb = reg(kRccAhb1enr);   // read-back: completes the write
    // PA2 → AF7 (USART2_TX)
    set_alt_func(2, kAfUsart2);
    // PA3 → AF7 (USART2_RX)
    set_alt_func(3, kAfUsart2);
}