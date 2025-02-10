#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "Object.hpp"
#include "../Particle.hpp"
#include "../Joint.hpp"


// YYY
// Ajouter une option de rotation
class Wall : public Object {
public:
    int                         w, h;
    // YYY NECESSARY ?
    glm::vec3                   pos;
    glm::vec3                   normal_vector;

    // YYY Maybe ?
    // float                       dampingFactor;


    void supp_Particle(Particle* ptr_P) {
        return;
    }

    /*        J1
    *      A ---- B
    *  J3  |  J2/ |  J4
    *      |  /   |
    *      C ---- D
    *         J5
    *
    */
    Wall(int x, int y, int z, int w, int h): w(w), h(h), Object(WALL, 2, 2, 1, 2) {
        Particle* ptr_NewP_A = new Particle(x, y, z);
        Particle* ptr_NewP_B = new Particle(x+w, y, z);
        Particle* ptr_NewP_C = new Particle(x, y+h, z);
        Particle* ptr_NewP_D = new Particle(x+w, y+h, z);
        LIST_particles[0] = ptr_NewP_A;
        LIST_particles[1] = ptr_NewP_B;
        LIST_particles[2] = ptr_NewP_C;
        LIST_particles[3] = ptr_NewP_D;

        Joint* ptr_J1 = new Joint(ptr_NewP_A, ptr_NewP_B);
        Joint* ptr_J2 = new Joint(ptr_NewP_B, ptr_NewP_C);
        Joint* ptr_J3 = new Joint(ptr_NewP_A, ptr_NewP_C);
        Joint* ptr_J4 = new Joint(ptr_NewP_B, ptr_NewP_D);
        Joint* ptr_J5 = new Joint(ptr_NewP_C, ptr_NewP_D);

        Triangle* ptr_NewT1 = new Triangle(ptr_J1, ptr_J2, ptr_J3);
        Triangle* ptr_NewT2 = new Triangle(ptr_J2, ptr_J4, ptr_J5);
        LIST_triangles[0] = ptr_NewT1;
        LIST_triangles[1] = ptr_NewT2;
    };
};