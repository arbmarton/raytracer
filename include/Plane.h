#pragma once

#include "Object.h"

#include "glm.hpp"

struct Plane : public Object
{
    Plane(const glm::vec3& offset, const glm::vec3& normal, const glm::vec3& color, const float ka, const float kr, const float kt);

    virtual IntersectionInfo intersect(const Ray& ray) override;
    virtual glm::vec3 getNormal(const Ray& ray, const IntersectionInfo& info) const override
    {
        return normal;
    }

    virtual glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info) const override
    {
        return color;
    }

    glm::vec3 offset;
    glm::vec3 normal;
    glm::vec3 color;
};