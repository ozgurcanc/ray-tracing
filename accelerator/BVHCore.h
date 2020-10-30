#pragma once
#include "../core/Vector3.h"
#include "../shapes/Shape.h"
#include "../core/Matrix4.h"
#include "../core/Vertex.h"

class BoundingBox
{

public:
	Vector3 pMin, pMax;

public:
	BoundingBox();
	BoundingBox(const Vector3& p1);
	BoundingBox(const Vector3& p1, const Vector3& p2);
	BoundingBox(const Vector3& p1, const Vector3& p2, const Vector3 p3);
	Vector3 Diagonal() const;
	real SurfaceArea() const;
	int MaximumExtent() const;
	Vector3 Offset(const Vector3& p) const;
	const Vector3 &operator[](int i) const;
	Vector3 &operator[](int i);

	bool Intersect(const Ray& ray, RayHit *rayhit);

	friend std::ostream& operator<<(std::ostream& os, const BoundingBox& b);
};

struct PrimitiveInfo
{
public:
	PrimitiveInfo();
	PrimitiveInfo(Shape* primitive, const BoundingBox &bounds);
	PrimitiveInfo(Shape* primitive, const Vector3& p1, const Vector3& p2, const Vector3& p3);
	PrimitiveInfo(Shape* primitive, const Vector3& p1, const real radius);

public:
	Shape* primitive;
	BoundingBox bounds;
	Vector3 center;
};

extern BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2);
extern BoundingBox Union(const BoundingBox& b, const Vector3& p);