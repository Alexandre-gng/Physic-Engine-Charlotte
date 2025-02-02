#include "../../OPENGL/include/Joint.hpp"
#include "../../OPENGL/include/Particle.hpp"
// Fix the error in the constructor

Joint::Joint(Particle* ptr_P1, Particle* ptr_P2): particle1(ptr_P1), particle2(ptr_P2) {
    glm::vec3 delta_pos = ptr_P2->pos - ptr_P1->pos;
    ptr_P1->LIST_joints.push_back(this);
    ptr_P2->LIST_joints.push_back(this);
    distance = sqrt((delta_pos.x)*(delta_pos.x) + (delta_pos.y*delta_pos.y));
}
