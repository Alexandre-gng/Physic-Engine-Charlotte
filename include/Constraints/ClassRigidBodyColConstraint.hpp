#pragma once

#include "ClassConstraint.hpp"

class Object;

/*
 * Handle a collision between a Particle and a RigidBody
 */

class RigidBodyCollisionConstraint : public Constraint {
public:
    void apply() override;

    RigidBodyCollisionConstraint(int d, Object* ptr_o)
            : Constraint(ptr_o){};
};

