#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <unordered_set>
#include "Object.hpp"
#include "../Particle.hpp"
#include "../Joint.hpp"
#include "../Constraints/StretchingConstraint.hpp"

using namespace std;

/*
 * Constraints:
 *      => Strecthing OK
 *      => Bending TO_DO
 *      => With rigid body
 *      => Self_collision
 *      => External and pressurized forces ?
 *
 */
class Cloth : public Object {
public:
    float        default_lenght;
    float        mass_particles;
    int          height;
    int          width;

    Cloth(int x, int y, int z, int w, int h, float d, float m_p, float frict);

    void supp_Particle(Particle* ptr_P);

};