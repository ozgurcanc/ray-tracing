#pragma once

#include "Vector3.h"
#include "Vector2.h"

class Ray
{
public:
	Vector3 origin;
	Vector3 direction;
	real time;

	Ray();
	Ray(const Vector3 & origin, const Vector3 &direction);
	Ray(const Vector3 & origin, const Vector3 &direction, real time);
	Vector3 GetPoint(float t) const;

public:
	Vector3 invDir;
	int sign[3];

public:
	Vector2 pixelCoord;
};

