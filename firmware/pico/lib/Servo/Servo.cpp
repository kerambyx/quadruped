#include "Servo.h"
#include <algorithm>
#include "hardware/gpio.h"

Servo::Servo(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    _slice = pwm_gpio_to_slice_num(gpio);
    _chan  = pwm_gpio_to_channel(gpio);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, PWM_CLKDIV);
    pwm_config_set_wrap(&cfg, PWM_WRAP);
    pwm_init(_slice, &cfg, true);

    setPulse(1500);  // initialise to mid-point
}

void Servo::setPulse(uint16_t pulse_us) {
    pulse_us = std::clamp(pulse_us, PULSE_MIN_US, PULSE_MAX_US);
    pwm_set_chan_level(_slice, _chan, pulse_us);
}

void Servo::disable() {
    pwm_set_chan_level(_slice, _chan, 0);
}
