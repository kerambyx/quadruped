#include "Leg.h"
#include <algorithm>
#include <cmath>
#include "pico/stdlib.h"

static constexpr float RAD2DEG = 180.0f / static_cast<float>(M_PI);

static constexpr float STAND_X  =  0.5f;
static constexpr float STAND_Y  = 15.0f;
static constexpr float STAND_Z  =  0.0f;

static constexpr float CROUCH_X = -1.0f;
static constexpr float CROUCH_Y =  8.0f;
static constexpr float CROUCH_Z =  0.0f;

Leg::Leg(const LegConfig &config)
    : _cfg(config),
      _coxa (config.coxa),
      _femur(config.femur),
      _tibia(config.tibia) {}

// 3-DOF IK.
// Coxa rotation is solved in the Y–Z plane.
// Femur and tibia are solved as a 2R planar arm in the X–Y_local plane.
void Leg::solveIK(float x, float y, float z,
                  float &outCoxa, float &outFemur, float &outTibia) const {
    const float L1 = _cfg.femurLength;
    const float L2 = _cfg.tibiaLength;
    const float Lc = _cfg.coxaLength;

    const float r_yz    = sqrtf(y * y + z * z);
    const float y_loc   = r_yz - Lc;
    const float coxaRad = atan2f(z, y);

    const float d2     = x * x + y_loc * y_loc;
    const float cosPhi = std::clamp((d2 - L1 * L1 - L2 * L2) / (2.0f * L1 * L2),
                                    -1.0f, 1.0f);
    const float phi    = acosf(cosPhi);
    const float hipRad = atan2f(y_loc, x)
                       - atan2f(L2 * sinf(phi), L1 + L2 * cosf(phi));

    outCoxa  = 90.0f + coxaRad * RAD2DEG;
    outFemur = 90.0f + hipRad  * RAD2DEG;
    outTibia = 90.0f + (180.0f - phi * RAD2DEG) - outFemur;
}

void Leg::moveTo(float x, float y, float z) {
    float c, f, t;
    solveIK(x, y, z, c, f, t);
    _coxa .setAngle(c);
    _femur.setAngle(f);
    _tibia.setAngle(t);
}

void Leg::home()   { moveTo(_cfg.femurLength, _cfg.tibiaLength, 0.0f); }
void Leg::stand()  { moveTo(STAND_X, STAND_Y, STAND_Z); }
void Leg::crouch() { moveTo(CROUCH_X, CROUCH_Y, CROUCH_Z); }
void Leg::fold()   { moveTo(-_cfg.tibiaLength, 0.0f, _cfg.femurLength); }

void Leg::moveToSmooth(float x, float y, float z,
                       float maxDegPerSec, uint16_t stepMs) {
    float endCoxa, endFemur, endTibia;
    solveIK(x, y, z, endCoxa, endFemur, endTibia);

    float curCoxa  = _coxa .getAngle();
    float curFemur = _femur.getAngle();
    float curTibia = _tibia.getAngle();

    const float maxStep = maxDegPerSec * (static_cast<float>(stepMs) / 1000.0f);

    auto step = [maxStep](float cur, float end, bool &done) -> float {
        const float diff = end - cur;
        if (fabsf(diff) <= maxStep) return end;
        done = false;
        return cur + (diff > 0.0f ? maxStep : -maxStep);
    };

    while (true) {
        bool done = true;
        curCoxa  = step(curCoxa,  endCoxa,  done);
        curFemur = step(curFemur, endFemur, done);
        curTibia = step(curTibia, endTibia, done);
        _coxa .setAngle(curCoxa);
        _femur.setAngle(curFemur);
        _tibia.setAngle(curTibia);
        if (done) break;
        sleep_ms(stepMs);
    }
}

void Leg::enable() {
    _enabled = true;
    _coxa .enable();
    _femur.enable();
    _tibia.enable();
}

void Leg::disable() {
    _enabled = false;
    _coxa .disable();
    _femur.disable();
    _tibia.disable();
}


void Leg::testAngles(float coxaDeg, float femurDeg, float tibiaDeg) {
    _coxa.setAngle(coxaDeg);
    _femur.setAngle(femurDeg);
    _tibia.setAngle(tibiaDeg);
}