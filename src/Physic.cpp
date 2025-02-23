#include "../include/Engine/Physic.hpp"

// Position Based Dynamic algorithm
void Physic::PBD(float dt, float k_damping, int constraints_iter) {
    for (const auto ptr_O: this->objects_list) {
        if (!ptr_O->moving) {continue;}
        for (const auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->forces = {0.f, 0.f, 0.0f};
            ptr_P->applyGravity(this->gravity);
            ptr_P->applyFriction();
            ptr_P->velocity = ptr_P->velocity + ptr_P->forces * ptr_P->inverse_mass * dt;
        }

        damping_velocities(k_damping, ptr_O);
        /*
        cout << "l17 " << endl;
        cout << "x = " << ptr_O->LIST_particles[6]->pos.x << "  y = " << ptr_O->LIST_particles[6]->pos.y << "   z = " << ptr_O->LIST_particles[6]->pos.z << endl;
        cout << "velo x = " << ptr_O->LIST_particles[6]->velocity.x << "  velo y = " << ptr_O->LIST_particles[6]->velocity.y << "   velo z = " << ptr_O->LIST_particles[6]->velocity.z << endl;
        cout << "prevPos x = " << ptr_O->LIST_particles[6]->prev_pos.x << "  prevPos y = " << ptr_O->LIST_particles[6]->prev_pos.y << "   prevPos z = " << ptr_O->LIST_particles[6]->prev_pos.z << endl;
        */
         for (auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->pos = ptr_P->prev_pos + dt * ptr_P->velocity;
        }
        // Constraints detections (collisions...) YYY
        /*
        cout << "l27 " << endl;
        cout << "x = " << ptr_O->LIST_particles[6]->pos.x << "  y = " << ptr_O->LIST_particles[6]->pos.y << "   z = " << ptr_O->LIST_particles[6]->pos.z << endl;
        */
        // Solving constraints
        for (int i = 0; i < constraints_iter; i++) {
            for (const auto &constraint: ptr_O->LIST_constraints) {
                constraint->apply();
            }
        }
        //cout << "x = " << ptr_O->LIST_particles[6]->pos.x << "  y = " << ptr_O->LIST_particles[6]->pos.y << "   z = " << ptr_O->LIST_particles[6]->pos.z << endl;
        for (auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->velocity = (ptr_P->pos - ptr_P->prev_pos) / dt;
            ptr_P->prev_pos = ptr_P->pos;
        }
    }
}


// Damping velocities to reduce energy accumulation*
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

    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr) {continue;}
        float r_x = ptr_P->pos.x - x_center.x;
        float r_y = ptr_P->pos.y - x_center.y;
        float r_z = ptr_P->pos.z - x_center.z;

        // r~ * r~T:
        glm::mat3x3 r_result = {{r_x*r_x, 0, 0}, {0, r_x*r_x, 0}, {0, 0, r_x*r_x}};
        I = I + r_result * ptr_P->mass;
        
        // cross product and angular moment
        // YYY
        float r_x_v = ((r_x * ptr_P->velocity.y) - (r_y - ptr_P->velocity.x));
        L += r_x_v * ptr_P->mass;
    }
    glm::mat3x3 I_inverse;
    I_inverse = glm::inverse(I);

    // angular velocity
    glm::mat3x3 w;
    w = I_inverse * L;

    glm::vec3 delta_velocity;
    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr || !ptr_P->moving) {continue;}
        glm::vec3 r(ptr_P->pos.x - x_center.x, ptr_P->pos.y - x_center.y, ptr_P->pos.z - x_center.z);
        glm::vec3 v_center_eigen(v_center.x, v_center.y, v_center.z);
        delta_velocity = v_center_eigen + w * r - ptr_P->velocity;

        ptr_P->velocity = ptr_P->velocity + k_damping * delta_velocity;
    }
}