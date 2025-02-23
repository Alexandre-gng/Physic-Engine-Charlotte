#pragma once

#include "Constraint.hpp"

class BendingConstraint : public Constraint {
public:
    void apply() override;
};