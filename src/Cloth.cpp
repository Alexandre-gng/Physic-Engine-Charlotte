#include "../include/Objects/Cloth.hpp"
#include "../../OPENGL/include/Particle.hpp"


Cloth::Cloth(int x, int y, int z, int w, int h, float d,float m_p, float friction)
        : default_lenght(d), mass_particles(m_p), width(w), height(h), Object(CLOTH, h, w, h-1, (w-1)*2) {

    // Adding the Stretching constraint
    LIST_constraints.push_back(std::make_shared<StretchingConstraint>(default_lenght, this));


    // YYY NECESSARY ?
    glm::vec3 last_pos = {float(x), float(y), 0.f};

    // Create all the Particle in the
    int ID = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            ID ++;
            Particle *ptr_NewP;
            if (i % 2 == 0) {
                if (j == w-1) {
                    ptr_NewP = new Particle(x + j * default_lenght - default_lenght/2, -(y + i * (default_lenght * sqrt(3) / 2)), z, m_p);
                    last_pos = ptr_NewP->pos;
                } else {
                    ptr_NewP = new Particle(x + j * default_lenght, -(y + i * (default_lenght * sqrt(3) / 2)), z, m_p);
                    last_pos = ptr_NewP->pos;
                }
            } else {
                if (j == 0) {
                    ptr_NewP = new Particle(x + j * default_lenght, -(y + i * (default_lenght * sqrt(3) / 2)), z, m_p);
                } else {
                    ptr_NewP = new Particle(x + j * default_lenght - default_lenght/2, -(y + i * (default_lenght * sqrt(3) / 2)), z,m_p);
                }
                last_pos = ptr_NewP->pos;
            }
            // MEMORY LEAK HERE YYY
            LIST_particles[i*w + j] = ptr_NewP;
            ptr_NewP->friction = friction;
            ptr_NewP->id = ID;

            if (i == 0) {
                ptr_NewP->prev_pos = ptr_NewP->pos;
                ptr_NewP->moving = false;
            } else {
                ptr_NewP->prev_pos = ptr_NewP->pos; // - glm::vec3{0.5, 0.5, 0.5};
                ptr_NewP->moving = true;
            }
            ptr_NewP->velocity = glm::vec3(0.f, 0.f, 0.f);
        }
    }
    LIST_particles;
    // Create Joint in Cloth YYY VALUE NOT USED
    for (int i = 0; i < h-1; i++) {
        for (int j = 0; j < w-1; j++) {
            Particle *ptr_P = LIST_particles[i*w + j];
            if (i % 2 == 0) {
                new Joint(LIST_particles[i*w + j], LIST_particles[(i+1)*w + j]);
                new Joint(LIST_particles[i*w + j], LIST_particles[i*w + j+1]);
                new Joint(LIST_particles[i*w + j], LIST_particles[(i+1)*w + j+1]);
            } else {
                new Joint(LIST_particles[i*w + j], LIST_particles[(i+1)*w + j]);
                new Joint(LIST_particles[(i+1)*w + j], LIST_particles[i*w + j+1]);
                new Joint(LIST_particles[i*w + j], LIST_particles[i*w + j+1]);
            }
            if (i == h-2) {
                new Joint(LIST_particles[(i+1)*w + j], LIST_particles[(i+1)*w + j+1]);
            }
            if (j == w-2) {
                new Joint(LIST_particles[i * w + j + 1], LIST_particles[(i + 1) * w + j + 1]);
            }
        }
    }
    // Create Triangle with Joint in Cloth

    int index_tab = 0;
    int h_number_triangle=0;
    int w_number_triangle=0;
    for (int i = 0; i < h-1; i++) {
        for (int j = 0; j < w-1; j++) {
            // YYY Would be better to rename all the LIST_particles[gnagnagna] into A, B, C , D
            auto A = LIST_particles[i*w + j];
            auto B = LIST_particles[i*w + j+1];
            auto C = LIST_particles[(i+1)*w + j];
            auto D = LIST_particles[(i+1)*w + j+1];
            Joint *AB = nullptr, *BD = nullptr, *AC = nullptr, *CD = nullptr, *AD = nullptr, *BC = nullptr;
            /*        AB
            *      A ---- B
            *  AC  | \AD  |  BD
            *      |   \  |
            *      C ---- D
            *         CD
            */
            if (i % 2 == 0) {
                for (auto joint: A->LIST_joints) {
                    if ((joint->particle1 == C || joint->particle2 == C)
                        && (joint->particle1 == A || joint->particle2 == A)) {
                        AC = joint;
                    }
                    if ((joint->particle1 == D || joint->particle2 == D)
                        && (joint->particle1 == A || joint->particle2 == A)) {
                        AD = joint;
                    }
                    if ((joint->particle1 == B || joint->particle2 == B)
                        && (joint->particle1 == A || joint->particle2 == A)) {
                        AB = joint;
                    }
                }
                for (auto joint: D->LIST_joints) {
                    if ((joint->particle1 == C || joint->particle2 == C)
                        && (joint->particle1 == D || joint->particle2 == D)) {
                        CD = joint;
                    }
                    if ((joint->particle1 == B || joint->particle2 == B)
                        && (joint->particle1 == D || joint->particle2 == D)) {
                        BD = joint;
                    }
                }
                // YYY FOR DEBUG
                if (AD == nullptr) {
                    cout << "ERROR: AD is NULL" << endl;
                } if (AB == nullptr) {
                    cout << "ERROR: AB is NULL" << endl;
                } if (AC == nullptr) {
                    cout << "ERROR: AC is NULL" << endl;
                } if (BD == nullptr) {
                    cout << "ERROR: BD is NULL" << endl;
                } if (CD == nullptr) {
                    cout << "ERROR: CD is NULL" << endl;
                }
                Triangle* ACD = new Triangle(AC, AD, CD);
                Triangle* ABD = new Triangle(AB, AD, BD);

                this->LIST_triangles[index_tab] = ACD;
                this->LIST_triangles[index_tab+1] = ABD;

                ACD->Triangle_id = index_tab;
                ABD->Triangle_id = index_tab + 1;

                index_tab += 2;
                w_number_triangle += 2;
                h_number_triangle ++;
            }
            /*        AB
            *      A ---- B
            *  AC  |  BC/ |  BD
            *      |  /   |
            *      C ---- D
            *         CD
            */
            else {
                for (auto joint: C->LIST_joints) {
                    if ((joint->particle1 == C || joint->particle2 == C)
                        && (joint->particle1 == A || joint->particle2 == A)) {
                        AC = joint;
                    }
                    if ((joint->particle1 == C || joint->particle2 == C)
                        && (joint->particle1 == D || joint->particle2 == D)) {
                        CD = joint;
                    }
                    if ((joint->particle1 == C|| joint->particle2 == C)
                        && (joint->particle1 == B || joint->particle2 == B)) {
                        BC = joint;
                    }
                }
                for (auto joint: B->LIST_joints) {
                    if ((joint->particle1 == B || joint->particle2 == B)
                        && (joint->particle1 == D || joint->particle2 == D)) {
                        BD = joint;
                    }
                    if ((joint->particle1 == B || joint->particle2 == B)
                        && (joint->particle1 == A || joint->particle2 == A)) {
                        AB = joint;
                    }
                }
                // YYY FOR DEBUG
                if (BC == nullptr) {
                    cout << "ERROR: BC is NULL" << endl;
                } if (AB == nullptr) {
                    cout << "ERROR: AB is NULL" << endl;
                } if (AC == nullptr) {
                    cout << "ERROR: AC is NULL" << endl;
                } if (BD == nullptr) {
                    cout << "ERROR: BD is NULL" << endl;
                } if (CD == nullptr) {
                    cout << "ERROR: CD is NULL" << endl;
                }
                Triangle* ABC = new Triangle(AB, BC, AC);
                Triangle* BCD = new Triangle(BD, CD, BC);
                ABC->Triangle_id = index_tab;
                BCD->Triangle_id = index_tab;

                this->LIST_triangles[index_tab] = ABC;
                this->LIST_triangles[index_tab+1] = BCD;

                index_tab += 2;
                w_number_triangle += 2;
                h_number_triangle ++;
            }
        }
    }
    // Assign each Triangle its neighbours
    for (int i = 0; i < h-1; i++) {
        for (int j = 0; j < (w-1)*2-1; j++) {
            Triangle *ptr_T = LIST_triangles[i*w + j];
            if (j != 0) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[i*w + j-1]);
            } if (j != w*2-1) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[i*w + j+1]);
            }
            if (j%4==0 && j != w*2-1 && i != h-1) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i+1)*w + j+1]);
            }if (j%4==1 && j != 0 && i != 0) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i-1)*w + j-1]);
            }if (j%4==2 && j != w*2-1 && i != 0) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i-1)*w + j+1]);
            }if (j%4==3 && j != 0 && i != h-1) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i+1)*w + j-1]);
            }
        }
    }
};


// Delete a Particle in the Cloth, and all the Triangle and Joint that contain it
void Cloth::supp_Particle(Particle* ptr_P) {
    cout << "start supp particl"<< endl;
    // Delete all the ptr_T concerned in the Cloth->TABtriangles
    /* YYY
    for (auto& row_T: this->TAB_triangles) {
        for (auto& ptr_T: row_T) {
    */
    for (auto ptr_T: this->LIST_triangles) {
        if (ptr_T != nullptr) {
            for (auto it = ptr_P->LIST_triangles_friends.begin(); it != ptr_P->LIST_triangles_friends.end(); it++) {
                if (*it == ptr_T) {
                    ptr_P->LIST_triangles_friends.erase(it);
                    break;
                }
            }
        }
    }

    // Delete the ptr_P in the Cloth->LIST_particles
    for (auto it = this->LIST_particles.begin(); it != this->LIST_particles.end(); it++) {
        if (*it == ptr_P) {
            this->LIST_particles.erase(it);
            break;
        }
    }

    // Delete the reference of the neighbour triangles of ptr_P
    for (Triangle *ptr_T: ptr_P->LIST_triangles_friends) {
        for (Triangle *ptr_T_neighbour: ptr_T->LIST_nearest_triangles) {
            if (ptr_T_neighbour != nullptr) {
                int cpt = 0;
                // loop ? YYY
                for (int i = 0; i < ptr_T_neighbour->LIST_nearest_triangles.size(); i++) {
                    cpt ++;
                }
                for (auto it = ptr_T_neighbour->LIST_nearest_triangles.begin(); it != ptr_T_neighbour->LIST_nearest_triangles.end(); it++) {
                    if (*it == ptr_T) {
                        *it = nullptr;
                        ptr_T_neighbour->LIST_nearest_triangles.erase(it);
                        break;
                    }
                }
            }
        }
    }

    // Delete the dead joints of all ptr_P's neighbours particles
    for (auto ptr_J: ptr_P->LIST_joints) {
        if (ptr_J->particle1 == ptr_P) {
            for (auto it = ptr_J->particle2->LIST_joints.begin(); it != ptr_J->particle2->LIST_joints.end(); it++) {
                if (*it == ptr_J) {
                    *it = nullptr;
                    ptr_J->particle2->LIST_joints.erase(it);
                    break;
                }
            }
        } else {
            for (auto it = ptr_J->particle1->LIST_joints.begin(); it != ptr_J->particle1->LIST_joints.end(); it++) {
                if (*it == ptr_J) {
                    *it = nullptr;
                    ptr_J->particle1->LIST_joints.erase(it);
                    break;
                }
            }
        }
    }

    // Delete all dead Triangles of the ptr_P's neighbours Particle
    for (auto ptr_T: ptr_P->LIST_triangles_friends) {
        for (auto ptr_J: ptr_T->LIST_joints) {
            if (ptr_J->particle1 != ptr_P && ptr_J->particle2 != ptr_P) {
                if (ptr_J->particle1 != nullptr) {
                    for (auto it = ptr_J->particle1->LIST_triangles_friends.begin(); it != ptr_J->particle1->LIST_triangles_friends.end(); it++) {
                        if (*it == ptr_T) {
                            *it = nullptr;
                            ptr_J->particle1->LIST_triangles_friends.erase(it);
                            break;
                        }
                    }
                }
                if (ptr_J->particle2 != nullptr) {
                    for (auto it = ptr_J->particle2->LIST_triangles_friends.begin(); it != ptr_J->particle2->LIST_triangles_friends.end(); it++) {
                        if (*it == ptr_T) {
                            *it = nullptr;
                            ptr_J->particle2->LIST_triangles_friends.erase(it);
                            break;
                        }
                    }
                }
            }
        }
    }

    // Delete every Triangle that contain ptr_P
    for (auto ptr_T: ptr_P->LIST_triangles_friends) {
        if (ptr_T == nullptr) { continue;}
        delete ptr_T;
    }

    // Delete every joints that contain ptr_P
    for (auto ptr_J: ptr_P->LIST_joints) {
        if (ptr_J == nullptr) { continue;}
        delete ptr_J;
    }

    delete ptr_P;
}
