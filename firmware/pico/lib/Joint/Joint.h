#pragma once
#include <cstdint>
#include "Servo.h"

struct JointConfig {
    uint8_t  gpio;
    uint16_t minPulse;   // pulse width in µs at 0°
    uint16_t maxPulse;   // pulse width in µs at 180°
    float    minAngle;   // physical lower limit (degrees)
    float    maxAngle;   // physical upper limit (degrees)
    float    offset;     // trim added after inversion (degrees)
    bool     inverted;   // if true, angle is mirrored about 90°
};

class Joint {
public:
    explicit Joint(const JointConfig &config);

    void  setAngle(float angle);
    float getAngle() const { return _currentAngle; }

    void enable();
    void disable();
    bool isEnabled() const { return _enabled; }

private:
    JointConfig _cfg;
    Servo       _servo;
    float       _currentAngle = 90.0f;
    bool        _enabled      = true;

    uint16_t angleToPulse(float angle) const;
};
