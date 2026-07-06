#include "Quadruped.h"
#include <cmath>
#include "pico/stdlib.h"

Quadruped::Quadruped(const QuadrupedConfig &config)
    : _fl(config.frontLeft),
      _rl(config.rearLeft),
      _fr(config.frontRight),
      _rr(config.rearRight),
      _bodyLength(config.bodyLength),
      _bodyWidth(config.bodyWidth) {}

void Quadruped::init() {
    crouch();
    sleep_ms(2000);
    stand();
    sleep_ms(2000);
}

void Quadruped::home() {
    _fl.home(); _rl.home();
    _fr.home(); _rr.home();
}

void Quadruped::stand() {
    _fl.stand(); _rl.stand();
    _fr.stand(); _rr.stand();
}

void Quadruped::crouch() {
    _fl.crouch(); _rl.crouch();
    _fr.crouch(); _rr.crouch();
}

// diagonal trot gait.
// FL+RR and FR+RL move 180° out of phase and each tracie a semi-circular arc
void Quadruped::trot(int steps) {
    static constexpr float Y_STAND  = 14.0f;
    static constexpr float Y_LIFT   = 11.0f;
    static constexpr float X_STRIDE =  6.0f;
    static constexpr float X_OFFSET =  0.5f;
    static constexpr float Z_POS    =  0.0f;
    static constexpr float T_STEP   =  0.15f;
    static constexpr float PI_F     = static_cast<float>(M_PI);

    for (int i = 0; i < steps; i++) {
        // start at π/2 so the cycle opens exactly at X_OFFSET
        // so transitions smoothly from standing
        for (float t = PI_F / 2.0f; t <= 2.5f * PI_F; t += T_STEP) {
            const float s1 = sinf(t);
            const float s2 = sinf(t + PI_F);
            const float x1 = X_OFFSET + X_STRIDE * cosf(t);
            const float x2 = X_OFFSET + X_STRIDE * cosf(t + PI_F);
            const float y1 = Y_STAND - (s1 > 0.0f ? (Y_STAND - Y_LIFT) * s1 : 0.0f);
            const float y2 = Y_STAND - (s2 > 0.0f ? (Y_STAND - Y_LIFT) * s2 : 0.0f);

            _fl.moveTo(x1, y1, Z_POS);
            _rr.moveTo(x1, y1, Z_POS);
            _fr.moveTo(x2, y2, Z_POS);
            _rl.moveTo(x2, y2, Z_POS);

            sleep_ms(15);
        }
    }

    stand();
}

void Quadruped::setStance(float pitchDeg, float rollDeg, float yOffset) {
    static constexpr float DEG2RAD = static_cast<float>(M_PI) / 180.0f;

    const float pitchRad = pitchDeg * DEG2RAD;
    const float rollRad  = rollDeg  * DEG2RAD;

    const float frontDY =  (_bodyLength / 2.0f) * sinf(pitchRad);
    const float backDY  = -(_bodyLength / 2.0f) * sinf(pitchRad);
    const float leftDY  = -(_bodyWidth  / 2.0f) * sinf(rollRad);
    const float rightDY =  (_bodyWidth  / 2.0f) * sinf(rollRad);

    const float base = REST_Y + yOffset;
    _fl.moveTo(REST_X, base + frontDY + leftDY,  REST_Z);
    _fr.moveTo(REST_X, base + frontDY + rightDY, REST_Z);
    _rl.moveTo(REST_X, base + backDY  + leftDY,  REST_Z);
    _rr.moveTo(REST_X, base + backDY  + rightDY, REST_Z);
}
