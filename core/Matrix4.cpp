#include "Matrix4.h"

Matrix4::Matrix4()
{
	data[1] = data[2] = data[3] = data[4] = data[6] =
		data[7] = data[8] = data[9] = data[11] = 0;
	data[0] = data[5] = data[10] = 1;
}

Matrix4::Matrix4(real d0, real d1, real d2, real d3, real d4, real d5, real d6, real d7, real d8, real d9, real d10, real d11)
{
	data[0] = d0; data[1] = d1; data[2] = d2; data[3] = d3;
	data[4] = d4; data[5] = d5; data[6] = d6; data[7] = d7;
	data[8] = d8; data[9] = d9; data[10] = d10; data[11] = d11;
}

Matrix4 Matrix4::operator*(const Matrix4 &o) const
{
	Matrix4 result;
	result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]);
	result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]);
	result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]);

	result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]);
	result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]);
	result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]);

	result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]);
	result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]);
	result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]);

	result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + data[3];
	result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + data[7];
	result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + data[11];

	return result;
}

void Matrix4::operator*=(const Matrix4 &o)
{
	*this = *this * o;
}

real Matrix4::GetDeterminant() const
{
	return -data[8] * data[5] * data[2] +
		data[4] * data[9] * data[2] +
		data[8] * data[1] * data[6] -
		data[0] * data[9] * data[6] -
		data[4] * data[1] * data[10] +
		data[0] * data[5] * data[10];
}

Vector3 Matrix4::TransformPoint(const Vector3 &v) const
{
	return Vector3(
		v.x * data[0] +
		v.y * data[1] +
		v.z * data[2] + data[3],

		v.x * data[4] +
		v.y * data[5] +
		v.z * data[6] + data[7],

		v.x * data[8] +
		v.y * data[9] +
		v.z * data[10] + data[11]
	);
}

Vector3 Matrix4::TransformDirection(const Vector3 &v) const
{
	return Vector3(
		v.x * data[0] +
		v.y * data[1] +
		v.z * data[2],

		v.x * data[4] +
		v.y * data[5] +
		v.z * data[6],

		v.x * data[8] +
		v.y * data[9] +
		v.z * data[10]
	);
}

Vector3 Matrix4::TransformNormal(const Vector3 &v) const
{
	return Vector3(
		v.x * data[0] +
		v.y * data[4] +
		v.z * data[8],

		v.x * data[1] +
		v.y * data[5] +
		v.z * data[9],

		v.x * data[2] +
		v.y * data[6] +
		v.z * data[10]
	);
}

void Matrix4::SetInverse(const Matrix4 &m)
{
	real det = m.GetDeterminant();
	if (det == 0) return;
	det = ((real)1.0) / det;

	data[0] = (-m.data[9] * m.data[6] + m.data[5] * m.data[10])*det;
	data[4] = (m.data[8] * m.data[6] - m.data[4] * m.data[10])*det;
	data[8] = (-m.data[8] * m.data[5] + m.data[4] * m.data[9])*det;

	data[1] = (m.data[9] * m.data[2] - m.data[1] * m.data[10])*det;
	data[5] = (-m.data[8] * m.data[2] + m.data[0] * m.data[10])*det;
	data[9] = (m.data[8] * m.data[1] - m.data[0] * m.data[9])*det;

	data[2] = (-m.data[5] * m.data[2] + m.data[1] * m.data[6])*det;
	data[6] = (+m.data[4] * m.data[2] - m.data[0] * m.data[6])*det;
	data[10] = (-m.data[4] * m.data[1] + m.data[0] * m.data[5])*det;

	data[3] = (m.data[9] * m.data[6] * m.data[3]
		- m.data[5] * m.data[10] * m.data[3]
		- m.data[9] * m.data[2] * m.data[7]
		+ m.data[1] * m.data[10] * m.data[7]
		+ m.data[5] * m.data[2] * m.data[11]
		- m.data[1] * m.data[6] * m.data[11])*det;
	data[7] = (-m.data[8] * m.data[6] * m.data[3]
		+ m.data[4] * m.data[10] * m.data[3]
		+ m.data[8] * m.data[2] * m.data[7]
		- m.data[0] * m.data[10] * m.data[7]
		- m.data[4] * m.data[2] * m.data[11]
		+ m.data[0] * m.data[6] * m.data[11])*det;
	data[11] = (m.data[8] * m.data[5] * m.data[3]
		- m.data[4] * m.data[9] * m.data[3]
		- m.data[8] * m.data[1] * m.data[7]
		+ m.data[0] * m.data[9] * m.data[7]
		+ m.data[4] * m.data[1] * m.data[11]
		- m.data[0] * m.data[5] * m.data[11])*det;
}

Matrix4 Matrix4::GetInverse() const
{
	Matrix4 inverse;
	inverse.SetInverse(*this);
	return inverse;
}

void Matrix4::SetInverse()
{
	SetInverse(*this);
}

Matrix4 Matrix4::Translate(real x, real y, real z)
{
	Matrix4 m;
	m.data[3] = x;
	m.data[7] = y;
	m.data[11] = z;

	return m;
}

Matrix4 Matrix4::Scale(real x, real y, real z)
{
	Matrix4 m;
	m.data[0] = x;
	m.data[5] = y;
	m.data[10] = z;

	return m;
}

Matrix4 Matrix4::Rotate(real angle, real x, real y, real z)
{
	Vector3 axis = Vector3(x, y, z).Unit();
	real sin = real_sin(angle * DegToRad);
	real cos = real_cos(angle * DegToRad);

	Matrix4 m;

	m.data[0] = axis.x * axis.x + (1 - axis.x * axis.x) * cos;
	m.data[1] = axis.x * axis.y * (1 - cos) - axis.z * sin;
	m.data[2] = axis.x * axis.z * (1 - cos) + axis.y * sin;

	m.data[4] = axis.x * axis.y * (1 - cos) + axis.z * sin;
	m.data[5] = axis.y * axis.y + (1 - axis.y * axis.y) * cos;
	m.data[6] = axis.y * axis.z * (1 - cos) - axis.x * sin;

	m.data[8] = axis.x * axis.z * (1 - cos) - axis.y * sin;
	m.data[9] = axis.y * axis.z * (1 - cos) + axis.x * sin;
	m.data[10] = axis.z * axis.z + (1 - axis.z * axis.z) * cos;

	return m;
}


void Matrix4::SetAbs()
{
	for (int i = 0; i < 12; i++)
		data[i] = real_abs(data[i]);
}

std::ostream& operator<<(std::ostream& os, const Matrix4& m)
{
	os << m.data[0] << " " << m.data[1] << " " << m.data[2] << " " << m.data[3] << std::endl;
	os << m.data[4] << " " << m.data[5] << " " << m.data[6] << " " << m.data[7] << std::endl;
	os << m.data[8] << " " << m.data[9] << " " << m.data[10] << " " << m.data[11] << std::endl;
	return os;
}