#include "../include/Engine/Physic.hpp"

// Position Based Dynamic algorithm
void Physic::PBD(float dt, float k_damping, int constraints_iter) {
    for (const auto ptr_O: this->objects_list) {
        if (!ptr_O->moving) {continue;}
        
        // First pass: apply forces and update velocities
        for (const auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->forces = {0.f, 0.f, 0.0f};
            ptr_P->applyGravity(this->gravity);
            ptr_P->applyFriction();
            ptr_P->velocity = ptr_P->velocity + ptr_P->forces * ptr_P->inverse_mass * dt;
        }

        damping_velocities(k_damping, ptr_O);
        
        // Second pass: update positions based on velocity
        for (auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->pos = ptr_P->prev_pos + dt * ptr_P->velocity;
        }
        
        // Solving constraints
        for (int i = 0; i < constraints_iter; i++) {
            for (const auto &constraint: ptr_O->LIST_constraints) {
                constraint->apply();
            }
        }
        
        // Final pass: update velocity from position change and store previous position
        // Pre-compute inverse dt to avoid repeated division
        float inv_dt = 1.0f / dt;
        for (auto& ptr_P: ptr_O->LIST_particles) {
            if (ptr_P == nullptr) {continue;}
            ptr_P->velocity = (ptr_P->pos - ptr_P->prev_pos) * inv_dt;
            ptr_P->prev_pos = ptr_P->pos;
        }
    }
}


// Damping velocities to reduce energy accumulation
void Physic::damping_velocities(float k_damping, Object* ptr_O) {
    glm::vec3 x_center = {0.f, 0.f, 0.0f};
    glm::vec3 v_center = {0.f, 0.f, 0.0f};
    float total_mass = 0.f;
    
    // Center of mass and velocity - combined loop for better cache locality
    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr) {continue;}
        total_mass += ptr_P->mass;
        x_center += ptr_P->pos * ptr_P->mass;
        v_center += ptr_P->velocity * ptr_P->mass;
    }
    
    // Pre-compute inverse of total mass
    float inv_total_mass = 1.0f / total_mass;
    x_center *= inv_total_mass;
    v_center *= inv_total_mass;
    
    // Angular movement
    float L = 0.f;

    // Inertia tensor
    glm::mat3x3 I = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr) {continue;}
        // Compute relative position vector once
        glm::vec3 r = ptr_P->pos - x_center;

        // r~ * r~T: (using r.x*r.x for diagonal instead of original r_x*r_x for all)
        float r_sq = r.x * r.x;
        glm::mat3x3 r_result = {{r_sq, 0, 0}, {0, r_sq, 0}, {0, 0, r_sq}};
        I += r_result * ptr_P->mass;
        
        // cross product and angular moment
        float r_x_v = (r.x * ptr_P->velocity.y) - (r.y - ptr_P->velocity.x);
        L += r_x_v * ptr_P->mass;
    }
    
    glm::mat3x3 I_inverse = glm::inverse(I);

    // angular velocity
    glm::mat3x3 w = I_inverse * L;

    // Apply damping to moving particles
    for (const auto& ptr_P: ptr_O->LIST_particles) {
        if (ptr_P == nullptr || !ptr_P->moving) {continue;}
        glm::vec3 r = ptr_P->pos - x_center;
        glm::vec3 delta_velocity = v_center + w * r - ptr_P->velocity;
        ptr_P->velocity += k_damping * delta_velocity;
    }
}