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
        ROPE,
        WALL,
    };
    type                                object_type;
    // YYY
    bool                                moving;

    vector<Particle*>                   LIST_particles;

    vector<vector<Triangle*>>           TAB_triangles;
    vector<shared_ptr<Constraint>>      LIST_constraints;

    // Delete a Joint between two Particles
    void delete_Joint(Joint* ptr_J);

    Object(type t, int h_P, int w_P, int h_T, int w_T) : object_type(t) {
        LIST_particles.resize(h_P * w_P);
        TAB_triangles.resize(h_T, std::vector<Triangle*>(w_T-2, nullptr));
    }

    virtual ~Object() = default;

    // YYY RIEN A FAIRE LA, DEVRAIT ALLER DANS UNE CLASSE "ENGINE" ?
    void detect_Particle(glm::vec3 mouse_pos);
    virtual void supp_Particle(Particle* ptr_P) = 0;
};
