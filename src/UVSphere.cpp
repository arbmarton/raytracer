#include "UVSphere.h"
#include "Utilities.h"
#include "Shader.h"

#include "gtc/constants.hpp"

UVSphere::UVSphere(const float radiusArg, const glm::vec3& positionArg, const uint32_t latitudeDivision, const uint32_t longitudeDivision)
    : radius(radiusArg)
    , position(positionArg)
{
    //http://www.songho.ca/opengl/gl_sphere.html
    const float radiusInverse = 1.0f / radius;

    for (uint32_t y = 0; y <= latitudeDivision; ++y)
    {
        for (uint32_t x = 0; x <= longitudeDivision; ++x)
        {
            const float u = float(x) / float(longitudeDivision);
            const float v = float(y) / float(latitudeDivision);

            const float theta = 2 * glm::pi<float>() * u;
            const float phi = glm::pi<float>() * (v - 0.5f);

            const float sin_theta = sinf(theta);
            const float cos_theta = cosf(theta);
            const float sin_phi = sinf(phi);
            const float cos_phi = cosf(phi);

            const float ux = cos_theta * cos_phi * radius;
            const float uy = sin_phi * radius;
            const float uz = cos_phi * sin_theta * radius;

            vertexData.push_back(ux);
            vertexData.push_back(uy);
            vertexData.push_back(uz);
            vertexData.push_back(ux * radiusInverse);
            vertexData.push_back(uy * radiusInverse);
            vertexData.push_back(uz * radiusInverse);
            vertexData.push_back(-u);
            vertexData.push_back(v);
        }
    }

    uint32_t curr{ 0 };
    indices.resize(latitudeDivision * longitudeDivision * size_t(6));
    for (uint32_t lon = 0; lon < longitudeDivision; ++lon)
    {
        for (uint32_t lat = 0; lat < latitudeDivision; ++lat)
        {
            indices[curr++] = (lat + 0) * (longitudeDivision + 1) + lon;
            indices[curr++] = (lat + 1) * (longitudeDivision + 1) + lon;
            indices[curr++] = (lat + 0) * (longitudeDivision + 1) + lon + 1;

            indices[curr++] = (lat + 1) * (longitudeDivision + 1) + lon;
            indices[curr++] = (lat + 1) * (longitudeDivision + 1) + lon + 1;
            indices[curr++] = (lat + 0) * (longitudeDivision + 1) + lon + 1;
        }
    }

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        const glm::vec3 a = { vertexData[indices[i] * 8ul], vertexData[indices[i] * 8ul + 1], vertexData[indices[i] * 8ul + 2] };
        const glm::vec3 b = { vertexData[indices[i + 1] * 8ul], vertexData[indices[i + 1] * 8ul + 1], vertexData[indices[i + 1] * 8ul + 2] };
        const glm::vec3 c = { vertexData[indices[i + 2] * 8ul], vertexData[indices[i + 2] * 8ul + 1], vertexData[indices[i + 2] * 8ul + 2] };

        triangles.push_back({ a, b, c });
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void UVSphere::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, GLsizei(vertexData.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}