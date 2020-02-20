#pragma once

#include "Triangle.h"

#include "glm.hpp"
#include <glad/glad.h>

#include <vector>

class Shader;

class UVSphere
{
public:
    UVSphere(const float radiusArg, const glm::vec3& positionArg, const uint32_t latitudeDivision, const uint32_t longitudeDivision);
    ~UVSphere() = default;

    void draw() const;

    glm::vec3 getPosition() const
    {
        return position;
    }

    float getRadius() const
    {
        return radius;
    }

    const std::vector<Triangle>& getTriangles() const
    {
        return triangles;
    }

private:
    const float radius;
    glm::vec3 position;

    std::vector<float> vertexData;
    std::vector<uint32_t> indices;
    std::vector<Triangle> triangles;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};