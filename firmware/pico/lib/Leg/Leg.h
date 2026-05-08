#pragma once
#include "Joint.h"
#include <math.h>

class Leg {
private:
    Joint coxa;  // Hip pan
    Joint femur; // Hip tilt
    Joint tibia; // Knee

    // physical parameters (mm
    const float L_COXA = 30.0f;
    const float L_FEMUR = 50.0f;
    const float L_TIBIA = 60.0f;

public:
    Leg(uint coxa_pin, uint femur_pin, uint tibia_pin);
    
    // is_right_side allows the leg to invert its joints automatically
    void init(bool is_right_side);
    
    // inverse kinematics math function
    void moveTo(float x, float y, float z);
    
    void home();
    void restPosition();
};