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
 * TO DO:
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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ========================= RENDERING THINGS ===========================
    Renderer renderer("../shaders/vertexShader.vs", "../shaders/fragmentShader.fs");
    cout << "Renderer created" << endl;

    // ========================= OBJECTS INITS ===========================
    vector<Object*> LIST_static_objects;
    vector<Object*> LIST_dynamic_objects;

    // Wall creation
    // ------------

    // Cube creation
    // ------------
    Cube* ptr_Cube = new Cube(0, 0, 0, 1, 1);
    cout << "Cube created" << endl;
    LIST_dynamic_objects.push_back(ptr_Cube);

    // Cloth creation
    // ------------
    Cloth* ptr_Cloth = new Cloth(0, -5, 0, 3, 3, 1, 1, 0.01);
    LIST_dynamic_objects.push_back(ptr_Cloth);

    // ========================= CAMERA THINGS ===========================
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    renderer.shader.setMat4("projection", projection);


    // ========================= RENDERING THINGS ==========================
    Renderer* ptr_renderer = new Renderer("../shaders/vertexShader.vs", "../shaders/fragmentShader.fs");
    ptr_renderer->init_dynamic_VAO(LIST_dynamic_objects);
    ptr_renderer->init_static_VAO(LIST_static_objects);


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

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