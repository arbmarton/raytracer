#pragma once

#include "Object.h"
#include "Utilities.h"

#include "glm.hpp"

struct Sphere : public Object
{
    Sphere(const glm::vec3& pos, const glm::vec3& color, const float radius, const float ka, const float kr, const float kt)
        : Object(ka, kr, kt)
        , pos(pos)
        , color(color)
        , radius(radius)
    {
    }

    virtual IntersectionInfo intersect(const Ray& ray) override
    {
        const std::optional<float> dist = utilities::calculateSphereIntersectionDistance(ray, *this);
        if (!dist)
        {
            return { nullptr, std::numeric_limits<float>::max() };
        }
        else
        {
            return { this, *utilities::calculateSphereIntersectionDistance(ray, *this) };
        }       
    }

    virtual glm::vec3 getNormal(const Ray& ray, const IntersectionInfo& info) const override
    {
        return ray.origin + ray.direction * info.distance - pos;
    }

    virtual glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info) const override
    {
        return color;
    }

    glm::vec3 pos;
    glm::vec3 color;
    float radius;
};