#include "../include/Constraints/StretchingConstraint.hpp"
#include "../../OPENGL/include/Particle.hpp"
#include "../include/Objects/Object.hpp"


void StretchingConstraint::apply() {
    // YYY
    unsigned int TEST = 0;
    for (auto &ptr_P1: ptr_object->LIST_particles)
        /*
        if (ptr_P1->id == 7) {
            cout << "l11 " << endl;
            cout << "x = " << ptr_P1->pos.x << "  y = " << ptr_P1->pos.y << "   z = " << ptr_P1->pos.z << endl;
        }
        */
    for (auto j: ptr_P1->LIST_joints) {
        Particle *ptr_P2 = nullptr;
        if (j->particle2 == ptr_P1) {
            ptr_P2 = j->particle1;
        } else {
            ptr_P2 = j->particle2;
        }
        glm::vec3 delta_pos = ptr_P2->pos - ptr_P1->pos;
        float current_distance = sqrt(
                delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y + delta_pos.z * delta_pos.z);
        /*
        if (current_distance == 0) {
            cout << "current distance null" << endl;
        }

        float ah = ((ptr_P1->inverse_mass) * (current_distance - j->distance));
        float eh = ((ptr_P2->inverse_mass + ptr_P1->inverse_mass) * current_distance);
        float oh = current_distance *ah / eh;
        glm::vec3 vec3finale = oh * delta_pos;
        cout << "vecx = " << vec3finale.x << "   vecy = " << vec3finale.y << "   vecz = " << vec3finale.z << endl;
        if (!vec3finale.x) {
            TEST ++;
            cout << "TEST = " << TEST << endl;
        }
         */
        glm::vec3 deltaP1, deltaP2;
        if (current_distance != 0) {
            deltaP1 = (-(ptr_P2->inverse_mass) * (current_distance - j->distance) /
                       ((ptr_P2->inverse_mass + ptr_P1->inverse_mass) * current_distance)) *
                      delta_pos;

            // BUG HERE YYY => current_distance = 0 donc vec3finale NaN
            deltaP2 = ((ptr_P1->inverse_mass) * (current_distance - j->distance) /
                       ((ptr_P2->inverse_mass + ptr_P1->inverse_mass) * current_distance)) *
                      delta_pos;
        } else {
            cout << "current distance null" << endl;
        }

        if ((current_distance > 0) && (current_distance >= j->distance * 1.25f)) {
            ptr_P1->cutTwoParticles(ptr_P2);
            continue;
        }
        if (!ptr_P1->moving && ptr_P2->moving) {
            ptr_P2->pos += deltaP1 * 2.0f;
        }
        if (ptr_P1->moving && !ptr_P2->moving) {
            ptr_P1->pos += deltaP2 * 2.0f;
        }
        if (ptr_P1->moving && ptr_P2->moving) {
            ptr_P1->pos += deltaP2;
            ptr_P2->pos += deltaP1;
        }
    }
}
        /*
        if (ptr_P1->id == 7) {
            cout << "l65 " << endl;
            cout << "x = " << ptr_P1->pos.x << "  y = " << ptr_P1->pos.y << "   z = " << ptr_P1->pos.z << endl;
        }*/
    // YYY
    // cout << "TEST = " << TEST << endl;

