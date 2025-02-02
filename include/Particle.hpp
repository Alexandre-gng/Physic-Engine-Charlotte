#pragma once

#include <glm/glm.hpp>
#include "Particle.hpp"
#include "Joint.hpp"

class Object;
class Triangle;
class Joint;

using namespace std;


class Particle {
public:
    bool              moving       = true;
    float             mass         = 1.0f;
    float             inverse_mass = 1 /mass;
    float             friction;
    unsigned int      id;
    glm::vec3         pos;
    glm::vec3         prev_pos;
    glm::vec3         forces;
    glm::vec3         velocity;
    vector<Joint*>    LIST_joints;
    vector<Triangle*> LIST_triangles_friends;

    Object* ptr_Object;

    // CREATE A RENDER CLASS YYY
    // sf::CircleShape shape;


    void applyFriction();
    void applyGravity(float grav);
    void updateVelocity(float dt);

    void cutTwoParticles(Particle* Pa);

    // Constructor dedicated for a static object - mass = 0kg;
    Particle(float x, float y, float z) : pos(glm::vec3 {x,y,z}), prev_pos(glm::vec3 {x,y,z}), mass(0), inverse_mass(0) {}

    Particle(float x, float y, float z, float m) : pos(glm::vec3 {x,y,z}), prev_pos(glm::vec3 {x,y,z}), mass(m), inverse_mass(1/m) {
        // SFML Shape creation
        // YYY OPENGL TO FIX
        /*
        sf::CircleShape ParticleShape(2.f);
        ParticleShape.setFillColor(sf::Color(255, 255, 255));
        this->shape = ParticleShape;
        */
    }

    Particle(float xprev, float yprev, float zprev, float x, float y, float z, float m) : pos(glm::vec3 {x,y,z}), prev_pos(glm::vec3 {xprev,yprev,zprev}), mass(m), inverse_mass(1/m) {
    }
};