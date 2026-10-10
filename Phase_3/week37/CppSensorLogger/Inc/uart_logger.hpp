// Inc/uart_logger.hpp — observer: LogRecord → CSV line → UART (firmware only)
#pragma once

#include <cstdint>

#include "csv_format.hpp"
#include "uart.hpp"

class UartLogger {
public:
    explicit UartLogger(Uart& uart) : uart_{uart} {}

    UartLogger(const UartLogger&)            = delete;
    UartLogger& operator=(const UartLogger&) = delete;

    void print_header() { 
        (void)uart_.send_string(kCsvHeader); 
    }

    // Subscribed to Subject<LogRecord, N> via etl::delegate.
    void on_record(const LogRecord& r)
    {
        char line[kCsvLineMax];
        if (format_csv(line, sizeof line, r) == 0 || !uart_.send_string(line)) {
            ++failed_lines_;
        }
    }

    std::uint32_t failed_lines() const { 
        return failed_lines_; 
    }

private:
    Uart&         uart_;
    std::uint32_t failed_lines_{0};
};