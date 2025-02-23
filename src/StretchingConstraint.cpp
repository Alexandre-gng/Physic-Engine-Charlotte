#include "../include/Constraints/StretchingConstraint.hpp"
#include "../include/Particle.hpp"
#include "../include/Objects/Object.hpp"


void StretchingConstraint::apply() {
    for (auto &ptr_P1: ptr_object->LIST_particles) {
        for (auto ptr_J: ptr_P1->LIST_joints) {
            /* YYY
            if (ptr_P1->id == 7) {
                // => Distance jamais mise à jour <!>
                cout << "relative distance j0 = " << ptr_P1->LIST_joints[0]->distance << endl;
                cout << "relative distance j1 = " << ptr_P1->LIST_joints[1]->distance << endl;
                cout << "relative distance j2 = " << ptr_P1->LIST_joints[2]->distance << endl;
            }*/
            // YYY Pourquoi ce réseau de conditions ?
            Particle *ptr_P2 = nullptr;
            if (ptr_J->particle2 == ptr_P1) {
                ptr_P2 = ptr_J->particle1;
            } else {
                ptr_P2 = ptr_J->particle2;
            }
            glm::vec3 delta_pos = ptr_P2->pos - ptr_P1->pos;
            float current_distance = sqrt(
                    delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y + delta_pos.z * delta_pos.z);

            glm::vec3 delta_P1, delta_P2;

            // YYY D'où ça vient ?
            if (current_distance != 0) {
                delta_P1 = (-(ptr_P2->inverse_mass) * (current_distance - ptr_J->distance) /
                           ((ptr_P2->inverse_mass + ptr_P1->inverse_mass) * current_distance)) *
                          delta_pos;

                delta_P2 = ((ptr_P1->inverse_mass) * (current_distance - ptr_J->distance) /
                           ((ptr_P2->inverse_mass + ptr_P1->inverse_mass) * current_distance)) *
                          delta_pos;
            } else {
                cout << "Error: current_distance null" << endl;
            }
            // YYY RECENT ADD, GOOD ?

            if ((current_distance > 0) && (current_distance >= ptr_J->distance * 1.25f)) {
                ptr_P1->cutTwoParticles(ptr_P2);
                continue;
            }
            if (!ptr_P1->moving && ptr_P2->moving) {
                ptr_P2->pos += delta_P1 * 2.0f;
            }
            if (ptr_P1->moving && !ptr_P2->moving) {
                ptr_P1->pos += delta_P2 * 2.0f;
            }
            if (ptr_P1->moving && ptr_P2->moving) {
                ptr_P1->pos += delta_P2;
                ptr_P2->pos += delta_P1;
            }

            ptr_J->distance = current_distance;
        }
    }
}