#include "Quad.h"

#include "Utilities.h"

Quad::Quad(const glm::vec3& upperLeft, const glm::vec3& u, const glm::vec3& v, const float width, const float height, const glm::vec3& color, const float ka, const float kr, const float kt)
	: Object(ka, kr, kt)
	, lower(upperLeft, upperLeft + v*height, upperLeft + u*width  + v* height)
	, upper(upperLeft, upperLeft + u *width  + v*height, upperLeft + u * width)
	, normal(glm::normalize(glm::cross(lower.b - lower.a, upper.c - upper.a)))
	, color(color)
{

}

IntersectionInfo Quad::intersect(const Ray& ray)
{
	if (glm::dot(ray.direction, normal) > 0)
	{
		return { nullptr, std::numeric_limits<float>::max() };
	}

	const std::optional<float> distance =  utilities::calculatePlaneIntersectionDistance(ray, upper.a, normal);
	if (!distance)
	{
		return { nullptr, std::numeric_limits<float>::max() };
	}

	const glm::vec3 intersectionPoint = ray.origin + ray.direction * *distance;

	if (utilities::isPointInsideTriangle(intersectionPoint, lower) || utilities::isPointInsideTriangle(intersectionPoint, upper))
	{
		return { this, *distance };
	}

	return { nullptr, std::numeric_limits<float>::max() };
}