#include "Joint.h"
#include <algorithm>

Joint::Joint(const JointConfig &config)
    : _cfg(config), _servo(config.gpio) {
    // initialise servo to the physical position matching 90° with this joint's config
    _servo.setPulse(angleToPulse(90.0f));
}

void Joint::setAngle(float angle) {
    if (!_enabled) return;
    angle = std::clamp(angle, _cfg.minAngle, _cfg.maxAngle);
    _currentAngle = angle;
    _servo.setPulse(angleToPulse(angle));
}

void Joint::enable() {
    _enabled = true;
}

void Joint::disable() {
    _enabled = false;
    _servo.disable();
}

uint16_t Joint::angleToPulse(float angle) const {
    // clamp to limits, mirror, apply trim, clamp to [0°,180°].
    // trim is applied after mirroring so it always shifts in the same physical
    // direction regardless of which side of the robot the joint is on
    angle = std::clamp(angle, _cfg.minAngle, _cfg.maxAngle);
    if (_cfg.inverted) angle = 180.0f - angle;
    angle = std::clamp(angle + _cfg.offset, 0.0f, 180.0f);

    return static_cast<uint16_t>(
        _cfg.minPulse
        + (_cfg.maxPulse - _cfg.minPulse) * (angle / 180.0f)
        + 0.5f);
}
