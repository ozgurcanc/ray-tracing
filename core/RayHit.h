#pragma once
#include "Vector2.h"
#include "Vector3.h"

struct RayHit
{
	real t = REAL_MAX;

	Vector3 pos;
	Vector3	normal;
	Vector3 toEye;

	Vector2 texCoord;
	Vector3 tangent;

	int texIndex[2];
	int matIndex;
	int object_id;
};