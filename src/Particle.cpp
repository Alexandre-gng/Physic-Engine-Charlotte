#include "../../OPENGL/include/ClassParticle.hpp"
#include "../../OPENGL/include/ClassJoint.hpp"

// YYY to supp tout ça ?

void Particle::applyFriction() {
    forces -= velocity * friction;
}
void Particle::applyGravity(float grav) {
    if (moving) {
        forces.y += mass * grav;
    }
}

void Particle::updateVelocity(float dt) {
    this->velocity = (pos - prev_pos) / dt;
}

// YYY TO DO
void Particle::cutTwoParticles(Particle* Pa) {
    for (const auto j: this->list_joints) {
        if ((j->particle2 == Pa || j->particle1 == Pa) && (j->particle2 == this || j->particle1 == this)) {
            //j->deleteJoint();
        }
    }
}