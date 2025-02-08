#pragma once

#include <vector>
#include <memory>
#include <iostream>
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
    int          default_lenght;
    // YYY Nécessaire ?
    int          mass_particles;
    int          height;
    int          width;

    Cloth(int x, int y, int z, int w, int h, float d,int m_p, float frict);

    // REFAIRE AVEC L'ALGO YYY ?
    void supp_Particle(Particle* ptr_P);

};