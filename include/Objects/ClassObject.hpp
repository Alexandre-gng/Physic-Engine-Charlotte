#pragma once

#include "../common.hpp"
#include "../ClassJoint.hpp"
#include "../ClassTriangle.hpp"
#include "../Constraints/ClassConstraint.hpp"
#include "../Constraints/ClassStretchingConstraint.hpp"

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
    bool                                moving;

    vector<Particle*>                   LIST_particles;

    vector<vector<Triangle*>>           TAB_triangles;
    vector<shared_ptr<Constraint>>      constraints_list;

    // Delete a Joint between two Particles
    void deleteJoint(Joint* ptr_J);

    Object() = default;
    Object(type t, int h_P, int w_P, int h_T, int w_T) : object_type(t) {
        LIST_particles.resize(h_P * w_P);
        TAB_triangles.resize(h_T, std::vector<Triangle*>(w_T, nullptr));
    }

    // Destructor
    virtual ~Object() = default;
    // Update the position of the object
    virtual void update() = 0;


    void detect_Particle(glm::vec2 mouse_pos);
    virtual void supp_Particle(Particle* ptr_P) = 0;
};
