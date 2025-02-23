#pragma once

#include "common.hpp"

class Particle;

class Joint {
public:
    Joint(Particle* p1, Particle* p2);
    Particle* particle1;
    Particle* particle2;
    float distance;

    void update_distance();
};