#pragma once

#include "../common.hpp"
#include "../Joint.hpp"
#include "../Triangle.hpp"
#include "../Constraints/Constraint.hpp"
#include "../Constraints/StretchingConstraint.hpp"

class Triangle;
class Constraint;


using namespace std;


class Object {
public:
    enum type {
        CLOTH,
        WALL,
        CUBE,
    };
    type                                object_type;
    // YYY
    bool                                moving;

    vector<Particle*>                   LIST_particles;
    // YYY Changer en LIST ?
    vector<Triangle*>                   LIST_triangles;
    // vector<vector<Triangle*>>           TAB_triangles;
    /* TAB_triangles :
     *     o--o--o--o
     *     | \| \| \| \
     *     o--o--o--o--o
     *     | /| /| /| /
     *     o--o--o--o
     *     | \| \| \| \
     *     o--o--o--o--o
     *     ...
     */

    vector<shared_ptr<Constraint>>      LIST_constraints;

    // Delete a Joint between two Particles
    void delete_Joint(Joint* ptr_J);

    Object(type t, int h_P, int w_P, int h_T, int w_T) : object_type(t) {
        if (t == CLOTH) {
            LIST_particles.resize(h_P * w_P);
            LIST_triangles.resize(h_T * (w_T - 2));
        }
        if (t == WALL) {
            LIST_particles.resize(4);
            LIST_triangles.resize(2);
        }
        if (t == CUBE) {
            LIST_particles.resize(36);
            LIST_triangles.resize(12);
        }
    }

    virtual ~Object() = default;

    // YYY RIEN A FAIRE LA, DEVRAIT ALLER DANS UNE CLASSE "ENGINE" ?
    void detect_Particle(glm::vec3 mouse_pos);
    // YYY pas virtual
    void supp_Particle(Particle* ptr_P);
};
