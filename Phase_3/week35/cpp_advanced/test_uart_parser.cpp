#include <stdint.h>
#include <functional>

extern "C" void uart_send_string(const char* s);
extern "C" void led_on(void);
extern "C" void led_off(void);
extern uint32_t g_duty_cycle;
extern uint32_t g_CCR;

static bool str_eq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return false;
        a++; b++;
    }
    return *a == *b;
}

void handle_led(const char* arg) {
    if (str_eq(arg, "on")) { led_on(); }
    else if (str_eq(arg, "off")) { led_off(); }
    else { uart_send_string("bad arg\r\n"); }
}

void handle_pwm(const char* arg) {
    g_duty_cycle = (uint32_t)arg[0];
    uart_send_string("pwm set\r\n");
}

void handle_status(const char* arg) {
    (void)arg;
    uart_send_string("status\r\n");
}

struct command_t {
    const char* name;
    std::function<void(const char*)> handler;
};

static const command_t commands[] = {
    { "led",    handle_led    },
    { "pwm",    handle_pwm    },
    { "status", handle_status },
};

extern "C" void parse_command(const char* input) {
    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (str_eq(input, commands[i].name)) {
            commands[i].handler(input);
            return;
        }
    }
    uart_send_string("invalid\r\n");
}