#include "../include/Joint.hpp"
#include "../include/Particle.hpp"

Joint::Joint(Particle* ptr_P1, Particle* ptr_P2): particle1(ptr_P1), particle2(ptr_P2) {
    glm::vec3 delta_pos = ptr_P2->pos - ptr_P1->pos;
    ptr_P1->LIST_joints.push_back(this);
    ptr_P2->LIST_joints.push_back(this);
    // Use glm::length() for optimized distance calculation and include z component
    distance = glm::length(delta_pos);
}

void Joint::update_distance() {
    glm::vec3 delta_pos = particle1->pos - particle2->pos;
    // Use glm::length() for optimized distance calculation
    distance = glm::length(delta_pos);
}