#pragma once

#include <glm/glm.hpp>
#include "ClassParticle.hpp"
#include "ClassJoint.hpp"

class Object;
class Triangle;
class Joint;

using namespace std;

// YYY CHANGE sf::vector2f to Eigen::Vector2f


class Particle {
public:
    bool              moving       = true;
    float             mass         = 1.0f;
    float             inverse_mass = 1 /mass;
    float             friction;
    unsigned int      id;
    glm::vec3      pos;
    glm::vec3      prev_pos;
    glm::vec3      forces;
    glm::vec3      velocity;
    vector<Joint*>    list_joints;
    vector<Triangle*> list_triangles_friends;

    Object* ptr_Object;

    // CREATE A RENDER CLASS YYY
    // sf::CircleShape shape;


    void applyFriction();
    void applyGravity(float grav);
    void updateVelocity(float dt);

    void cutTwoParticles(Particle* Pa);

    Particle(float x, float y, float m) : pos(sf::Vector2f {x,y}), prev_pos(sf::Vector2f {x,y}), mass(m), inverse_mass(1/m) {
        // SFML Shape creation
        // YYY OPENGL TO FIX
        sf::CircleShape ParticleShape(2.f);
        ParticleShape.setFillColor(sf::Color(255, 255, 255));
        this->shape = ParticleShape;
    }
    Particle(float xprev, float yprev, float x, float y) {
        this->pos = {x, y};
        this->prev_pos = {x, y};
    }
};