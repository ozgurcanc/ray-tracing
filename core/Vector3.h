#pragma once
#include "headers.h"

class Vector3
{

public:

	real x, y, z;

	Vector3();
	Vector3(const real _x, const real _y, const real _z);

	Vector3 operator+(const Vector3& v) const;
	void operator+=(const Vector3& v);
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator-() const;
	void operator-=(const Vector3& v);
	Vector3 operator*(const real k) const;
	real operator*(const Vector3& v) const;
	void operator*=(const Vector3& v);
	void operator*=(const real k);
	Vector3 operator%(const Vector3 & v) const;
	void operator%=(const Vector3 &v);
	Vector3 operator/(const real k) const;
	void operator/=(const real k);
	real Magnitude() const;
	real SquareMagnitude() const;
	void Normalise();
	Vector3 Unit() const;
	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;
	void Invert();
	void AddScaledVector(const Vector3& v, real k);
	real DotProduct(const Vector3& v);
	Vector3 CrossProduct(const Vector3& v);
	Vector3 ComponentProduct(const Vector3& v);
	void Clear();
	real operator[](unsigned i) const;
	real& operator[](unsigned i);
	void OrthonormalBasis(Vector3 & u, Vector3 & v) const;

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
};
