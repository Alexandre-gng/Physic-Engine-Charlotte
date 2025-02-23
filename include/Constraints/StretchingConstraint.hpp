#pragma once

#include "Constraint.hpp"

class Object;

/*
 * Handle distance collision between two Particles in an Object
 */

class StretchingConstraint : public Constraint {
public:
    void apply() override;

     StretchingConstraint(int d, Object* ptr_o)
    : Constraint(ptr_o){};
};

