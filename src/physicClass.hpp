#ifndef OPENGL_GLFW_PROJECT_PHYSICCLASS_HPP
#define OPENGL_GLFW_PROJECT_PHYSICCLASS_HPP

#include "includeHeader.hpp"

using namespace std;

class Physic {
public:
    vector<Particle> alone_particles;
    vector<Object> objects;

    void apply_friction_particle() {
        for (Particle particle : alone_particles) {
            particle.forces -= FRICTION * particle.velocity;
        }
    }
    void apply_gravity_particles(float grav) {
        for (Particle particle : alone_particles) {
            if (particle.moving) {
                particle.forces.y -= GRAVITY * particle.mass;
            }
        }
    }

};

#endif