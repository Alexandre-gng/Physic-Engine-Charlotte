#include "../include/Objects/Cloth.hpp"
#include "../include/Particle.hpp"
#include <unordered_map>
#include <algorithm>

// Helper function to create a unique key for a particle pair (order-independent)
static inline std::pair<Particle*, Particle*> makeParticlePairKey(Particle* p1, Particle* p2) {
    if (p1 < p2) {
        return {p1, p2};
    }
    return {p2, p1};
}

// Custom hash for particle pair - using improved hash combination
struct ParticlePairHash {
    std::size_t operator()(const std::pair<Particle*, Particle*>& p) const {
        // Use improved hash combination to reduce collisions
        auto h1 = std::hash<Particle*>{}(p.first);
        auto h2 = std::hash<Particle*>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

Cloth::Cloth(int x, int y, int z, int w, int h, float d, float m_p, float friction)
        : default_lenght(d), mass_particles(m_p), width(w), height(h), Object(CLOTH, h, w, h-1, (w-1)*2) {

    // Adding the Stretching constraint
    LIST_constraints.push_back(std::make_shared<StretchingConstraint>(default_lenght, this));

    // Pre-compute sqrt(3)/2 for particle positioning
    const float sqrt3_over_2 = std::sqrt(3.0f) / 2.0f;

    // Create all the Particles
    int ID = 0;
    for (int i = 0; i < h; i++) {
        const float y_offset = -(y + i * (default_lenght * sqrt3_over_2));
        const bool is_even_row = (i % 2 == 0);
        
        for (int j = 0; j < w; j++) {
            ID++;
            float x_pos;
            
            if (is_even_row) {
                x_pos = (j == w-1) ? x + j * default_lenght - default_lenght/2 : x + j * default_lenght;
            } else {
                x_pos = (j == 0) ? x + j * default_lenght : x + j * default_lenght - default_lenght/2;
            }
            
            Particle* ptr_NewP = new Particle(x_pos, y_offset, static_cast<float>(z), m_p);
            LIST_particles[i*w + j] = ptr_NewP;
            ptr_NewP->friction = friction;
            ptr_NewP->id = ID;
            ptr_NewP->prev_pos = ptr_NewP->pos;
            ptr_NewP->moving = (i != 0);
            ptr_NewP->velocity = glm::vec3(0.f, 0.f, 0.f);
        }
    }

    // Use hashmap for O(1) joint lookup instead of O(n) linear search
    std::unordered_map<std::pair<Particle*, Particle*>, Joint*, ParticlePairHash> jointMap;

    // Create Joints and store them in the hashmap
    for (int i = 0; i < h-1; i++) {
        const bool is_even_row = (i % 2 == 0);
        for (int j = 0; j < w-1; j++) {
            auto createAndStoreJoint = [&jointMap](Particle* p1, Particle* p2) -> Joint* {
                auto key = makeParticlePairKey(p1, p2);
                auto it = jointMap.find(key);
                if (it != jointMap.end()) {
                    return it->second;
                }
                Joint* newJoint = new Joint(p1, p2);
                jointMap[key] = newJoint;
                return newJoint;
            };

            if (is_even_row) {
                createAndStoreJoint(LIST_particles[i*w + j], LIST_particles[(i+1)*w + j]);
                createAndStoreJoint(LIST_particles[i*w + j], LIST_particles[i*w + j+1]);
                createAndStoreJoint(LIST_particles[i*w + j], LIST_particles[(i+1)*w + j+1]);
            } else {
                createAndStoreJoint(LIST_particles[i*w + j], LIST_particles[(i+1)*w + j]);
                createAndStoreJoint(LIST_particles[(i+1)*w + j], LIST_particles[i*w + j+1]);
                createAndStoreJoint(LIST_particles[i*w + j], LIST_particles[i*w + j+1]);
            }
            if (i == h-2) {
                createAndStoreJoint(LIST_particles[(i+1)*w + j], LIST_particles[(i+1)*w + j+1]);
            }
            if (j == w-2) {
                createAndStoreJoint(LIST_particles[i * w + j + 1], LIST_particles[(i + 1) * w + j + 1]);
            }
        }
    }

    // Helper function for O(1) joint lookup
    auto getJoint = [&jointMap](Particle* p1, Particle* p2) -> Joint* {
        auto it = jointMap.find(makeParticlePairKey(p1, p2));
        return (it != jointMap.end()) ? it->second : nullptr;
    };

    // Create Triangles using O(1) hashmap lookup instead of O(n) list search
    int index_tab = 0;
    for (int i = 0; i < h-1; i++) {
        const bool is_even_row = (i % 2 == 0);
        for (int j = 0; j < w-1; j++) {
            auto A = LIST_particles[i*w + j];
            auto B = LIST_particles[i*w + j+1];
            auto C = LIST_particles[(i+1)*w + j];
            auto D = LIST_particles[(i+1)*w + j+1];

            // O(1) lookups instead of O(n) linear searches
            Joint* AB = getJoint(A, B);
            Joint* AC = getJoint(A, C);
            Joint* BD = getJoint(B, D);
            Joint* CD = getJoint(C, D);

            if (is_even_row) {
                Joint* AD = getJoint(A, D);
                
                if (AD == nullptr) { cout << "ERROR: AD is NULL" << endl; }
                if (AB == nullptr) { cout << "ERROR: AB is NULL" << endl; }
                if (AC == nullptr) { cout << "ERROR: AC is NULL" << endl; }
                if (BD == nullptr) { cout << "ERROR: BD is NULL" << endl; }
                if (CD == nullptr) { cout << "ERROR: CD is NULL" << endl; }
                
                Triangle* ACD = new Triangle(AC, AD, CD);
                Triangle* ABD = new Triangle(AB, AD, BD);

                this->LIST_triangles[index_tab] = ACD;
                this->LIST_triangles[index_tab+1] = ABD;

                ACD->Triangle_id = index_tab;
                ABD->Triangle_id = index_tab + 1;
            } else {
                Joint* BC = getJoint(B, C);
                
                if (BC == nullptr) { cout << "ERROR: BC is NULL" << endl; }
                if (AB == nullptr) { cout << "ERROR: AB is NULL" << endl; }
                if (AC == nullptr) { cout << "ERROR: AC is NULL" << endl; }
                if (BD == nullptr) { cout << "ERROR: BD is NULL" << endl; }
                if (CD == nullptr) { cout << "ERROR: CD is NULL" << endl; }
                
                Triangle* ABC = new Triangle(AB, BC, AC);
                Triangle* BCD = new Triangle(BD, CD, BC);
                
                ABC->Triangle_id = index_tab;
                BCD->Triangle_id = index_tab + 1;

                this->LIST_triangles[index_tab] = ABC;
                this->LIST_triangles[index_tab+1] = BCD;
            }
            index_tab += 2;
        }
    }

    // Assign each Triangle its neighbours
    for (int i = 0; i < h-1; i++) {
        for (int j = 0; j < (w-1)*2-1; j++) {
            Triangle* ptr_T = LIST_triangles[i*w + j];
            if (j != 0) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[i*w + j-1]);
            }
            if (j != w*2-1) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[i*w + j+1]);
            }
            if (j%4==0 && j != w*2-1 && i != h-1) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i+1)*w + j+1]);
            }
            if (j%4==1 && j != 0 && i != 0) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i-1)*w + j-1]);
            }
            if (j%4==2 && j != w*2-1 && i != 0) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i-1)*w + j+1]);
            }
            if (j%4==3 && j != 0 && i != h-1) {
                ptr_T->LIST_nearest_triangles.push_back(LIST_triangles[(i+1)*w + j-1]);
            }
        }
    }
}


// Delete a Particle in the Cloth, and all the Triangle and Joint that contain it
// Optimized: using std::find and erase-remove idiom instead of manual iteration
void Cloth::supp_Particle(Particle* ptr_P) {
    cout << "start supp particle" << endl;
    
    // Use unordered_set for O(1) lookup of triangles to remove
    std::unordered_set<Triangle*> triangles_to_remove(
        ptr_P->LIST_triangles_friends.begin(), 
        ptr_P->LIST_triangles_friends.end()
    );

    // Remove triangles from this cloth's list using erase-remove idiom
    LIST_triangles.erase(
        std::remove_if(LIST_triangles.begin(), LIST_triangles.end(),
            [&triangles_to_remove](Triangle* t) {
                return t != nullptr && triangles_to_remove.count(t) > 0;
            }),
        LIST_triangles.end()
    );

    // Remove this particle from the cloth's particle list
    auto particle_it = std::find(LIST_particles.begin(), LIST_particles.end(), ptr_P);
    if (particle_it != LIST_particles.end()) {
        LIST_particles.erase(particle_it);
    }

    // Remove references from neighbour triangles
    for (Triangle* ptr_T : ptr_P->LIST_triangles_friends) {
        if (ptr_T == nullptr) continue;
        for (Triangle* ptr_T_neighbour : ptr_T->LIST_nearest_triangles) {
            if (ptr_T_neighbour == nullptr) continue;
            auto& neighbour_list = ptr_T_neighbour->LIST_nearest_triangles;
            neighbour_list.erase(
                std::remove(neighbour_list.begin(), neighbour_list.end(), ptr_T),
                neighbour_list.end()
            );
        }
    }

    // Remove dead joints from neighbour particles
    for (Joint* ptr_J : ptr_P->LIST_joints) {
        if (ptr_J == nullptr) continue;
        Particle* other_particle = (ptr_J->particle1 == ptr_P) ? ptr_J->particle2 : ptr_J->particle1;
        auto& other_joints = other_particle->LIST_joints;
        other_joints.erase(
            std::remove(other_joints.begin(), other_joints.end(), ptr_J),
            other_joints.end()
        );
    }

    // Remove triangle references from neighbour particles
    for (Triangle* ptr_T : ptr_P->LIST_triangles_friends) {
        if (ptr_T == nullptr) continue;
        for (Joint* ptr_J : ptr_T->LIST_joints) {
            if (ptr_J->particle1 != ptr_P && ptr_J->particle2 != ptr_P) {
                for (Particle* neighbour : {ptr_J->particle1, ptr_J->particle2}) {
                    if (neighbour != nullptr) {
                        auto& tri_friends = neighbour->LIST_triangles_friends;
                        tri_friends.erase(
                            std::remove(tri_friends.begin(), tri_friends.end(), ptr_T),
                            tri_friends.end()
                        );
                    }
                }
            }
        }
    }

    // Delete triangles that contain this particle
    for (Triangle* ptr_T : ptr_P->LIST_triangles_friends) {
        if (ptr_T != nullptr) {
            delete ptr_T;
        }
    }

    // Delete joints that contain this particle
    for (Joint* ptr_J : ptr_P->LIST_joints) {
        if (ptr_J != nullptr) {
            delete ptr_J;
        }
    }

    delete ptr_P;
}
