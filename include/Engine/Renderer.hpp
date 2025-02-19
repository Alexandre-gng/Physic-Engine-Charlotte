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
    GLuint          VAO_static;
    GLuint          VBO_static;
    vector<int>     LIST_static_objects_length;
    vector<int>     LIST_static_objects_start;

    GLuint          VAO_dynamic;
    GLuint          VBO_dynamic;
    float*          ptr_VBO_dynamic = nullptr;
    GLuint          VBO2_dynamic;
    vector<int>     LIST_dynamic_objects_length;
    vector<int>     LIST_dynamic_objects_start;

    Shader          shader;

    Renderer() = default;
    Renderer(const char* vertexShader, const char* fragmentShader) : shader(vertexShader, fragmentShader) {
    }
    ~Renderer() = default;

    // Initilisation of Engine (shaders, buffers, glfw window...)
    // YYY TO_DO
    void init_OPENGL() const;

    // ================= Handling Static Objects rendering =======================
    // Initialiaze VAOs by using the LIST_static_objects and LIST_dynamic_objects
    void init_static_VAO(vector<float> static_vertices, unsigned int size_static_vertices);
    vector<float> update_static_vertices(const vector<Object*>& LIST_static_objects);


    // ================= Handling Dynamic Objects rendering =======================
    void init_dynamic_VAO(vector<float> dynamic_vertices, unsigned int size_dynamic_vertices);
    vector<float> update_dynamic_vertices(const vector<Object*>& LIST_dynamic_objects);

    // Update the dynamic VBO with the new coords
    // TEST1: SubData
    void update_dynamic_VBO(const vector<float>& dynamic_vertices, int size_dynamic_vertices) const;
    // TEST2: glMapBufferRange
    void update_dynamic_VBO2(const vector<float>& dynamic_vertices, int size_dynamic_vertices) const;


    // Bind the VAOs and draw the objects
    void render() const;


    // Delete les buffers et VAOS
    // YYY NECESSARY ?
    void cleanup_ressources() {
    };
};