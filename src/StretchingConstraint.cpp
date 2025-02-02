#include "../include/Constraints/StretchingConstraint.hpp"
#include "../../OPENGL/include/Particle.hpp"
#include "../include/Objects/Object.hpp"


void StretchingConstraint::apply() {
    for (const auto& ptr_P1: ptr_object->LIST_particles) {
        for (auto j: ptr_P1->LIST_joints) {
            Particle *ptr_P2;
            if (j->particle2 == ptr_P1) {
                ptr_P2 = j->particle1;
            } else {
                ptr_P2 = j->particle2;
            }
            glm::vec3 delta_pos = ptr_P2->pos - ptr_P1->pos;
            float current_distance = sqrt(delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y + delta_pos.z * delta_pos.z);

            glm::vec3 deltaP1 = (-(ptr_P2->inverse_mass) * (current_distance - j->distance) /
                                    ((ptr_P2->inverse_mass + ptr_P1->inverse_mass) * current_distance)) *
                                   delta_pos;
            glm::vec3 deltaP2 = ((ptr_P1->inverse_mass) * (current_distance - j->distance) /
                                    ((ptr_P2->inverse_mass + ptr_P1->inverse_mass) * current_distance)) *
                                   delta_pos;

            if ((current_distance > 0) && (current_distance >= j->distance * 1.25f)) {
                ptr_P1->cutTwoParticles(ptr_P2);
                continue;
            }
            if (!ptr_P1->moving && ptr_P2->moving) {
                ptr_P2->pos += deltaP1 * 2.0f;
            } if (ptr_P1->moving && !ptr_P2->moving) {
                ptr_P1->pos += deltaP2 * 2.0f;
            } if (ptr_P1->moving && ptr_P2->moving) {
                ptr_P1->pos += deltaP2;
                ptr_P2->pos += deltaP1;
            }
        }
    }
}

