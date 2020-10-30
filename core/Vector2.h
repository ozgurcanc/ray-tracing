#pragma once
#include "headers.h"

class Vector2
{
public:

	real x;
	real y;

	Vector2();
	Vector2(real x, real y);

	Vector2 operator*(const real k) const;
	Vector2 operator+(const Vector2 v) const;
	Vector2 operator-(const Vector2 v) const;
};