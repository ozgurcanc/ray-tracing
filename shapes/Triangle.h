#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle(int index1, int index2, int index3);
	void Intersect(const Ray& ray, const std::vector<Vertex> &vertices, RayHit * rayHit, bool smoothShading, int texOffset, bool texturing);
	Vector3 RandomPoint(const std::vector<Vertex> &vertices);
	real Area(const std::vector<Vertex> & vertices);

private:
	int v1Index;
	int v2Index;
	int v3Index;
};