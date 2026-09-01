#include <cstdio>
#include <array>
#include "status.hpp"

struct RingBuffer{
    std::array<uint8_t, 50> data;
    std::atomic<uint16_t> write_idx=0;
    std::atomic<uint16_t> read_idx=0;
};

UartLogger g_uart_logger;  // global object, lives for the whole program

extern "C" void USART2_IRQHandler(void) {
    auto byte = g_uart_logger.drainByte();
    if (byte) {
        // write *byte to the UART data register
        *((volatile uint32_t *)(USART2_BASE_ADDR + USART_DR_OFFSET)) = *byte;
    } else {
        // buffer empty — disable TX-empty interrupt
        *((volatile uint32_t *)(USART2_BASE_ADDR + USART_CR1_OFFSET)) &= ~(0x1 << 7);
    }
}

class UartLogger{
    public:
        UartLogger()
        {}
    std::expected<void, ErrorCode> log(const char* txt){
        while(*txt != '\0'){
            if( ((r_buffer_.write_idx + 1) % 50)== r_buffer_.read_idx){
                return std::unexpected(ErrorCode::UART_RB_FULL);
            } // Buffer full
            r_buffer_.data[r_buffer_.write_idx] = *txt;
            r_buffer_.write_idx = (r_buffer_.write_idx + 1) % 50;
            txt++;
        }
        return {};
    }

    std::expected<uint8_t, ErrorCode> drainByte(){
        if(r_buffer_.write_idx == r_buffer_.read_idx){
                return std::unexpected(ErrorCode::UART_RB_EMPTY);
        } // Buffer Empty
        uint8_t ret_val = r_buffer_.data[r_buffer_.read_idx];
        r_buffer_.read_idx = (r_buffer_.read_idx + 1) % 50;
        return ret_val;
    }

    private:
        RingBuffer r_buffer_;
};