#pragma once

using namespace std;

#include "Joint.hpp"
#include "Particle.hpp"

class Triangle {
public:
    vector<Joint*> list_joints;
    vector<Triangle*> list_nearest_triangles;
    // YYY TO CHANGE TO A VECTOR IN 3D (produit vectoriel)
    glm::vec3 normal_vector;

    // Triangle constructor: add the 3 joints to the list_joints and add the ptr_triangle to each Particle concerned
    Triangle(Joint* ptr_J1, Joint* ptr_J2, Joint* ptr_J3) {
        list_joints.push_back(ptr_J1);
        list_joints.push_back(ptr_J2);
        list_joints.push_back(ptr_J3);
        // Add the ptr_triangle to each Particle concerned
        ptr_J1->particle1->list_triangles_friends.push_back(this);
        ptr_J1->particle2->list_triangles_friends.push_back(this);
        if (ptr_J2->particle1 != ptr_J1->particle1 && ptr_J2->particle1 != ptr_J1->particle2) {
            ptr_J2->particle1->list_triangles_friends.push_back(this);
        } else {
            ptr_J2->particle2->list_triangles_friends.push_back(this);
        }
        update_normal();
    }


    // Update the normal_vector to the triangle
    void update_normal() {
        Particle* ptrA = list_joints[0]->particle1;
        Particle* ptrB = list_joints[0]->particle2;
        if (list_joints[1]->particle1 == ptrB) {
            if (list_joints[2]->particle1 == ptrA) {
                normal_vector = glm::normalize(glm::cross(ptrB->pos - ptrA->pos, list_joints[1]->particle2->pos - ptrB->pos));
            } else {
                normal_vector = glm::normalize(glm::cross(ptrB->pos - ptrA->pos, list_joints[2]->particle1->pos - ptrB->pos));
            }
        } else {
            if (list_joints[2]->particle1 == ptrA) {
                normal_vector = glm::normalize(glm::cross(ptrB->pos - ptrA->pos, list_joints[1]->particle1->pos - ptrB->pos));
            } else {
                normal_vector = glm::normalize(glm::cross(ptrB->pos - ptrA->pos, list_joints[2]->particle2->pos - ptrB->pos));
            }
        }
    }

    // Update the normal_vector to the triangle, a scalar in 2D and vector in 3D
};
