#pragma once
#include <cstdint>
#include "hardware/pwm.h"

// Servo.h handles raw PWM output only
// it knows nothing about angles, limits, or joint configuration
// 125 MHz / 125 (clkdiv) / 20000 (wrap+1) = 50 Hz; at this rate 1 tick = 1 µs.

static constexpr float    PWM_CLKDIV  = 125.0f;
static constexpr uint16_t PWM_WRAP    = 19999;
static constexpr uint16_t PULSE_MIN_US = 560;
static constexpr uint16_t PULSE_MAX_US = 2600;

class Servo {
public:
    explicit Servo(uint gpio);

    void setPulse(uint16_t pulse_us);
    void disable();  // zero pulse de-energises the servo

private:
    uint _slice;
    uint _chan;
};
