#include "Plane.h"

#include "Utilities.h"

Plane::Plane(const glm::vec3& offset, const glm::vec3& normal, const glm::vec3& color, const float ka, const float kr, const float kt)
    : Object(ka, kr, kt)
    , offset(offset)
    , normal(normal)
    , color(color)
{
}

IntersectionInfo Plane::intersect(const Ray& ray)
{
    const std::optional<float> dist = utilities::calculatePlaneIntersectionDistance(ray, *this);
    if (!dist)
    {
        return { nullptr, std::numeric_limits<float>::max() };
    }
    else
    {
        return { this, *dist };
    }
}