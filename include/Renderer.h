#pragma once

#include "Sphere.h"
#include "LightSource.h"
#include "Shader.h"
#include "Utilities.h"

#include "ThreadPool.h"

#include "glm.hpp"
#include "glad/glad.h"
#include "gtc/constants.hpp"

#include <thread>
#include <vector>

struct Ray;

class Renderer
{
public:
    static Renderer& instance()
    {
        static Renderer* inst = nullptr;
        if (!inst)
        {
            inst = new Renderer();
        }
        return *inst;
    }

    ~Renderer()
    {
        delete[] imgArray;

        glDeleteBuffers(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
    }

    glm::vec3 trace(const Ray& ray, const int recursionDepth);
    std::pair<Sphere*, float> intersect(const Ray& ray);

    GLuint renderToTexture();
    GLuint renderToTextureParallel();
    void drawTextureToScreen(const GLuint texture);
    void render();

    std::vector<Sphere> spheres;
    std::vector<LightSource> lights;

private:
    Renderer();

    constexpr static int maxRecursion = 8;
    constexpr static float ka = 0.5f;
    constexpr static float quadVertices[] = {  // positions + texcoords
        -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    const float fourPI = glm::pi<float>() * 4.0f;
    float planeDistance;

    glm::vec3 ambientLight{ 0.0f, 0.0f, 0.0f };

    GLuint quadVAO;
    GLuint quadVBO;

    Shader quadshader;

    unsigned char* imgArray = new unsigned char[ScreenDescriptor::WINDOW_WIDTH * ScreenDescriptor::WINDOW_HEIGHT * 3];

    ThreadPool pool{ std::thread::hardware_concurrency() };
};