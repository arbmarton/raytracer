#pragma once

#include "Object.h"
#include "Triangle.h"

#include "glm.hpp"

struct Quad : public Object
{
	Quad(const glm::vec3& upperLeft, const glm::vec3& u, const glm::vec3& v, const float width, const float height, const glm::vec3& color, const float ka, const float kr, const float kt);

	virtual IntersectionInfo intersect(const Ray& ray) override;
	virtual glm::vec3 getNormal(const Ray& ray, const IntersectionInfo& info) const
	{
		return normal;
	}
	virtual glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info) const
	{
		return color;
	}

	Triangle lower;
	Triangle upper;
	//glm::vec3 upperLeft;
	//glm::vec3 u;
	//glm::vec3 v;
	glm::vec3 normal;
	glm::vec3 color; // replace with texture
	//float width;
	//float height;
};