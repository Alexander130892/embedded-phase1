#include <stdint.h>

#define RCC_BASE        0x40023800UL
#define RCC_CR          (*(volatile uint32_t*)(RCC_BASE + 0x00))
#define RCC_PLLCFGR     (*(volatile uint32_t*)(RCC_BASE + 0x04))
#define RCC_CFGR        (*(volatile uint32_t*)(RCC_BASE + 0x08))

#define FLASH_BASE      0x40023C00UL
#define FLASH_ACR       (*(volatile uint32_t*)(FLASH_BASE + 0x00))

#define SCB_CPACR       (*(volatile uint32_t*)(0xE000ED88UL))

void SystemInit(void)
{
    /* 1. Enable HSE and wait for it to stabilize */
    RCC_CR |= (1u << 16);              // HSEON
    while (!(RCC_CR & (1u << 17)));    // wait HSERDY

    /* 2. Flash latency: 5 wait states, required BEFORE raising SYSCLK to 180MHz
     *    (must be set before the clock switch, or fetched instructions can be corrupted) */
    FLASH_ACR = (FLASH_ACR & ~0x7u) | 0x5u;   // LATENCY = 5WS

    /* 3. Configure PLL: HSE(8MHz) / PLLM(4) = 2MHz -> x PLLN(180) = 360MHz VCO
     *    -> / PLLP(2) = 180MHz SYSCLK. PLLQ=7 for USB/SDIO (unused here, set anyway). */
    RCC_PLLCFGR = (4u  << 0)   // PLLM = 4
                | (180u << 6)  // PLLN = 180
                | (0u   << 16) // PLLP = 2  (encoded as 0b00)
                | (1u   << 22) // PLLSRC = HSE
                | (7u   << 24);// PLLQ = 7

    /* 4. Enable PLL, wait for lock */
    RCC_CR |= (1u << 24);              // PLLON
    while (!(RCC_CR & (1u << 25)));    // wait PLLRDY

    /* 5. Bus prescalers: AHB=/1 (180MHz), APB1=/4 (45MHz, max 45), APB2=/2 (90MHz, max 90) */
    RCC_CFGR = (RCC_CFGR & ~0xF0u)   | (0x5u << 10);  // APB1 = /4  (0b101)
    RCC_CFGR = (RCC_CFGR & ~0x1C00u) | (0x4u << 13);  // APB2 = /2  (0b100)
    // AHB prescaler left at /1 (reset default), no change needed

    /* 6. Switch SYSCLK source to PLL, wait for it to take effect */
    RCC_CFGR = (RCC_CFGR & ~0x3u) | 0x2u;             // SW = PLL
    while (((RCC_CFGR >> 2) & 0x3u) != 0x2u);          // wait SWS == PLL

    /* 7. Enable FPU (CP10/CP11 full access) — required before any float/double code runs */
    SCB_CPACR |= (0xFu << 20);
}