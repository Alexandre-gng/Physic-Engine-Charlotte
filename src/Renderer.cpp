#include "../include/Engine/Renderer.hpp"

// YYY Commentaires à reprendre


// Initialize the VAO for static objects
// --------------------------------------
void Renderer::init_static_VAO(const vector<Object*>& LIST_static_objects) {
    int size_static_vertices = 0;
    // size_dynamic_vertices => A mettre dans une liste de Render() qd plusieurs objets dedans!
    for (auto ptr_o: LIST_static_objects) {
        for (auto i: ptr_o->LIST_triangles) {
            size_static_vertices += 3;
        }
    }
    float static_vertices[size_static_vertices * 5];
    int index = 0;
    for (auto ptr_o: LIST_static_objects) {
        int last_index = index;
        for (auto ptr_T: ptr_o->LIST_triangles) {
            Particle* ptr_P1 = ptr_T->LIST_joints[0]->particle1;
            Particle* ptr_P2 = ptr_T->LIST_joints[0]->particle2;
            Particle* ptr_P3 = nullptr;
            if (ptr_T->LIST_joints[1]->particle1 != ptr_P1 && ptr_T->LIST_joints[1]->particle1 != ptr_P2) {
                ptr_P3 = ptr_T->LIST_joints[1]->particle1;
            } else {
                ptr_P3 = ptr_T->LIST_joints[1]->particle2;
            }
            // Particle 1
            static_vertices[index] = ptr_P1->pos.x;
            static_vertices[index+1] = ptr_P1->pos.y;
            static_vertices[index+2] = ptr_P1->pos.z;
            static_vertices[index+3] = float((index + 1)%2);
            static_vertices[index+4] = float(index%2);
            index += 5;
            // Particle 2
            static_vertices[index] = ptr_P2->pos.x;
            static_vertices[index+1] = ptr_P2->pos.y;
            static_vertices[index+2] = ptr_P2->pos.z;
            static_vertices[index+3] = float((index + 1)%2);
            static_vertices[index+4] = float(index%2);
            index += 5;
            // Particle 3
            static_vertices[index] = ptr_P3->pos.x;
            static_vertices[index+1] = ptr_P3->pos.y;
            static_vertices[index+2] = ptr_P3->pos.z;
            static_vertices[index+3] = float((index + 1)%2);
            static_vertices[index+4] = float(index%2);
            index += 5;
        }
        LIST_static_objects_start.push_back(last_index);
        LIST_static_objects_length.push_back(index - last_index);
    }
    cout << "static vertices created" << endl;

    // =========== VAO_static configuration ===========
    glGenVertexArrays(1, &VAO_static);
    glBindVertexArray(VAO_static);
    // VBOs init
    glGenBuffers(1, &VBO_static);
    // === Static VBO configuration ===
    glBindBuffer(GL_ARRAY_BUFFER, VBO_static);
    glBufferData(GL_ARRAY_BUFFER, sizeof(static_vertices), static_vertices, GL_STATIC_DRAW);
    // Attributes configuration
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    cout << "VAO_static created" << endl;
}



// Initialize the VAO for dynamic objects
// --------------------------------------
void Renderer::init_dynamic_VAO(const vector<Object*>& LIST_dynamic_objects) {
    int index = 0;
    int size_dynamic_vertices = 0;

    // size_static_vertices => A mettre dans une liste de Render() qd plusieurs objets dedans!
    for (auto ptr_o: LIST_dynamic_objects) {
        for (auto i: ptr_o->LIST_triangles) {
            size_dynamic_vertices+= 3;
        }
    }
    cout << "size_static_vertices = " << size_dynamic_vertices << endl;
    float dynamic_vertices[size_dynamic_vertices*5];

    for (auto ptr_o: LIST_dynamic_objects) {
        int last_index = index;
        for (auto ptr_T: ptr_o->LIST_triangles) {
            Particle *ptr_P1 = ptr_T->LIST_joints[0]->particle1;
            Particle *ptr_P2 = ptr_T->LIST_joints[0]->particle2;
            Particle *ptr_P3 = nullptr;
            if (ptr_T->LIST_joints[1]->particle1 != ptr_P1 && ptr_T->LIST_joints[1]->particle1 != ptr_P2) {
                ptr_P3 = ptr_T->LIST_joints[1]->particle1;
            } else {
                ptr_P3 = ptr_T->LIST_joints[1]->particle2;
            }
            // Particle 1
            dynamic_vertices[index] = ptr_P1->pos.x;
            dynamic_vertices[index + 1] = ptr_P1->pos.y;
            dynamic_vertices[index + 2] = ptr_P1->pos.z;
            dynamic_vertices[index + 3] = float((index + 1) % 2);
            dynamic_vertices[index + 4] = float(index % 2);
            index += 5;
            // Particle 2
            dynamic_vertices[index] = ptr_P2->pos.x;
            dynamic_vertices[index + 1] = ptr_P2->pos.y;
            dynamic_vertices[index + 2] = ptr_P2->pos.z;
            dynamic_vertices[index + 3] = float((index + 1) % 2);
            dynamic_vertices[index + 4] = float(index % 2);
            index += 5;
            // Particle 3
            dynamic_vertices[index] = ptr_P3->pos.x;
            dynamic_vertices[index + 1] = ptr_P3->pos.y;
            dynamic_vertices[index + 2] = ptr_P3->pos.z;
            dynamic_vertices[index + 3] = float((index + 1) % 2);
            dynamic_vertices[index + 4] = float(index % 2);
            index += 5;
        }

        LIST_dynamic_objects_start.push_back(last_index);
        LIST_dynamic_objects_length.push_back(index - last_index);
    }
    cout << "dynamic vertices created" << endl;

    // =========== VAO_static configuration ===========
    glGenVertexArrays(1, &VAO_dynamic);
    glBindVertexArray(VAO_dynamic);
    // VBOs init
    glGenBuffers(1, &VBO_dynamic);
    // === Static VBO configuration ===
    glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dynamic_vertices), dynamic_vertices, GL_DYNAMIC_DRAW);
    // Attributes configuration
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    cout << "VAO_dynamic created" << endl;
}


// Draw objects
void Renderer::render() const {
    // Draw dynamic objects
    glBindVertexArray(VAO_dynamic);
    for (int i = 0; i < LIST_dynamic_objects_length.size(); i++) {
        glDrawArrays(GL_TRIANGLES, LIST_dynamic_objects_start[i], LIST_dynamic_objects_length[i]);
    }

    // Draw static objects
    glBindVertexArray(VAO_static);
    for (int i = 0; i < LIST_static_objects_length.size(); i++) {
        glDrawArrays(GL_TRIANGLES, LIST_static_objects_start[i], LIST_static_objects_length[i]);
    }
}


