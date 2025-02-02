#pragma once

#include <vector>
#include "Object.hpp"


// YYY
// TO_DO
class Cube : public Object {
public:
    float       size;
    glm::vec3   pos;

    Cube(type t,int x, int y, int z, int size) : Object(t, 0, 0, 0, 0) {
        // YYY TO_DO
    }
};