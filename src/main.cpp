#include "../include/common.hpp"
#include "../include/Engine/Renderer.hpp"
#include "../include/Engine/Camera.hpp"

#include "../include/Objects/Cloth.hpp"
#include "../include/Objects/Wall.hpp"
#include "../include/Objects/Cube.hpp"

#include "../include/Engine/Physic.hpp"



using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/*
 * STARTING 08/11/2024
 *
 * BUG:
 *      - Mauvaise réecriture dans StretchingConstraint::apply
 *           => Origine: Classe Phyics qui modifie mal les positions, on se retrouve avec plusieurs joints en double ou des positions bizarres
 *           ou même des joints vers lui-même
 *           => Ils ont tous la même prev_Pos
 *
 *      - Particle->pos.y prend une valeur random, parfois 10, parfois 9.6777 parfois 8.YYY, parfois 11.YY ?
 *
 * TO DO:
 *      - Ajouter la rotation pour Wall et Cube
 *      - Revoir complément le système de classe, les sous classes de Object ont un ptr vers un Object ???
 *      - règler le soucis de resize de la fenêtre /!\
 *      - delete Eigen everywhere (Damping_velocities(), Particle, ...?) OK
 *          -> A tester: supprimer le fichier Eigen
 *      - delete_Joint() à faire in Object()
 *
 * PROBLEMES FUTURS:
 *     - Le Cloth est initialisé à l'envers, le repère n'étant pas le même entre SFML et Engine
 *
 * AMELIORATIONS:
 *      - Faire un schéma pour le Cloth
 *     - Passer pas glDrawElements() dans render, et utiliser des EBO
 *     - Se questionner sur l'utilité de la classe Joint
 *     - Utiliser une hashmap pour la détection des triangles voisins
 *          => est ce mieux?
 *     - glEnable(GL_CULL_FACE)) pour ne pas afficher les triangles qui ne sont pas vus ?
 *
 *     - Si ajout de fragment shader => Utiliser UBO pour les matrices de transformation
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



int main() {
    cout << "STARTING PROGRAM" << endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LEARNOPENGL",NULL, NULL);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Enable the Z buffer to have depth
    glEnable(GL_DEPTH_TEST);
    // Show only the lines of the objects
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    printf("%s\n", glGetString(GL_VERSION));
    // ========================= RENDERING THINGS ===========================
    Renderer renderer("../shaders/vertexShader.vs", "../shaders/fragmentShader.fs");
    cout << "Renderer created" << endl;

    // ========================= OBJECTS INITS ===========================
    vector<Object*> LIST_static_objects;
    vector<Object*> LIST_dynamic_objects;

    // Wall creation
    // ------------
    /*
    Wall* ptr_Wall = new Wall(0, 0, 0, 10, 10);
    LIST_static_objects.push_back(ptr_Wall);
    cout << "Static Wall created" << endl;
    */

    // Cube creation
    // ------------
    /*
    Cube* ptr_Cube = new Cube(0, 0, 0, 1, 1);
    ptr_Cube->moving = false;
    LIST_static_objects.push_back(ptr_Cube);
    cout << "Static Cube created" << endl;
    */

    // Cloth creation
    // ------------
    Cloth* ptr_Cloth = new Cloth(-3, -2, -4, 6, 6, 1, 1, 0.01);
    cout << "Cloth created" << endl;
    ptr_Cloth->moving = true;
    LIST_dynamic_objects.push_back(ptr_Cloth);
    cout << "Cloth added to LIST_dynamic_objects" << endl;

    // ========================= CAMERA THINGS ===========================
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    renderer.shader.setMat4("projection", projection);

    // ========================= RENDERING THINGS ==========================
    Renderer* ptr_renderer = new Renderer("../shaders/vertexShader.vs", "../shaders/fragmentShader.fs");
    vector<float> dynamic_vertices = ptr_renderer->update_dynamic_vertices(LIST_dynamic_objects);
    ptr_renderer->init_dynamic_VAO(dynamic_vertices, dynamic_vertices.size() * 5 * sizeof(float));
    cout << "Dynamic VAO initialized" << endl;

    vector<float> static_vertices = ptr_renderer->update_static_vertices(LIST_static_objects);
    ptr_renderer->init_static_VAO(static_vertices, static_vertices.size() * 5 * sizeof(float));
    cout << "Static VAO initialized" << endl;


    // ========================= PHYSICS THINGS ==========================
    Physic* ptr_Physic = new Physic(LIST_dynamic_objects,10);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Physics maj
        ptr_Physic->PBD(deltaTime, 0, 10);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ptr_renderer->shader.use();

        // pass projection matrix to shader
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ptr_renderer->shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ptr_renderer->shader.setMat4("view", view);

        // prepare object transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        float angle = 0.f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        ptr_renderer->shader.setMat4("model", model);

        /*
        dynamic_vertices = ptr_renderer->update_dynamic_vertices(LIST_dynamic_objects);
        ptr_renderer->init_dynamic_VAO(dynamic_vertices, dynamic_vertices.size() * 5 * sizeof(float));
        */

        // ptr_renderer->update_dynamic_VBO(dynamic_vertices, dynamic_vertices.size() * 5 * sizeof(float));
        // ptr_renderer->update_dynamic_VBO2(LIST_dynamic_objects);

        ptr_renderer->update_dynamic_VBO2(dynamic_vertices, dynamic_vertices.size() * sizeof(float));

        ptr_renderer->render();

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