#pragma once

#include "Object.h"
#include "Utilities.h"

#include "glm.hpp"

struct Sphere : public Object
{
    Sphere(const glm::vec3& pos, const glm::vec3& color, const float radius, const float ka, const float kr, const float kt)
        : Object()
        , pos(pos)
        , color(color)
        , radius(radius)
        , ka(ka)
        , kr(kr)
        , kt(kt)
    {
    }

    virtual IntersectionInfo intersect(const Ray& ray) override
    {
        return { this, *utilities::calculateSphereIntersectionDistance(ray, *this) };
    }

    glm::vec3 pos;
    glm::vec3 color;
    float radius;

    float ka;
    float kr;
    float kt;
};