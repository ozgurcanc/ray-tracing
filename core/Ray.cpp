#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3 & origin, const Vector3 &direction)
	:origin(origin), direction(direction)
{
	invDir.x = 1 / direction.x;
	invDir.y = 1 / direction.y;
	invDir.z = 1 / direction.z;

	sign[0] = invDir.x < 0;
	sign[1] = invDir.y < 0;
	sign[2] = invDir.z < 0;
}

Ray::Ray(const Vector3 & origin, const Vector3 &direction, real time)
	:origin(origin), direction(direction), time(time)
{
	invDir.x = 1 / direction.x;
	invDir.y = 1 / direction.y;
	invDir.z = 1 / direction.z;

	sign[0] = invDir.x < 0;
	sign[1] = invDir.y < 0;
	sign[2] = invDir.z < 0;
}

Vector3 Ray::GetPoint(float t) const
{
	return origin + direction * t;
}