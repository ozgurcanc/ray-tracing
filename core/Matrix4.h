#pragma once
#include "Vector3.h"

class Matrix4
{
public:
	Matrix4();
	Matrix4(real d0, real d1, real d2, real d3, real d4, real d5, real d6, real d7, real d8, real d9, real d10, real d11);
	
	Matrix4 operator*(const Matrix4 &o) const;
	void operator*=(const Matrix4 &o);

	Vector3 TransformPoint(const Vector3 &v) const;
	Vector3 TransformDirection(const Vector3 &v) const;
	Vector3 TransformNormal(const Vector3 &v) const;

	real GetDeterminant() const;
	void SetInverse(const Matrix4 &m);
	Matrix4 GetInverse() const;
	void SetInverse();

	void SetAbs();

	static Matrix4 Translate(real x, real y, real z);
	static Matrix4 Scale(real x, real y, real z);
	static Matrix4 Rotate(real angle, real x, real y, real z);

	friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);

public:
	real data[12];
};
