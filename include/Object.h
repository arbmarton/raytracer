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
	virtual IntersectionInfo intersect(const Ray& ray) = 0;
};