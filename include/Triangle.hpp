#pragma once

using namespace std;

#include "Joint.hpp"
#include "Particle.hpp"

class Triangle {
public:
    vector<Joint*>      list_joints;
    vector<Triangle*>   list_nearest_triangles;
    glm::vec3           normal_vector;

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
        Particle* ptr_P1 = list_joints[0]->particle1;
        Particle* ptr_P2 = list_joints[0]->particle2;
        if (list_joints[1]->particle1 == ptr_P2) {
            if (list_joints[2]->particle1 == ptr_P1) {
                normal_vector = glm::normalize(glm::cross(ptr_P2->pos - ptr_P1->pos, list_joints[1]->particle2->pos - ptr_P2->pos));
            } else {
                normal_vector = glm::normalize(glm::cross(ptr_P2->pos - ptr_P1->pos, list_joints[2]->particle1->pos - ptr_P2->pos));
            }
        } else {
            if (list_joints[2]->particle1 == ptr_P1) {
                normal_vector = glm::normalize(glm::cross(ptr_P2->pos - ptr_P1->pos, list_joints[1]->particle1->pos - ptr_P2->pos));
            } else {
                normal_vector = glm::normalize(glm::cross(ptr_P2->pos - ptr_P1->pos, list_joints[2]->particle2->pos - ptr_P2->pos));
            }
        }
    }
};
