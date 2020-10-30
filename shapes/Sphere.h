#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
	void Intersect(const Ray& ray, const std::vector<Vertex> &vertices, RayHit * rayHit, bool smoothShading, int texOffset, bool texturing);
	Vector3 RandomPoint(const std::vector<Vertex> &vertices);
	real Area(const std::vector<Vertex> & vertices);
};