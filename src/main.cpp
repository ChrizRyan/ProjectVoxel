#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include <iostream>

#include "MeshRenderer.h"
#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "Plane.h"

#include "Grid.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

unsigned int createShaderFromFile(GLenum shaderType, const std::string& filepath);

void render(MeshRenderer *meshRenderer);

void initGL();

GLFWwindow* createGLFWWindow();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::unique_ptr<Camera> camera;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLuint textureID;

int main()
{
    GLFWwindow* window = createGLFWWindow();

    initGL();

    //unsigned int max_layers;
    //glGetIntegerv (GL_MAX_ARRAY_TEXTURE_LAYERS, max_layers);

    int temp; 
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &temp);
    std::cout << temp << std::endl;


    Grid* grid = new Grid(Vector3Int(100, 10, 100));
    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);





    glEnable(GL_TEXTURE_2D);

    // Generate a texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
 
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::vector<GLubyte> initialData(256 * 256 * 4, 0); // RGBA, all black
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, initialData.data());

    std::unique_ptr<Shader> shader = std::make_unique<Shader>("/shaders/vert/basic.vert", "/shaders/frag/voxel_renderer.frag");
    std::unique_ptr<Plane> plane = std::make_unique<Plane>();

    std::unique_ptr<Material> material = std::make_unique<Material>(std::move(shader));
    material->setBuffer(grid->getData());
    std::unique_ptr<MeshRenderer> meshRenderer = std::make_unique<MeshRenderer>(std::move(plane), std::move(material));

    meshRenderer->getMaterial()->getShader()->use();
    meshRenderer->getMaterial()->getShader()->updateUniform3f("u_dimensions", 1, 0.5, 0.2);  

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    meshRenderer->getMaterial()->getShader()->updateUniform3f("u_dimensions", grid->GetDimensions().x, grid->GetDimensions().y, grid->GetDimensions().z);  
    meshRenderer->getMaterial()->getShader()->updateUniform2f("u_resolution", SCR_WIDTH, SCR_HEIGHT);
    meshRenderer->getMaterial()->getShader()->updateUniformMatrix4fv("u_projectionMatrix", projection);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        float radius = 50.0f;
        glm::vec3 center(50.0f, 5.0f, 50.0f);
        float camX = static_cast<float>(sin(glfwGetTime()) * radius + center.x);
        float camZ = static_cast<float>(cos(glfwGetTime()) * radius + center.z);

        glm::mat4 view = glm::mat4(1.0f); 
        view = glm::lookAt(glm::vec3(camX, 90.0f, camZ), center, glm::vec3(0.0f, 1.0f, 0.0f));
        meshRenderer->getMaterial()->getShader()->updateUniformMatrix4fv("u_viewMatrix", view);

        // input
        // -----
        processInput(window);

        render(meshRenderer.get());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void initGL()
{
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}

GLFWwindow* createGLFWWindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Project Voxel", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void render(MeshRenderer *meshRenderer)
{
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
\
    // update shader uniform
    double  timeValue = glfwGetTime();
    

    

    meshRenderer->render();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// void savingLoadingVoxel()
// {
//     std::vector<std::vector<std::vector<int>>> voxels(10, std::vector<std::vector<int>>(10, std::vector<int>(10, 1)));
//     if (VoxelLoader::save("voxels.dat", voxels)) {
//         std::cout << "Saved successfully!\n";
//     }

//     std::vector<std::vector<std::vector<int>>> loadedVoxels;
//     if (VoxelLoader::load("voxels.dat", loadedVoxels)) {
//         std::cout << "Loaded successfully!\n";
//     }
// }