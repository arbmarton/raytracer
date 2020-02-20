#include "Camera.h"
#include "Utilities.h"
#include "UVSphere.h"
#include "Sphere.h"
#include "Shader.h"
#include "Ray.h"
#include "Renderer.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <array>

bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Camera::intance().moveForward();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Camera::intance().moveBackward();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Camera::intance().moveLeft();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Camera::intance().moveRight();
    }
}

void mouse_callback(GLFWwindow* window, const double xpos, const double ypos)
{
    if (firstMouse)
    {
        CursorPosDescriptor::instance().lastX = float(xpos);
        CursorPosDescriptor::instance().lastY = float(ypos);

        firstMouse = false;
    }

    const float xOffset = float(xpos) - CursorPosDescriptor::instance().lastX;
    const float yOffset = CursorPosDescriptor::instance().lastY - float(ypos);
    CursorPosDescriptor::instance().lastX = float(xpos);
    CursorPosDescriptor::instance().lastY = float(ypos);


    Camera::intance().processMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    constexpr float maxFov = 90.0f;
    constexpr float minFov = 1.0f;

    if (Camera::intance().getFov() >= minFov && Camera::intance().getFov() <= maxFov)
    {
        Camera::intance().setFov(Camera::intance().getFov() - float(yoffset));
    }
    if (Camera::intance().getFov() <= minFov)
    {
        Camera::intance().setFov(minFov);
    }
    if (Camera::intance().getFov() >= maxFov)
    {
        Camera::intance().setFov(maxFov);
    }
}


int main()
{
    // Initial cursor state
    CursorPosDescriptor::instance().lastX = ScreenDescriptor::WINDOW_WIDTH / 2;
    CursorPosDescriptor::instance().lastY = ScreenDescriptor::WINDOW_HEIGHT / 2;

    // Initialize GLFW and GLAD
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(ScreenDescriptor::WINDOW_WIDTH, ScreenDescriptor::WINDOW_HEIGHT, "Raytracer", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Render loop setup
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        // Update
        const auto currentTime = std::chrono::steady_clock::now();
        const float millisecs = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000.0f;
        lastTime = currentTime;
        std::cout << "FPS: " << 1 / (millisecs / 1000.0f) << "\n";


        Camera::intance().setSpeed((millisecs / 1000.0f) * 10.0f);
        processInput(window);

        //Renderer::intance().lights[0].position.z = cosf(float(glfwGetTime())) * 10;
        //Renderer::intance().spheres[0].pos.z = cosf(float(glfwGetTime())) * 10;

        Renderer::instance().render();

        // Upkeep
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}