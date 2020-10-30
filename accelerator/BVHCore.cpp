#include "BVHCore.h"

Vector3 Min(const Vector3& p1, const Vector3& p2)
{
	return Vector3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
}

Vector3 Max(const Vector3& p1, const Vector3& p2)
{
	return Vector3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
}

BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(const Vector3& p1)
{
	pMin = p1;
	pMax = p1;
}

BoundingBox::BoundingBox(const Vector3& p1, const Vector3& p2)
{
	pMin = Min(p1, p2);
	pMax = Max(p1, p2);
}

BoundingBox::BoundingBox(const Vector3& p1, const Vector3& p2, const Vector3 p3)
{
	pMin = Min(p1, Min(p2, p3));
	pMax = Max(p1, Max(p2, p3));
}


Vector3 BoundingBox::Diagonal() const
{
	return pMax - pMin;
}


real BoundingBox::SurfaceArea() const
{
	Vector3 d = Diagonal();
	return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}

int BoundingBox::MaximumExtent() const
{
	Vector3 d = Diagonal();
	if (d.x > d.y && d.x > d.z)
		return 0;
	else if (d.y > d.z)
		return 1;
	else
		return 2;
}

Vector3 BoundingBox::Offset(const Vector3& p) const
{
	Vector3 o = p - pMin;
	if (pMax.x > pMin.x)	o.x /= pMax.x - pMin.x;
	if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
	if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;

	return o;
}


BoundingBox Union(const BoundingBox& b1, const BoundingBox& b2)
{
	BoundingBox b;
	b.pMin = Min(b1.pMin, b2.pMin);
	b.pMax = Max(b1.pMax, b2.pMax);
	return b;
}

BoundingBox Union(const BoundingBox& b, const Vector3& p)
{
	BoundingBox b1;
	b1.pMin = Min(b.pMin, p);
	b1.pMax = Max(b.pMax, p);
	return b1;
}

const Vector3& BoundingBox::operator[](int i) const
{
	return (i == 0) ? pMin : pMax;
}

Vector3 & BoundingBox::operator[](int i)
{
	return (i == 0) ? pMin : pMax;
}


std::ostream& operator<<(std::ostream& os, const BoundingBox& b)
{
	os << b.pMin << std::endl << b.pMax;
	return os;
}


PrimitiveInfo::PrimitiveInfo()
{
}

PrimitiveInfo::PrimitiveInfo(Shape* primitive, const BoundingBox &bounds)
	:primitive(primitive), bounds(bounds), center((bounds.pMin + bounds.pMax) * (real)0.5)
{
}

PrimitiveInfo::PrimitiveInfo(Shape* primitive, const Vector3& p1, const Vector3& p2, const Vector3& p3)
	: primitive(primitive)
{
	bounds = BoundingBox(p1, p2, p3);
	center = (bounds.pMin + bounds.pMax) * (real)0.5;
}

PrimitiveInfo::PrimitiveInfo(Shape* primitive, const Vector3& p1, const real radius)
	:primitive(primitive)
{
	center = p1;

	Vector3 r(radius, radius, radius);
	bounds.pMin = p1 - r;
	bounds.pMax = p1 + r;
}


bool BoundingBox::Intersect(const Ray& ray, RayHit *rayhit)
{
	const BoundingBox &bounds = *this;
	real tmin = (bounds[ray.sign[0]].x - ray.origin.x) * ray.invDir.x;
	real tmax = (bounds[1 - ray.sign[0]].x - ray.origin.x) * ray.invDir.x;
	real tymin = (bounds[ray.sign[1]].y - ray.origin.y) * ray.invDir.y;
	real tymax = (bounds[1 - ray.sign[1]].y - ray.origin.y) * ray.invDir.y;

	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	real tzmin = (bounds[ray.sign[2]].z - ray.origin.z) * ray.invDir.z;
	real tzmax = (bounds[1 - ray.sign[2]].z - ray.origin.z) * ray.invDir.z;

	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	return (tmin < rayhit->t) && (tmax > 0);

}