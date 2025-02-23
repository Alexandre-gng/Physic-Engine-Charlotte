#include "../include/Joint.hpp"
#include "../include/Particle.hpp"
// Fix the error in the constructor

Joint::Joint(Particle* ptr_P1, Particle* ptr_P2): particle1(ptr_P1), particle2(ptr_P2) {
    glm::vec3 delta_pos = ptr_P2->pos - ptr_P1->pos;
    ptr_P1->LIST_joints.push_back(this);
    ptr_P2->LIST_joints.push_back(this);
    distance = sqrt((delta_pos.x)*(delta_pos.x) + (delta_pos.y*delta_pos.y));
}

void Joint::update_distance() {
    glm::vec3 delta_pos = particle1->pos - particle2->pos;
    float current_distance = sqrt(delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y + delta_pos.z * delta_pos.z);

    distance = current_distance;
}