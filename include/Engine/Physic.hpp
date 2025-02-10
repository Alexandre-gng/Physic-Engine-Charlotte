#pragma once

#include <utility>

#include "../Particle.hpp"
#include "../Objects/Object.hpp"

using namespace std;

class Physic {
public:
    float gravity;

    vector<Object*> objects_list;

    void solving_constraints();

    // Position base dynamic method
    void PBD(float dt, float k_damping, int constraints_iter);
    // Reduce the velocity to keep only the global movement
    static void damping_velocities(float k_damping, Object* ptr_O);

    Physic(vector<Object*> vec, float g): objects_list(std::move(vec)), gravity(g) {}
};

