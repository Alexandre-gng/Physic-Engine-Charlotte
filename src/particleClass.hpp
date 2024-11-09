//
// Created by alexa on 08/11/2024.
//

#ifndef OPENGL_GLFW_PROJECT_PARTICLECLASS_HPP
#define OPENGL_GLFW_PROJECT_PARTICLECLASS_HPP

#include "includeHeader.hpp"
/*
*   -> "particleClass"  =>  contient pos, vel, acc...
*                       =>  contient des fonctions uniques et intrasèques à chaque particule
*                              => update_vel()  update_pose()
*/

class Particle {
public:
    bool         moving      = true;
    int          id;
    float        friction;
    float        mass        = 1.0f;
    glm::vec3 position;
    glm::vec3 prev_position;
    glm::vec3 forces;
    glm::vec3 velocity;
    glm::vec3 acc;
    Particle(glm::vec3 pos, glm::vec3 prev_pos) {
        position = pos;
        prev_position = prev_pos;
    }

    void update_vel(float dt) {
        velocity = dt * glm::vec3(position-prev_position);
    }
    void update_acc(float dt) {
        forces = {0.f, 0.f, 0.f};
        if (moving) {
            applyGravity(GRAVITY);
            applyFriction(FRICTION);
        }
        acc = forces * (1/mass);
    }
};

#endif //OPENGL_GLFW_PROJECT_PARTICLECLASS_HPP
