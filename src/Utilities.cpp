#include "Utilities.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Ray.h"
#include "Plane.h"

#include <random>

namespace utilities {
std::filesystem::path getShaderFolderPath()
{
    std::string root = std::filesystem::current_path().parent_path().string();
    root.append("/shaders/");
    return std::filesystem::path(root);
}

std::filesystem::path getShaderPath(const std::string& shaderName)
{
    return std::filesystem::path(getShaderFolderPath().string() + shaderName);
}

std::filesystem::path getTextureFolderPath()
{
    std::string root = std::filesystem::current_path().parent_path().string();
    root.append("/textures/");
    return std::filesystem::path(root);
}

std::filesystem::path getTexturePath(const std::string& shaderName)
{
    return std::filesystem::path(getTextureFolderPath().string() + shaderName);
}

glm::vec3 calculateTriangleNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
    return glm::cross(b - a, c - a);
}

glm::vec3 calculateTriangleNormal(const Triangle& tr)
{
    return glm::cross(tr.b - tr.a, tr.c - tr.a);
}

float calculateIntersectionDistance(const glm::vec3& cameraPos, const glm::vec3& normal, const glm::vec3& dir, const glm::vec3& a)
{
    return (glm::dot(normal, cameraPos - a)) / (glm::dot(normal, dir));
}

bool isPointInsideTriangle(const glm::vec3& p, const Triangle& tr)
{
    const glm::vec3 normal = calculateTriangleNormal(tr.a, tr.b, tr.c);

    return glm::dot(glm::cross(tr.b - tr.a, p - tr.a), normal) >= 0 && glm::dot(glm::cross(tr.c - tr.b, p - tr.b), normal) >= 0
           && glm::dot(glm::cross(tr.a - tr.c, p - tr.c), normal) >= 0;
}

bool isPointInsideTriangle(const glm::vec3& p, const glm::vec3& normal, const Triangle& tr)
{
    return glm::dot(glm::cross(tr.b - tr.a, p - tr.a), normal) >= 0 && glm::dot(glm::cross(tr.c - tr.b, p - tr.b), normal) >= 0
           && glm::dot(glm::cross(tr.a - tr.c, p - tr.c), normal) >= 0;
}

std::optional<float> calculateSphereIntersectionDistance(const Ray& ray, const Sphere& sphere)
{
    const glm::vec3 temp = ray.origin - sphere.pos;

    const float a = glm::dot(ray.direction, ray.direction);
    const float b = 2.0f * glm::dot(ray.direction, temp);
    const float c = glm::dot(temp, temp) - sphere.radius * sphere.radius;

    const float divisor = 2 * a;

    const float D = b * b - divisor * 2 * c;
    if (D < 0.0f)
    {
        return {};
    }

    const float sqD = sqrt(D);

    const float x1 = (-b + sqD) / divisor;
    const float x2 = (-b - sqD) / divisor;

    if (x1 < 0 && x2 < 0)
    {
        return {};
    }
    else if (x1 < 0)
    {
        return x2;
    }
    else if (x2 < 0)
    {
        return x1;
    }
    else
    {
        return std::min(x1, x2);
    }
}

std::optional<float> calculatePlaneIntersectionDistance(const Ray& ray, const Plane& plane)
{
    const float dist = glm::dot(plane.normal, plane.offset - ray.origin) / glm::dot(plane.normal, ray.direction);
    if (dist < 0)
    {
        return {};
    }
    else
    {
        return { dist };
    }
}

glm::vec3 calculateReflectionDirection(const glm::vec3& ray, const glm::vec3& normal)
{
    const float cosAlpha = glm::dot(-ray, normal) / (glm::length(ray) * glm::length(normal));
    return ray + 2.0f * normal * cosAlpha;
}

glm::vec3 calculateReflectionDirection(const Ray& ray, const glm::vec3& normal)
{
    return calculateReflectionDirection(ray.direction, normal);
}

glm::vec3 calculateRefractionDirection(const glm::vec3& ray, const glm::vec3& normal, const float nu)
{
    const float cosAlpha = glm::dot(ray, normal) / (glm::length(ray) * glm::length(normal));
    const float c = (1 - cosAlpha * cosAlpha) * (nu * nu);

    // total internal reflection
    if (c > 1.0f)
    {
        return { 0, 0, 0 };
    }

    const float sqr = sqrt(1 - c);

    return (ray * nu) + normal * (cosAlpha * nu - sqr);
}

glm::vec3 calculateRefractionDirection(const Ray& ray, const glm::vec3& normal, const float nu)
{
    return calculateRefractionDirection(ray.direction, normal, nu);
}

glm::vec3 elementWiseMult(const glm::vec3& a, const glm::vec3& b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

glm::vec3 generateRandomVec3(const float min, const float max)
{
    glm::vec3 ret;

    static std::random_device rd;
    static std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(min, max);

    ret.x = dist(e2);
    ret.y = dist(e2);
    ret.z = dist(e2);

    return ret;
}

float generateRandomFloat(const float min, const float max)
{
    static std::random_device rd;
    static std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(min, max);

    return dist(e2);
}

}  // namespace utilities
