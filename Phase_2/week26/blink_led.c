#include <gpiod.h>
#include <unistd.h>

int main(void){
    struct gpiod_chip * chip = gpiod_chip_open("/dev/gpiochip0");
    struct gpiod_line_settings * settings = gpiod_line_settings_new();
    // configure settings...
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    struct gpiod_line_config * line_cfg = gpiod_line_config_new();
    // attach settings to offset 18...
    const unsigned int offsets[1] = {18};
    size_t num_offsets = 1;
    gpiod_line_config_add_line_settings(line_cfg, offsets, num_offsets, settings);

    struct gpiod_request_config * req_cfg = gpiod_request_config_new();
    // set consumer name...
    gpiod_request_config_set_consumer(req_cfg, "LED Blink");
    struct gpiod_line_request * request = gpiod_chip_request_lines(chip, req_cfg, line_cfg); 

    // free chip, settings, line_cfg, req_cfg here — done with them
    gpiod_chip_close(chip);
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);

    // blink loop — only uses `request`
    while (1) {
        gpiod_line_request_set_value(request, 18, GPIOD_LINE_VALUE_ACTIVE);
        sleep(1);
        gpiod_line_request_set_value(request, 18, GPIOD_LINE_VALUE_INACTIVE);
        sleep(1);
    }

    gpiod_line_request_release(request);  // cleanup at the end 
}