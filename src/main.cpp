#include "../include/common.hpp"
#include "../include/OPENGL/Renderer.hpp"
#include "../include/Objects/Cloth.hpp"
#include "../include/Objects/Wall.hpp"
#include "../include/Physic.hpp"


using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/*
 * pour débuguer je dois:
 *      - afficher un carré tout le temps pour être sûr que l'affichage fonctionne
 *      - Ensuite véirifer si le wall est afffiché aussi
 *          => Si non, corriger
 *          => Si oui, comprendre
 *
 *  1) Créer un carré
 */
/*
 *
 * STARTING 08/11/2024
 *
 * TO DO:
 *      - Taille du Wall
 *      - class Renderer
 *      - class Object::Cube
 *      - delete Eigen everywhere (Damping_velocities(), Particle, ...?) OK
 *          -> A tester: supprimer le fichier Eigen
 *      - changer tous les gnagnagna_list en LIST_gnagnagn
 *      - delete_Joint() à faire in Object()
 *
 * PROBLEMES FUTURS:
 *     - Le Cloth est initialisé à l'envers, le repère n'étant pas le même entre SFML et OPENGL
 *
 * AMELIORATIONS:
 *     - Passer pas glDrawElements() dans render, et utiliser des EBO
 *     - Se questionner sur l'utilité de la classe Joint
 *     - Utiliser une hashmap pour la détection des triangles voisins
 *          => est ce mieux?
 *     - glEnable(GL_CULL_FACE)) pour ne pas afficher les triangles qui ne sont pas vus ?
 *
*/


// camera things
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int main() {
    cout << "STARTING PROGRAM" << endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LEARNOPENGL",NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Load all OpenGL pointers (apparently necessary)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    // Enable the Z buffer to have depth
    glEnable(GL_DEPTH_TEST);

    // ========================= RENDERING THINGS ===========================
    Renderer renderer("../shaders/vertexShader.vs", "../shaders/fragmentShader.fs");
    cout << "Renderer created" << endl;

    // ========================= OBJECTS INITS ===========================
    vector<Object*> LIST_immuable_object;
    vector<Object*> LIST_moving_object;

    // Wall creation
    // ------------
    Wall* ptr_Wall = new Wall(0, 0, -7, 1, 1);
    cout << "Wall created" << endl;

    // LIST_immuable_object.push_back(ptr_Wall);
    // renderer.create_VAO(LIST_immuable_object, LIST_moving_object);
    // cout << "Cloth added to renderer" << endl;

    // ========================= CAMERA THINGS ===========================
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    renderer.shader.setMat4("projection", projection);

    // ========================= TEMPORARY SETUP VERTICES ==========================
    /*
     * ======= renderer::VAO_creation() =======
     * 1. Transformer les coordonées des Objects en vertices (pos3, col2)
     * 2. Créer un VBO dynamique
     * 3. Créer un VBO statique
     * 4. Créer un VAO
     * 5. Paramétrer le VAO
     * ==============================
     *
     *
     * ======= Object::update_objects() =======
     * 1. Use PBD to update the objects
     * =============================
     *
     *
     * ======= renderer::update_VBO() =======
     * 1. Transformer les coordonées des Objects en vertices (pos3, col2)
     * 2. Mise à jour partielle (uniquement objects dynamiques) => glBufferSubData()
     * ============================
     *
     *
     * ======= renderer::render() =======
     * 1. Utiliser le shader
     * 2. projection matrix
     * 3. projection as uniform in Shader class
     * 4. view with GetViewMatrix with Camera class
     * 5. view as uniform in Shader class
     * 6. Bind le VAO
     * 7. model matrix as diagonal matrix
     * 8. model matrix is translate
     * 9. model but with hard constraints (rotation..)
     * 10. model as uniform in Shader class
     * 11. Bind le VBO Static
     * 12. DrawArrays VBO Static
     * 13. Bind le VBO Dynamic
     * 14. DrawArrays VBO Dynamic
     * =======================
     *
     * => Mettre le cube en static
     * => Mettre le wall en dynamic
     */

    // ===== static (Cube) =====
    // Things Cube (static)
    int size_cube_vertices = 0;
    for (auto i: cube_vertices) {
        size_cube_vertices++;
    }
    float static_Vertices[size_cube_vertices];
    for (int i = 0; i < size_cube_vertices; i++) {
        static_Vertices[i] = cube_vertices[i];
    }
    // Number of vertices
    unsigned int static_vertices_lenght = 36;


    // ===== dynamic (Wall) =====
    // Things Wall (dynamic)
    int size_wall_vertices = 0;
    for (auto i: ptr_Wall->LIST_triangles) {
        size_wall_vertices+= 3;
    }
    float dynamic_Vertices[size_wall_vertices*5];
    int index = 0;
    for (auto ptr_T: ptr_Wall->LIST_triangles) {
        Particle* ptr_P1 = ptr_T->LIST_joints[0]->particle1;
        Particle* ptr_P2 = ptr_T->LIST_joints[0]->particle2;
        Particle* ptr_P3 = nullptr;
        if (ptr_T->LIST_joints[1]->particle1 != ptr_P1 && ptr_T->LIST_joints[1]->particle1 != ptr_P2) {
            ptr_P3 = ptr_T->LIST_joints[1]->particle1;
        } else {
            ptr_P3 = ptr_T->LIST_joints[1]->particle2;
        }
        // Particle 1
        dynamic_Vertices[index] = ptr_P1->pos.x;
        dynamic_Vertices[index+1] = ptr_P1->pos.y;
        dynamic_Vertices[index+2] = ptr_P1->pos.z;
        dynamic_Vertices[index+3] = float((index + 1)%2);
        dynamic_Vertices[index+4] = float(index%2);
        index += 5;
        // Particle 2
        dynamic_Vertices[index] = ptr_P2->pos.x;
        dynamic_Vertices[index+1] = ptr_P2->pos.y;
        dynamic_Vertices[index+2] = ptr_P2->pos.z;
        dynamic_Vertices[index+3] = float((index + 1)%2);
        dynamic_Vertices[index+4] = float(index%2);
        index += 5;
        // Particle 3
        dynamic_Vertices[index] = ptr_P3->pos.x;
        dynamic_Vertices[index+1] = ptr_P3->pos.y;
        dynamic_Vertices[index+2] = ptr_P3->pos.z;
        dynamic_Vertices[index+3] = float((index + 1)%2);
        dynamic_Vertices[index+4] = float(index%2);
        index += 5;
    }
    unsigned int dynamic_vertices_lenght = size_wall_vertices;


    unsigned int VAO, VBO_static, VBO_dynamic;
    glGenBuffers(1, &VBO_static);
    glGenBuffers(1, &VBO_dynamic);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // ==== Static VBO ====
    glBindBuffer(GL_ARRAY_BUFFER, VBO_static);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    // VAO static configuration
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));

    // ==== Dynamic VBO ====
    glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dynamic_Vertices), dynamic_Vertices, GL_DYNAMIC_DRAW);
    // VAO dynamic configuration
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));

    // ========================== CUBE ==========================
    /*
    unsigned int VBO_cube, VAO_cube;
    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO_cube);

    glBindVertexArray(VAO_cube);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    */
    // ========================== WALL ==========================
    /*
    unsigned int VBO_wall, VAO_wall;
    glGenVertexArrays(1, &VAO_wall);
    glGenBuffers(1, &VBO_wall);

    glBindVertexArray(VAO_wall);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_wall);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    */

    renderer.shader.use();
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render things
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        renderer.shader.use();

        // pass projection matrix to shader
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        renderer.shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        renderer.shader.setMat4("view", view);


        // Rendu du cube
        glBindVertexArray(VAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        float angle = 0.f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        renderer.shader.setMat4("model", model);

        // Dessiner les objets statiques
        glBindBuffer(GL_ARRAY_BUFFER, VBO_static);
        glDrawArrays(GL_TRIANGLES, 0, static_vertices_lenght);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic);
        glDrawArrays(GL_TRIANGLES, 0, dynamic_vertices_lenght);

        // Rendu du mur
        /*
        glBindVertexArray(VAO_wall);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));  // Position différente
        renderer.shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);  // Supposons que le mur a 6 sommets
        */

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}