#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "Object.hpp"
#include "../Particle.hpp"
#include "../Joint.hpp"

class Wall : public Object {
public:
    int                         w, h;
    float                       friction;
    glm::vec3                   normal_vector;


    Wall(int x, int y, int z, int w, int h, float d, float m_p, float frict): friction(frict), w(w), h(h), Object(WALL, 2, 2, 1, 2) {
        Particle* ptr_NewP1 = new Particle(x, y, z, m_p);
        Particle* ptr_NewP2 = new Particle(x+w, y, z, m_p);
        Particle* ptr_NewP3 = new Particle(x, y+h, z, m_p);
        Particle* ptr_NewP4 = new Particle(x+w, y+h, z, m_p);
        LIST_particles[0] = ptr_NewP1;
        LIST_particles[1] = ptr_NewP2;
        LIST_particles[2] = ptr_NewP3;
        LIST_particles[3] = ptr_NewP4;

        Joint* ptr_J1 = new Joint(ptr_NewP1, ptr_NewP2);
        Joint* ptr_J2 = new Joint(ptr_NewP2, ptr_NewP3);
        Joint* ptr_J3 = new Joint(ptr_NewP1, ptr_NewP3);
        Joint* ptr_J4 = new Joint(ptr_NewP1, ptr_NewP4);
        Joint* ptr_J5 = new Joint(ptr_NewP3, ptr_NewP4);

        Triangle* ptr_NewT1 = new Triangle(ptr_J1, ptr_J2, ptr_J3);
        Triangle* ptr_NewT2 = new Triangle(ptr_J3, ptr_J4, ptr_J5);
        TAB_triangles[0][0] = ptr_NewT1;
        TAB_triangles[1][0] = ptr_NewT2;


        Particle* ptr_P1 = list_joints[0]->particle1;
        Particle* ptr_P2 = list_joints[0]->particle2;

    };
};