#include "Shader.h"
#include "TextureRenderer.h"
#include "Camera2D.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // For macOS?
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Texture Renderer", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* /*window*/, int width, int height)
    {
        glViewport(0, 0, width, height);
    });
    
    glewExperimental = GL_TRUE; // Ensure modern OpenGL functions are available
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    Camera2D camera(0.0f, 200.0f, 0.0f, 200.0f);

    Shader shader;
    shader.attachFromFile("shaders/vertex_shader.glsl", GL_VERTEX_SHADER)
          .attachFromFile("shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER)
          .link();

    TextureRenderer textureRenderer(shader);

    std::vector<Entity> entities = {
        { {50.0f, 50.0f}, {25.0f, 25.0f}, 0.0f, "textures/icon_dark.png" },
        { {150.0f, 100.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {155.0f, 110.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {160.0f, 120.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {165.0f, 130.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {170.0f, 140.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {175.0f, 150.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {180.0f, 160.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {185.0f, 170.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {190.0f, 180.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" },
        { {195.0f, 190.0f}, {40.0f, 40.0f}, 45.0f, "textures/icon_bright.png" }
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // Move camera
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.move(glm::vec2(0.0f, 1.0f));
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.move(glm::vec2(0.0f, -1.0f));
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.move(glm::vec2(-1.0f, 0.0f));
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.move(glm::vec2(1.0f, 0.0f));

        // Zoom in and out
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camera.setZoom(1.01f);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camera.setZoom(0.99f);

        // Rotate around center
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.rotate(-1.0f);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.rotate(1.0f);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glm::mat4 vpMatrix = camera.getVPMatrix();
        shader.setMat4("projView", vpMatrix);

        float rotationDegreesPerSecond = 10.0f;
        float rotationAngle = static_cast<float>(glfwGetTime() * rotationDegreesPerSecond);

        for (auto& entity : entities)
        {   
            entity.rotation = rotationAngle;
            textureRenderer.drawEntity(entity);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}