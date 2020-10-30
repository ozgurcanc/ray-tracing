#include "Vector2.h"

Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(real x, real y) : x(x), y(y) {}

Vector2 Vector2::operator*(const real k) const
{
	return Vector2(x*k, y*k);
}

Vector2 Vector2::operator+(const Vector2 v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2 v) const
{
	return Vector2(x - v.x, y - v.y);
}