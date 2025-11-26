#include "../include/Constraints/StretchingConstraint.hpp"
#include "../include/Particle.hpp"
#include "../include/Objects/Object.hpp"
#include <unordered_set>


void StretchingConstraint::apply() {
    // Track processed joints to avoid processing each joint twice
    // (once from particle1's side and once from particle2's side)
    std::unordered_set<Joint*> processed_joints;
    
    for (auto &ptr_P1: ptr_object->LIST_particles) {
        for (auto ptr_J: ptr_P1->LIST_joints) {
            // Skip already processed joints to avoid duplicate work
            if (processed_joints.count(ptr_J) > 0) {
                continue;
            }
            processed_joints.insert(ptr_J);
            
            // Determine which particle is the "other" particle
            Particle *ptr_P2 = (ptr_J->particle2 == ptr_P1) ? ptr_J->particle1 : ptr_J->particle2;
            
            glm::vec3 delta_pos = ptr_P2->pos - ptr_P1->pos;
            // Use glm::length() which is optimized compared to manual sqrt calculation
            float current_distance = glm::length(delta_pos);

            glm::vec3 delta_P1, delta_P2;

            if (current_distance > 0.0f) {
                // Pre-compute common denominator to avoid redundant division
                float inv_sum_mass = 1.0f / (ptr_P2->inverse_mass + ptr_P1->inverse_mass);
                float distance_diff = current_distance - ptr_J->distance;
                float correction_factor = distance_diff / current_distance;
                
                delta_P1 = (-(ptr_P2->inverse_mass) * inv_sum_mass * correction_factor) * delta_pos;
                delta_P2 = ((ptr_P1->inverse_mass) * inv_sum_mass * correction_factor) * delta_pos;
            } else {
                cout << "Error: current_distance null" << endl;
                continue;
            }

            if (current_distance >= ptr_J->distance * 1.25f) {
                ptr_P1->cutTwoParticles(ptr_P2);
                continue;
            }
            
            // Use else-if chain since conditions are mutually exclusive
            if (!ptr_P1->moving && ptr_P2->moving) {
                ptr_P2->pos += delta_P1 * 2.0f;
            } else if (ptr_P1->moving && !ptr_P2->moving) {
                ptr_P1->pos += delta_P2 * 2.0f;
            } else if (ptr_P1->moving && ptr_P2->moving) {
                ptr_P1->pos += delta_P2;
                ptr_P2->pos += delta_P1;
            }

            ptr_J->distance = current_distance;
        }
    }
}