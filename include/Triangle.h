#pragma once

#include "glm.hpp"

struct Triangle
{
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
        : a(a)
        , b(b)
        , c(c)
    {
    }

    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
};