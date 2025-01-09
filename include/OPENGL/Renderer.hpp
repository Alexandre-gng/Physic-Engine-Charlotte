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
    vector<unsigned int> VAOs;
    vector<unsigned int> VBOs_moving;
    unsigned int VBO_immuable;

    Shader shader;

    Renderer() = default;
    Renderer(const char* vertexShader, const char* fragmentShader) : shader(vertexShader, fragmentShader) {}

    ~Renderer() = default;

    // Adds a moving Object to the engine (new VBO)
    void add_moving_Object(Object* ptr_O) {
        unsigned int new_VAO;
        unsigned int new_VBO;
        glGenBuffers(1, &new_VBO);
        glGenVertexArrays(1, &new_VAO);

        glBindVertexArray(new_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, new_VBO);

        float vertices[sizeof(ptr_O->LIST_particles)*5];
        // Convert Particles positions to a clean verticies list
        int index = 0;
        for (auto vector: ptr_O->TAB_triangles) {
            for (auto ptr_T: vector) {
                if (ptr_T == nullptr) {
                    continue;
                }
                Particle* ptr_P1 = ptr_T->list_joints[0]->particle1;
                Particle* ptr_P2 = ptr_T->list_joints[0]->particle2;
                Particle* ptr_P3;
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
        /* YYY JE GARDE AU CAS OU
        for (int i=0; i < sizeof(ptr_O->LIST_particles); i++) {
            if (ptr_O->LIST_particles[i] == nullptr) {
                continue;
            }
            vertices[i] = ptr_O->LIST_particles[i]->pos.x;
            vertices[i+1] = ptr_O->LIST_particles[i]->pos.y;
            vertices[i+2] = ptr_O->LIST_particles[i]->pos.z;
            vertices[i+3] = float((i%2) + 1);
            vertices[i+4] = float(i%2);
        }
         */
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
    };

    // Adds an immuable Object to the engine (Reusing the same VBO for these)
    void add_immuable_Object(vector<Object*> LIST_objects) {

    };


    // Initilisation of OPENGL (shaders, buffers, glfw window...)
    void init();

    // Parcourt la liste des VAO, et les bind et affiche
    void render() {
        shader.use();  // Activer le shader
        for (unsigned int VAO : VAOs) {
            glBindVertexArray(VAO);
            // TROP RESTRICTIF: par exemple si c'est un cube, on Draw Arrays par 36, non par 6
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
    };


    // Delete les buffers et VAOS
    void cleanup();
};