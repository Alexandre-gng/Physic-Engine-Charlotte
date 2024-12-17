#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "ClassObject.hpp"
#include "../ClassParticle.hpp"
#include "../ClassJoint.hpp"

class Wall : public Object {
public:
    int                         w, h;
    float                       friction;
    Eigen::Vector2f             normal_vector;

    void update() override {
        cout << "updating" << endl;
        return;
    }

    Wall(int x, int y, int w, int h, float d, float m_p, float frict): friction(frict), w(w), h(h), Object(WALL, 2, 2, 1, 2) {
        Particle* ptr_NewP1 = new Particle(x, y, m_p);
        Particle* ptr_NewP2 = new Particle(x+w, y, m_p);
        Particle* ptr_NewP3 = new Particle(x, y+h, m_p);
        Particle* ptr_NewP4 = new Particle(x+w, y+h, m_p);
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

        // Calculate the normal vector of the wall, need to be done in 3D.
        // See "produit vectoriel" in french
        // normal_vector =
    };
};