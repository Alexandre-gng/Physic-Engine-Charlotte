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
    GLuint VAO_static;
    GLuint VBO_static;
    vector<int> LIST_static_objects_length;
    vector<int> LIST_static_objects_start;

    GLuint VAO_dynamic;
    GLuint VBO_dynamic;
    vector<int> LIST_dynamic_objects_length;
    vector<int> LIST_dynamic_objects_start;

    Shader shader;

    Renderer() = default;
    Renderer(const char* vertexShader, const char* fragmentShader) : shader(vertexShader, fragmentShader) {
    }

    ~Renderer() = default;

    // Initilisation of Engine (shaders, buffers, glfw window...)
    // YYY TO_DO
    void init_OPENGL() {
    }

    void init_static_VAO(const vector<Object*>& LIST_static_objects);
    void init_dynamic_VAO(const vector<Object*>& LIST_dynamic_objects);

    // YYY TO DELETE
    void init_VAO_VBOs(const vector<Object*>& LIST_dynamic_objects, const vector<Object*>& LIST_static_objects);

    void render() const;

    // Delete les buffers et VAOS
    // YYY NECESSARY ?
    void cleanup_ressources() {
    };
};