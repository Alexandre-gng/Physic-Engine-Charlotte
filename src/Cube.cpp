#include "../include/Objects/Cube.hpp"
#include "../include/Objects/Object.hpp"

/*            AB
 *         A ----(B1 or B2)
 *         |    / |
 *         |  /   |
 * (C1 or C2)---- D
 *
*/
vector<float> cube_vertices = {
        -1.f, -1.f, -1.f,// B1
        1.f, -1.f, -1.f, // D
        1.f,  1.f, -1.f, // C1
        1.f,  1.f, -1.f, // C2
        -1.f,  1.f, -1.f, // A
        -1.f, -1.f, -1.f, // B2

        -1.f, -1.f,  1.f,
        1.f, -1.f,  1.f,
        1.f,  1.f,  1.f,
        1.f,  1.f,  1.f,
        -1.f,  1.f,  1.f,
        -1.f, -1.f,  1.f,

        -1.f,  1.f,  1.f,
        -1.f,  1.f, -1.f,
        -1.f, -1.f, -1.f,
        -1.f, -1.f, -1.f,
        -1.f, -1.f,  1.f,
        -1.f,  1.f,  1.f,

        1.f,  1.f,  1.f,
        1.f,  1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, -1.f,  1.f,
        1.f,  1.f,  1.f,

        -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, -1.f,  1.f,
        1.f, -1.f,  1.f,
        -1.f, -1.f,  1.f,
        -1.f, -1.f, -1.f,

        -1.f,  1.f, -1.f,
        1.f,  1.f, -1.f,
        1.f,  1.f,  1.f,
        1.f,  1.f,  1.f,
        -1.f,  1.f,  1.f,
        -1.f,  1.f, -1.f,
};

// YYY Fix weight repartition
// YYY I don't use the int for colors in the list upthere
Cube::Cube(int x, int y, int z, float s, float m) : size(s), mass(m), Object(CUBE, 0, 0, 0, 0) {
    pos = glm::vec3(x, y, z);
    vector<float>& ptr_Cube_vertices = cube_vertices;
    for (auto &i: ptr_Cube_vertices) {
        i = i * size;
    }
    int index = 0;
    for (int i = 0; i < cube_vertices.size(); i+=18) {
        Particle* ptr_P_A = new Particle(cube_vertices[i+12], cube_vertices[i+13], cube_vertices[i+14], 1.0);
        Particle* ptr_P_B1 = new Particle(cube_vertices[i], cube_vertices[i+1], cube_vertices[i+2], 1.0);
        Particle* ptr_P_B2 = new Particle(cube_vertices[i+15], cube_vertices[i+16], cube_vertices[i+17], 1.0);
        Particle* ptr_P_C1 = new Particle(cube_vertices[i+6], cube_vertices[i+7], cube_vertices[i+8], 1.0);
        Particle* ptr_P_C2 = new Particle(cube_vertices[i+9], cube_vertices[i+10], cube_vertices[i+11], 1.0);
        Particle* ptr_P_D = new Particle(cube_vertices[i+3], cube_vertices[i+4], cube_vertices[i+5], 1.0);

        LIST_particles[i/18 * 6] = ptr_P_A;
        LIST_particles[(i/18) * 6 + 1] = ptr_P_B1;
        LIST_particles[(i/18) * 6 + 2] = ptr_P_B2;
        LIST_particles[(i/18) * 6 + 3] = ptr_P_C1;
        LIST_particles[(i/18) * 6 + 4] = ptr_P_C2;
        LIST_particles[(i/18) * 6 + 5] = ptr_P_D;

        Joint* ptr_J1 = new Joint(ptr_P_A, ptr_P_B1);
        Joint* ptr_J2 = new Joint(ptr_P_A, ptr_P_C1);
        Joint* ptr_J3 = new Joint(ptr_P_B1, ptr_P_C1);
        Triangle* ptr_T1 = new Triangle(ptr_J1, ptr_J2, ptr_J3);
        LIST_triangles[(i/18) * 2] = ptr_T1;

        Joint* ptr_J4 = new Joint(ptr_P_B2, ptr_P_D);
        Joint* ptr_J5 = new Joint(ptr_P_B2, ptr_P_C2);
        Joint* ptr_J6 = new Joint(ptr_P_C2, ptr_P_D);
        Triangle* ptr_T2 = new Triangle(ptr_J4, ptr_J5, ptr_J6);
        LIST_triangles[(i/18) * 2 + 1] = ptr_T2;
    }
}

