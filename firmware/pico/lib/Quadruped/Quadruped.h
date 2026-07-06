#pragma once
#include "Leg.h"

struct QuadrupedConfig {
    LegConfig frontLeft;
    LegConfig rearLeft;
    LegConfig frontRight;
    LegConfig rearRight;
    float bodyLength;
    float bodyWidth;
};

class Quadruped {
public:
    explicit Quadruped(const QuadrupedConfig &config);

    void init();
    void home();
    void stand();
    void crouch();
    void trot(int steps);

    // pitchDeg: + looks up,    − looks down
    // rollDeg:  + tilts right, − tilts left
    // yOffset:  + raises body, − lowers body
    void setStance(float pitchDeg, float rollDeg, float yOffset = 0.0f);

    Leg& frontLeftLeg()  { return _fl; }
    Leg& rearLeftLeg()   { return _rl; }
    Leg& frontRightLeg() { return _fr; }
    Leg& rearRightLeg()  { return _rr; }

private:
    Leg   _fl;
    Leg   _rl;
    Leg   _fr;
    Leg   _rr;
    float _bodyLength;
    float _bodyWidth;

    static constexpr float REST_X = 0.5f;
    static constexpr float REST_Y = 12.0f;
    static constexpr float REST_Z = 0.0f;
};
