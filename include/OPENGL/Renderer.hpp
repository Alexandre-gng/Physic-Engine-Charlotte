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
    unsigned int VAO;
    vector<unsigned int> LIST_MOVING_VAO;
    vector<int> LIST_lenght_VAO;
    vector<int> LIST_start_indice_VAO;

    Shader shader;

    Renderer() = default;
    Renderer(const char* vertexShader, const char* fragmentShader) : shader(vertexShader, fragmentShader) {}

    ~Renderer() = default;

    /*
     * ETAPES:
     * ==== Create_VAO() part ====
     *  crée le VBO dynamic
     *  crée le VBO static
     *
     *  crée le VAO
     *  paramètre le VAO
     *  ==== Create_VAO() part ====
     *
     *
     *  ==== while loop part =====
     *  use shader
     *  projection matrix
     *  projection as uniform in Shader class
     *
     *  view with GetViewMatrix with Camera class
     *  view as uniform in Shader class
     */

    /*  crée le VBO dynamic
    *   crée le VBO static
    *
    *   crée le VAO
    *   paramètre le VAO
    */
    // Create one VBO for each moving object, and one VAO for all immuable objects
    // Create a unique VAO for these buffers

    /*
     *
     * Le bufferData supprime les précédents
     */
    void create_VAO(vector<Object*> LIST_dynamic_objects, vector<Object*> LIST_static_objects) {
        unsigned int new_VAO;
        unsigned int new_VBO;
        glGenBuffers(1, &new_VBO);
        glGenVertexArrays(1, &new_VAO);
        glBindVertexArray(new_VAO);

        // ================= Static objects part =================
        int total_size = 0;
        for (auto ptr_O: LIST_static_objects) {
            total_size += ptr_O->LIST_triangles.size()*5*3;
        }
        float vertices_static_objects[total_size];
        int index = 0;
        for (auto ptr_O: LIST_static_objects) {
            int delta_index = index;
            for (auto ptr_T: ptr_O->LIST_triangles) {
                if (ptr_T != nullptr) {
                    Particle* ptr_P1 = ptr_T->LIST_joints[0]->particle1;
                    Particle* ptr_P2 = ptr_T->LIST_joints[0]->particle2;
                    Particle* ptr_P3 = nullptr;
                    if (ptr_T->LIST_joints[1]->particle1 != ptr_P1 && ptr_T->LIST_joints[1]->particle1 != ptr_P2) {
                        ptr_P3 = ptr_T->LIST_joints[1]->particle1;
                    } else {
                        ptr_P3 = ptr_T->LIST_joints[1]->particle2;
                    }

                    // Particle 1
                    vertices_static_objects[index] = ptr_P1->pos.x;
                    vertices_static_objects[index+1] = ptr_P1->pos.y;
                    vertices_static_objects[index+2] = ptr_P1->pos.z;
                    vertices_static_objects[index+3] = float((index%2) + 1);
                    vertices_static_objects[index+4] = float(index%2);
                    index += 5;
                    // Particle 2
                    vertices_static_objects[index] = ptr_P2->pos.x;
                    vertices_static_objects[index+1] = ptr_P2->pos.y;
                    vertices_static_objects[index+2] = ptr_P2->pos.z;
                    vertices_static_objects[index+3] = float((index%2) + 1);
                    vertices_static_objects[index+4] = float(index%2);
                    index += 5;
                    // Particle 3
                    vertices_static_objects[index] = ptr_P3->pos.x;
                    vertices_static_objects[index+1] = ptr_P3->pos.y;
                    vertices_static_objects[index+2] = ptr_P3->pos.z;
                    vertices_static_objects[index+3] = float((index%2) + 1);
                    vertices_static_objects[index+4] = float(index%2);
                    index += 5;
                }
            }
            LIST_start_indice_VAO.push_back(index);
            LIST_lenght_VAO.push_back(index - delta_index);
        }
        glBindBuffer(GL_ARRAY_BUFFER, new_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_static_objects), vertices_static_objects, GL_STATIC_DRAW);
        cout << "Static VBO created" << endl;

        // ================= Dynamic objects part =================
        for (auto ptr_O: LIST_dynamic_objects) {
            index = 0;
            new_VBO = 0;
            total_size = ptr_O->LIST_triangles.size()*5*3;

            float vertices_dynamic_object[total_size];

            int delta_index = index;
            for (auto ptr_T: ptr_O->LIST_triangles) {
                if (ptr_T != nullptr) {
                    Particle* ptr_P1 = ptr_T->LIST_joints[0]->particle1;
                    Particle* ptr_P2 = ptr_T->LIST_joints[0]->particle2;
                    Particle* ptr_P3 = nullptr;
                    if (ptr_T->LIST_joints[1]->particle1 != ptr_P1 && ptr_T->LIST_joints[1]->particle1 != ptr_P2) {
                        ptr_P3 = ptr_T->LIST_joints[1]->particle1;
                    } else {
                        ptr_P3 = ptr_T->LIST_joints[1]->particle2;
                    }

                    // Particle 1
                    vertices_dynamic_object[index] = ptr_P1->pos.x;
                    vertices_dynamic_object[index+1] = ptr_P1->pos.y;
                    vertices_dynamic_object[index+2] = ptr_P1->pos.z;
                    vertices_dynamic_object[index+3] = float((index%2) + 1);
                    vertices_dynamic_object[index+4] = float(index%2);
                    index += 5;
                    // Particle 2
                    vertices_dynamic_object[index] = ptr_P2->pos.x;
                    vertices_dynamic_object[index+1] = ptr_P2->pos.y;
                    vertices_dynamic_object[index+2] = ptr_P2->pos.z;
                    vertices_dynamic_object[index+3] = float((index%2) + 1);
                    vertices_dynamic_object[index+4] = float(index%2);
                    index += 5;
                    // Particle 3
                    vertices_dynamic_object[index] = ptr_P3->pos.x;
                    vertices_dynamic_object[index+1] = ptr_P3->pos.y;
                    vertices_dynamic_object[index+2] = ptr_P3->pos.z;
                    vertices_dynamic_object[index+3] = float((index%2) + 1);
                    vertices_dynamic_object[index+4] = float(index%2);
                    index += 5;
                }
            }
            LIST_start_indice_VAO.push_back(index);
            LIST_lenght_VAO.push_back(index - delta_index);

            glBindBuffer(GL_ARRAY_BUFFER, new_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_dynamic_object), vertices_dynamic_object, GL_DYNAMIC_DRAW);
            cout << "Dynamic VBO created" << endl;
        }

        // VAO creation
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        VAO = new_VAO;
        cout << "== VAO created ==" << endl;
    };


    // Initilisation of OPENGL (shaders, buffers, glfw window...)
    // YYY TO_DO
    void init() {
    }

    // Draw objects
    void render() {
        glBindVertexArray(VAO);
        for (int i = 0; i < LIST_lenght_VAO.size(); i++) {
            glDrawArrays(GL_TRIANGLES, LIST_start_indice_VAO[i], LIST_lenght_VAO[i] /5);
            cout << "Object #" << i << " rendered" << endl;
        }
        glBindVertexArray(0);
    }
    /* YYY TO DELETE ?
    void render() {
        // Rendering immuable objects
        glBindVertexArray(VAO);
        for (int i = 0; i < LIST_lenght_VAO.size(); i++) {
            // TROP RESTRICTIF: par exemple si c'est un cube, on Draw Arrays par 36, non par 6
            glDrawArrays(GL_TRIANGLES, LIST_start_indice_VAO[i], LIST_lenght_VAO[i] /5);
            cout << "Static Object #" << i << " rendered" << endl;
        }

        // Rendering moving object
        for (int i = 0; i < LIST_MOVING_VAO.size(); i++) {
            glBindVertexArray(LIST_MOVING_VAO[i]);
            // TROP RESTRICTIF: par exemple si c'est un cube, on Draw Arrays par 36, non par 6
            glDrawArrays(GL_TRIANGLES, 0, LIST_lenght_VAO[i] /5);
            cout << "Moving Object #" << i << " rendered" << endl;
        }

        glBindVertexArray(0);
    };
     */


    // Delete les buffers et VAOS
    // YYY TO_DO
    void cleanup();
};