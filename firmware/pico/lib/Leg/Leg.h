#pragma once
#include <cstdint>
#include "Joint.h"

struct LegConfig {
    JointConfig coxa;
    JointConfig femur;
    JointConfig tibia;
    float coxaLength;
    float femurLength;
    float tibiaLength;
};

class Leg {
public:
    explicit Leg(const LegConfig &config);

    void moveTo(float x, float y, float z);
    void moveToSmooth(float x, float y, float z,
                      float    maxDegPerSec = 120.0f,
                      uint16_t stepMs       = 10);

    void home();
    void stand();
    void crouch();
    void fold();

    void enable();
    void disable();
    bool isEnabled() const { return _enabled; }
void testAngles(float coxaDeg, float femurDeg, float tibiaDeg);

private:
    LegConfig _cfg;
    Joint     _coxa;
    Joint     _femur;
    Joint     _tibia;
    bool      _enabled = true;

    void solveIK(float x, float y, float z,
                 float &outCoxa, float &outFemur, float &outTibia) const;
};
