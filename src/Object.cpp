#include "../include/Objects/ClassObject.hpp"


// YYY : Faire que la fonction retourne un " Particle* ", et que l'appel de la suppression se fasse dans le main
void Object::detect_Particle(sf::Vector2f mousePos) {
    for (const auto i : this->LIST_particles) {
        Particle* ptr_P = i;
        if (ptr_P != nullptr) {
            if ((mousePos.x + 5.0 > ptr_P->pos.x) && (mousePos.x - 5.0 < ptr_P->pos.x)) {
                if ((mousePos.y + 5.0 > ptr_P->pos.y) && (mousePos.y - 5.0 < ptr_P->pos.y)) {
                    if (this->object_type == CLOTH) {
                        this->supp_Particle(ptr_P);
                    }
                    return;
                }
            }
        }
    }
}

void Object::deleteJoint(Joint* ptr_J) {
    return;
}