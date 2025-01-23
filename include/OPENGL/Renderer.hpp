#pragma once

#include "../common.hpp"
#include "Shader.hpp"
#include "../Objects/Object.hpp"

/*
 * SOUCIS:
 *      - add_moving_Object(): On peut pas juste caler les coordonnées comme ça il faut les aligner trois par
 *      trois comme pour des triangles, puis configuer le VAO en fonction (cf copilot)
 *          => itérer dans la liste des triangles, et pour chaque triangle, ajouter les 3 coordonnées d'affillées
 *          => EBO un jour ?
 *
 *      - render(): On peut pas juste faire un glDrawArrays, il faut faire un glDrawElements (ça c'est du pur
 *      copilot par contre)
 */
class Renderer {
public:
    // For Moving Objects (multpiple VAO)
    vector<unsigned int> LIST_MOVING_VAO;
    vector<int> LIST_lenght_Moving_VAO;

    // For Static Objects (one single VAO)
    unsigned int STATIC_VAO;
    vector<int> LIST_lenght_Static_VAO;
    vector<int> LIST_start_indice_Static_VAO;

    Shader shader;

    Renderer() = default;
    Renderer(const char* vertexShader, const char* fragmentShader) : shader(vertexShader, fragmentShader) {}

    ~Renderer() = default;

    // Adds a moving Object to the engine (Creating a new VBO/ VAO for each)
    void add_moving_Object(Object* ptr_O) {
        unsigned int new_VAO;
        unsigned int new_VBO;
        glGenBuffers(1, &new_VBO);
        glGenVertexArrays(1, &new_VAO);

        glBindVertexArray(new_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, new_VBO);

        // float vertices[ptr_O->TAB_triangles.size()*5*ptr_O->TAB_triangles[0].size()*3];
        float vertices[ptr_O->LIST_triangles.size()*5*3];

        int index = 0;
        // TO DEBUG YYY
        /*
        for (int i = 0; i < ptr_O->TAB_triangles.size(); i++) {
            for (int j = 0; j < ptr_O->TAB_triangles[i].size(); j++) {
        */
        for (auto ptr_T: ptr_O->LIST_triangles) {
                // if (ptr_O->TAB_triangles[i][j] != nullptr) {
            if (ptr_T != nullptr) {
                // Triangle* ptr_t = ptr_O->TAB_triangles[i][j];
                Particle* ptr_P1 = ptr_T->list_joints[0]->particle1;
                Particle* ptr_P2 = ptr_T->list_joints[0]->particle2;
                Particle* ptr_P3 = nullptr;
                if (ptr_T->list_joints[1]->particle1 != ptr_P1 && ptr_T->list_joints[1]->particle1 != ptr_P2) {
                    ptr_P3 = ptr_T->list_joints[1]->particle1;
                } else {
                    ptr_P3 = ptr_T->list_joints[1]->particle2;
                }

                // Particle 1
                vertices[index] = ptr_P1->pos.x;
                vertices[index+1] = ptr_P1->pos.y;
                vertices[index+2] = ptr_P1->pos.z;
                vertices[index+3] = float((index%2) + 1);
                vertices[index+4] = float(index%2);
                index += 5;
                // Particle 2
                vertices[index] = ptr_P2->pos.x;
                vertices[index+1] = ptr_P2->pos.y;
                vertices[index+2] = ptr_P2->pos.z;
                vertices[index+3] = float((index%2) + 1);
                vertices[index+4] = float(index%2);
                index += 5;
                // Particle 3
                vertices[index] = ptr_P3->pos.x;
                vertices[index+1] = ptr_P3->pos.y;
                vertices[index+2] = ptr_P3->pos.z;
                vertices[index+3] = float((index%2) + 1);
                vertices[index+4] = float(index%2);
                index += 5;
            }
        }
        LIST_lenght_Moving_VAO.push_back(index);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        LIST_MOVING_VAO.push_back(new_VAO);
        cout << "ONE MOVING OBJECT ADDED" << endl;
    };

    // Adds a list of immuable Object to the engine (Reusing the same VBO for these)
    void add_immuable_Objects(vector<Object*> LIST_objects) {
        unsigned int new_VAO;
        unsigned int new_VBO;
        glGenBuffers(1, &new_VBO);
        glGenVertexArrays(1, &new_VAO);
        glBindVertexArray(new_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, new_VBO);

        int total_size = 0;
        for (auto ptr_O: LIST_objects) {
            total_size += ptr_O->LIST_triangles.size()*5*3;
        }
        float vertices[total_size];

        int index = 0;
        for (auto ptr_O: LIST_objects) {
            int delta_index = index;
            for (auto ptr_T: ptr_O->LIST_triangles) {
                if (ptr_T != nullptr) {

                    // Triangle* ptr_t = ptr_O->TAB_triangles[i][j];
                    Particle* ptr_P1 = ptr_T->list_joints[0]->particle1;
                    Particle* ptr_P2 = ptr_T->list_joints[0]->particle2;
                    Particle* ptr_P3 = nullptr;
                    if (ptr_T->list_joints[1]->particle1 != ptr_P1 && ptr_T->list_joints[1]->particle1 != ptr_P2) {
                        ptr_P3 = ptr_T->list_joints[1]->particle1;
                    } else {
                        ptr_P3 = ptr_T->list_joints[1]->particle2;
                    }

                    // Particle 1
                    vertices[index] = ptr_P1->pos.x;
                    vertices[index+1] = ptr_P1->pos.y;
                    vertices[index+2] = ptr_P1->pos.z;
                    vertices[index+3] = float((index%2) + 1);
                    vertices[index+4] = float(index%2);
                    index += 5;
                    // Particle 2
                    vertices[index] = ptr_P2->pos.x;
                    vertices[index+1] = ptr_P2->pos.y;
                    vertices[index+2] = ptr_P2->pos.z;
                    vertices[index+3] = float((index%2) + 1);
                    vertices[index+4] = float(index%2);
                    index += 5;
                    // Particle 3
                    vertices[index] = ptr_P3->pos.x;
                    vertices[index+1] = ptr_P3->pos.y;
                    vertices[index+2] = ptr_P3->pos.z;
                    vertices[index+3] = float((index%2) + 1);
                    vertices[index+4] = float(index%2);
                    index += 5;
                }
            }
            LIST_start_indice_Static_VAO.push_back(index);
            LIST_lenght_Static_VAO.push_back(index - delta_index);
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        STATIC_VAO = new_VAO;
        cout << "IMMUABLE OBJECTS ADDED IN A SINGLE VAO" << endl;
    };


    // Initilisation of OPENGL (shaders, buffers, glfw window...)
    void init() {

    }

    // Parcourt la liste des VAO, et les bind et affiche
    void render() {
        shader.use();

        // Rendering immuable objects
        glBindVertexArray(STATIC_VAO);
        for (int i = 0; i < LIST_lenght_Static_VAO.size(); i++) {
            // TROP RESTRICTIF: par exemple si c'est un cube, on Draw Arrays par 36, non par 6
            glDrawArrays(GL_TRIANGLES, LIST_start_indice_Static_VAO[i], LIST_lenght_Static_VAO[i] /5);
            cout << "Static Object #" << i << " rendered" << endl;
        }

        // Rendering moving object
        for (int i = 0; i < LIST_MOVING_VAO.size(); i++) {
            glBindVertexArray(LIST_MOVING_VAO[i]);
            // TROP RESTRICTIF: par exemple si c'est un cube, on Draw Arrays par 36, non par 6
            glDrawArrays(GL_TRIANGLES, 0, LIST_lenght_Moving_VAO[i] /5);
            cout << "Moving Object #" << i << " rendered" << endl;
        }

        glBindVertexArray(0);
    };


    // Delete les buffers et VAOS
    void cleanup();
};