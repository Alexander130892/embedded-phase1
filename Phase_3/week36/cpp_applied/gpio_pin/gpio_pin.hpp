#pragma once
#include <cstdint>
#include <cstddef>

// --- Register field tags ---
// Each tag describes ONE property: how many bits wide is this field per pin?
struct ModerTag  { static constexpr uint32_t width = 2; };
struct OtyperTag { static constexpr uint32_t width = 1; };
struct OspeedrTag{ static constexpr uint32_t width = 2; };
struct PupdrTag  { static constexpr uint32_t width = 2; };

// --- Generic bitfield accessor ---
template<typename RegTag, uint8_t Pin>
class RegField {
    // Static_assert on Pin range (0-15)
    static_assert(Pin < 16);
    static constexpr uint32_t shift         = RegTag::width * Pin;
    static constexpr uint32_t field_mask    = (1u << RegTag::width) - 1u;   // width-bit mask at position 0
    static constexpr uint32_t mask          = field_mask << shift;          // positioned mask
public:
    static void set(volatile uint32_t& reg, uint32_t value){
        reg &= ~(mask);
        reg |= ((value & field_mask) << shift);
    }
    static uint32_t read(volatile uint32_t& reg){
        return ((reg & mask) >> shift);
    }
};

// Mode enum (replaces raw ints for set_mode, per earlier discussion) ---
enum class Mode : uint32_t {
    Input = 0b00,
    Output = 0b01,
    AlternateFunction = 0b10,
    Analog = 0b11
};

// --- GPIO register block struct ---
struct GPIO_TypeDef {
    volatile uint32_t MODER;    // offset 0x00
    volatile uint32_t OTYPER;   // offset 0x04
    volatile uint32_t OSPEEDR;  // offset 0x08
    volatile uint32_t PUPDR;    // offset 0x0C
    volatile uint32_t IDR;      // offset 0x10
    volatile uint32_t ODR;      // offset 0x14
    volatile uint32_t BSRR;     // offset 0x18
    volatile uint32_t LCKR;     // offset 0x1C    
    volatile uint32_t AFRL;     // offset 0x20
    volatile uint32_t AFRH;     // offset 0x24
};
// Extra check at compile time
static_assert(offsetof(GPIO_TypeDef, MODER)   == 0x00, "MODER offset mismatch");
static_assert(offsetof(GPIO_TypeDef, OTYPER)  == 0x04, "OTYPER offset mismatch");
static_assert(offsetof(GPIO_TypeDef, OSPEEDR) == 0x08, "OSPEEDR offset mismatch");
static_assert(offsetof(GPIO_TypeDef, PUPDR)   == 0x0C, "PUPDR offset mismatch");
static_assert(offsetof(GPIO_TypeDef, IDR)     == 0x10, "IDR offset mismatch");
static_assert(offsetof(GPIO_TypeDef, ODR)     == 0x14, "ODR offset mismatch");
static_assert(offsetof(GPIO_TypeDef, BSRR)    == 0x18, "BSRR offset mismatch");
static_assert(offsetof(GPIO_TypeDef, LCKR)    == 0x1C, "LCKR offset mismatch");
static_assert(offsetof(GPIO_TypeDef, AFRL)    == 0x20, "AFRL offset mismatch");
static_assert(offsetof(GPIO_TypeDef, AFRH)    == 0x24, "AFRH offset mismatch");

// --- GpioPin  ---
template<uint32_t PortBase, uint8_t Pin>
class GpioPin {
    static_assert(Pin < 16);
    // private accessor returning GPIO_TypeDef& at PortBase
    static GPIO_TypeDef& port() { 
        return *reinterpret_cast<GPIO_TypeDef*>(PortBase); 
    }
public:
    static void set_mode(Mode m){
        RegField<ModerTag, Pin>::set(port().MODER, static_cast<uint32_t>(m));
    }
    // BSRR directly (bit Pin for set, bit Pin+16 for clear
    static void high(){
        port().BSRR = (1u << Pin);
    }
    static void low(){
        port().BSRR = (1u << (Pin+16));
    }
    static bool read(){
        return (port().IDR & (1u << Pin)) >> Pin;
    }

};