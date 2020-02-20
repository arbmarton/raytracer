#pragma once

#include "glm.hpp"

struct Sphere
{
    glm::vec3 pos;
    glm::vec3 color;
    float radius;

    float ka = 0.5f;
    float kr = 0.5f;
    float kt = 0.5f;
};