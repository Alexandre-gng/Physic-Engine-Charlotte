#pragma once

#include <vector>
#include "Object.hpp"

// YYY
class Cube : public Object {
public:
    float       mass;
    float       size;
    glm::vec3   pos;

    Cube(int x, int y, int z, float s, float m);
};