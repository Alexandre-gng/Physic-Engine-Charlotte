#include "../include/Engine/Renderer.hpp"

// YYY Commentaires à reprendre


// Initialize the VAO for static objects
// --------------------------------------
vector<float> Renderer::update_static_vertices(const vector<Object*>& LIST_static_objects) {
    int size_static_vertices = 0;
    // size_dynamic_vertices => A mettre dans une liste de Render() qd plusieurs objets dedans!
    for (auto ptr_o: LIST_static_objects) {
        for (auto i: ptr_o->LIST_triangles) {
            size_static_vertices += 3;
        }
    }
    // Size = [size_static_vertices * 5]
    vector<float> static_vertices;

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
            static_vertices.push_back(ptr_P1->pos.x);
            static_vertices.push_back(ptr_P1->pos.y);
            static_vertices.push_back(ptr_P1->pos.z);
            static_vertices.push_back(float((index + 1)%2));
            static_vertices.push_back(float(index%2));
            index += 5;
            // Particle 2
            static_vertices.push_back(ptr_P2->pos.x);
            static_vertices.push_back(ptr_P2->pos.y);
            static_vertices.push_back(ptr_P2->pos.z);
            static_vertices.push_back(float((index + 1)%2));
            static_vertices.push_back(float(index%2));
            index += 5;
            // Particle 3
            static_vertices.push_back(ptr_P3->pos.x);
            static_vertices.push_back(ptr_P3->pos.y);
            static_vertices.push_back(ptr_P3->pos.z);
            static_vertices.push_back(float((index + 1)%2));
            static_vertices.push_back(float(index%2));
            index += 5;
        }
        LIST_static_objects_start.push_back(last_index);
        LIST_static_objects_length.push_back(index - last_index);
    }

    return static_vertices;
}



void Renderer::init_static_VAO(vector<float> static_vertices, unsigned int size_static_vertices) {
    // =========== VAO_static configuration ===========
    glGenVertexArrays(1, &VAO_static);
    glBindVertexArray(VAO_static);
    // VBOs init
    glGenBuffers(1, &VBO_static);
    // === Static VBO configuration ===
    glBindBuffer(GL_ARRAY_BUFFER, VBO_static);
    glBufferData(GL_ARRAY_BUFFER, static_vertices.size()*sizeof(float), static_vertices.data(), GL_STATIC_DRAW);
    // Attributes configuration
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



// Initialize the VAO for dynamic objects
// --------------------------------------
vector<float> Renderer::update_dynamic_vertices(const vector<Object*>& LIST_dynamic_objects) {
    int index = 0;
    int size_dynamic_vertices = 0;

    // size_static_vertices => A mettre dans une liste de Render() qd plusieurs objets dedans!
    for (auto ptr_o: LIST_dynamic_objects) {
        for (auto i: ptr_o->LIST_triangles) {
            size_dynamic_vertices+= 3;
        }
    }

    // Size = size_dynamic_vertices*5
    vector<float> dynamic_vertices;

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
            dynamic_vertices.push_back(ptr_P1->pos.x);
            dynamic_vertices.push_back(ptr_P1->pos.y);
            dynamic_vertices.push_back(ptr_P1->pos.z);
            dynamic_vertices.push_back(float((index + 1) % 2));
            dynamic_vertices.push_back(float(index % 2));
            index += 5;
            // Particle 2
            dynamic_vertices.push_back(ptr_P2->pos.x);
            dynamic_vertices.push_back(ptr_P2->pos.y);
            dynamic_vertices.push_back(ptr_P2->pos.z);
            dynamic_vertices.push_back(float((index + 1) % 2));
            dynamic_vertices.push_back(float(index % 2));
            index += 5;
            // Particle 3
            dynamic_vertices.push_back(ptr_P3->pos.x);
            dynamic_vertices.push_back(ptr_P3->pos.y);
            dynamic_vertices.push_back(ptr_P3->pos.z);
            dynamic_vertices.push_back(float((index + 1) % 2));
            dynamic_vertices.push_back(float(index % 2));
            index += 5;
        }

        LIST_dynamic_objects_start.push_back(last_index);
        LIST_dynamic_objects_length.push_back(index - last_index);
    }

    return dynamic_vertices;
}


// Dynamic VAO initialization (VBOs, attributes)
void Renderer::init_dynamic_VAO(vector<float> dynamic_vertices, unsigned int size_dynamic_vertices) {
    glGenVertexArrays(1, &VAO_dynamic);
    glBindVertexArray(VAO_dynamic);
    // VBOs init
    glGenBuffers(1, &VBO_dynamic);

    // ========== dynamic VBO configuration ==========
    glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic);
    // Give the memory to the buffer + configuration to be persistent
    glBufferStorage(GL_ARRAY_BUFFER, dynamic_vertices.size()*sizeof(float), NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    // Return a pointer to the future VBO emplacement in RAM
    ptr_VBO_dynamic = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, dynamic_vertices.size()*sizeof(float),GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    // Transfer the data from the CPU (RAM) to the GPU
    if (ptr_VBO_dynamic) {
        memcpy(ptr_VBO_dynamic, dynamic_vertices.data(), dynamic_vertices.size() * sizeof(float));
    } else {
        cout << "Error: glMapBufferRange failed" << endl;
    }

    // ========== dynamic VAO configuration ==========

    // Attributes configuration
    glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    cout << "VAO_dynamic created" << endl;
}


// Update the dynamic VBO with the new coords
void Renderer::update_dynamic_VBO(const vector<float>& dynamic_vertices, int size_dynamic_vertices) const {
    glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic);
    glBufferData(GL_ARRAY_BUFFER, size_dynamic_vertices, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dynamic_vertices.size()*sizeof(float), dynamic_vertices.data());

}


void Renderer::update_dynamic_VBO2(const vector<float>& new_dynamic_vertices, int size_dynamic_vertices) const {
    if (ptr_VBO_dynamic) {
        memcpy(ptr_VBO_dynamic, new_dynamic_vertices.data(), new_dynamic_vertices.size()*sizeof(float));
    } else {
        cout << "Error: VBO pointer null" << endl;
    }
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


