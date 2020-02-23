#pragma once

#include "Ray.h"

struct Object;

struct IntersectionInfo
{
	Object* obj;
	float distance;
};

struct Object
{
	Object(const float ka, const float kr, const float kt)
		: ka(ka)
		, kr(kr)
		, kt(kt)
	{

	}

	virtual IntersectionInfo intersect(const Ray& ray) = 0;
	virtual glm::vec3 getNormal(const Ray& ray, const IntersectionInfo& info) const = 0;
	virtual glm::vec3 getColor(const Ray& ray, const IntersectionInfo& info) const = 0;

	float ka;
	float kr;
	float kt;
};