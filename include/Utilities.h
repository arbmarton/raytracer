#pragma once

#include "glm.hpp"
#include <glad/glad.h>

#include <iostream>
#include <filesystem>
#include <optional>

class Camera;
struct Triangle;
struct Sphere;
struct Ray;

inline std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
{
    out << v.x << " " << v.y << " " << v.z << "\n";
    return out;
}

struct CursorPosDescriptor
{
    float lastX;
    float lastY;

    static CursorPosDescriptor& instance()
    {
        static CursorPosDescriptor* ptr{ nullptr };

        if (!ptr)
        {
            ptr = new CursorPosDescriptor();
        }

        return *ptr;
    }

private:
    CursorPosDescriptor() = default;
};

struct ScreenDescriptor
{
    constexpr static uint32_t WINDOW_WIDTH = 1024;
    constexpr static uint32_t WINDOW_HEIGHT = 768;

private:
    ScreenDescriptor() = delete;
    ScreenDescriptor(const ScreenDescriptor& rhs) = delete;
    ScreenDescriptor(const ScreenDescriptor&& rhs) = delete;

    ScreenDescriptor& operator=(const ScreenDescriptor& rhs) = delete;
    ScreenDescriptor& operator=(const ScreenDescriptor&& rhs) = delete;
};

namespace utilities {

std::filesystem::path getShaderFolderPath();
std::filesystem::path getShaderPath(const std::string& shaderName);

std::filesystem::path getTextureFolderPath();
std::filesystem::path getTexturePath(const std::string& textureName);

glm::vec3 calculateTriangleNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
glm::vec3 calculateTriangleNormal(const Triangle& tr);
float calculateIntersectionDistance(const glm::vec3& cameraPos, const glm::vec3& normal, const glm::vec3& dir, const glm::vec3& a);
bool isPointInsideTriangle(const glm::vec3& p, const Triangle& tr);
bool isPointInsideTriangle(const glm::vec3& p, const glm::vec3& normal, const Triangle& tr);

std::optional<float> calculateSphereIntersectionDistance(const Ray& ray, const Sphere& sphere);

glm::vec3 calculateReflectionDirection(const glm::vec3& ray, const glm::vec3& normal);
glm::vec3 calculateReflectionDirection(const Ray& ray, const glm::vec3& normal);
glm::vec3 calculateRefractionDirection(const glm::vec3& ray, const glm::vec3& normal, const float nu);
glm::vec3 calculateRefractionDirection(const Ray& ray, const glm::vec3& normal, const float nu);

glm::vec3 elementWiseMult(const glm::vec3& a, const glm::vec3& b);

glm::vec3 generateRandomVec3(const float min = -1.0f, const float max = 1.0f);
float generateRandomFloat(const float min = 0.0f, const float max = 1.0f);

}  // namespace utilities
