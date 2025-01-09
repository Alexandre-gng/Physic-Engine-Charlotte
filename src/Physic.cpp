#include "../../OPENGL/include/Physic.hpp"

// Position Based Dynamic algorithm
void Physic::PBD(float dt, float k_damping, int constraints_iter) {
    for (const auto ptr_O: this->objects_list) {

        for (const auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->forces = {0.f, 0.f, 0.0f};
            ptr_P->applyGravity(this->gravity);
            ptr_P->applyFriction();
            ptr_P->velocity = ptr_P->velocity + ptr_P->forces * ptr_P->inverse_mass * dt;
        }

        damping_velocities(k_damping, ptr_O);

        for (const auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->pos = ptr_P->prev_pos + dt * ptr_P->velocity;
        }
        // Constraints detections (collisions...) YYY
        // HASH MAP OR BARNES-HUT ?

        // Solving constraints
        for (int i = 0; i < constraints_iter; i++) {
            for (const auto &constraint: ptr_O->LIST_constraints) {
                constraint->apply();
            }
        }

        for (const auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->velocity = (ptr_P->pos - ptr_P->prev_pos) / dt;
            ptr_P->prev_pos = ptr_P->pos;
        }
    }
}


// Damping velocities to reduce energy accumulation*
// YYY CHANGE EIGEN TO GLM
void Physic::damping_velocities(float k_damping, Object* ptr_O) {
    glm::vec3 x_center = {0.f, 0.f, 0.0f};
    glm::vec3 v_center = {0.f, 0.f, 0.0f};
    float total_mass = 0.f;
    // Center of mass and velocity
    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr) {continue;}
        total_mass += ptr_P->mass;
        x_center += ptr_P->pos * ptr_P->mass;
        v_center += ptr_P->velocity * ptr_P->mass;
    }
    x_center = x_center * (1 / total_mass);
    v_center = v_center * (1 / total_mass);
    // Angular movement
    float L = 0.f;

    // Inertia tensor
    glm::mat3x3 I = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    /*
    Eigen::Matrix2f I;
    I << 0, 0,
         0, 0;
    */
    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr) {continue;}
        float r_x = ptr_P->pos.x - x_center.x;
        float r_y = ptr_P->pos.y - x_center.y;
        float r_z = ptr_P->pos.z - x_center.z;

        // r~ * r~T:
        /*
        Eigen::Matrix2f r_result;
        r_result << r_x*r_x, 0,
                0,  r_x*r_x;
        I = I + r_result * ptr_P->mass;
        */
        glm::mat3x3 r_result = {{r_x*r_x, 0, 0}, {0, r_x*r_x, 0}, {0, 0, r_x*r_x}};
        I = I + r_result * ptr_P->mass;
        
        // cross product and angular moment
        // YYY
        float r_x_v = ((r_x * ptr_P->velocity.y) - (r_y - ptr_P->velocity.x));
        L += r_x_v * ptr_P->mass;
    }
    glm::mat3x3 I_inverse;
    I_inverse = glm::inverse(I);
    /*
    Eigen::Matrix2f I_inverse;
    I_inverse = I.inverse();
    */

    // angular velocity
    glm::mat3x3 w;
    w = I_inverse * L;
    /*
    Eigen::Matrix2f w;
    w = I_inverse * L;
    */
    /*
    Eigen::Vector2f delta_velocity;
    */
    glm::vec3 delta_velocity;
    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr) {continue;}
        /*
        Eigen::Vector2f r(ptr_P->pos.x - x_center.x, ptr_P->pos.y - x_center.y);
        Eigen::Vector2f v_center_eigen(v_center.x, v_center.y);
        Eigen::Vector2f velocity_eigen(ptr_P->velocity.x, ptr_P->velocity.y);
        delta_velocity = v_center_eigen + w * r - velocity_eigen;
        */

        glm::vec3 r(ptr_P->pos.x - x_center.x, ptr_P->pos.y - x_center.y, ptr_P->pos.z - x_center.z);
        glm::vec3 v_center_eigen(v_center.x, v_center.y, v_center.z);
        delta_velocity = v_center_eigen + w * r - ptr_P->velocity;

        ptr_P->velocity = ptr_P->velocity + k_damping * delta_velocity;
        /*
         * OLD VERSION:
        ptr_P->velocity = ptr_P->velocity + k_damping * sf::Vector2f(delta_velocity.x(), delta_velocity.y());
        */
    }
}