#include "Vector3.h"

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(const real _x, const real _y, const real _z) : x(_x), y(_y), z(_z) {}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

void Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

void Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Vector3 Vector3::operator*(const real k) const
{
	return Vector3(x * k, y * k, z * k);
}

real Vector3::operator*(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

void Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void Vector3::operator*=(const real k)
{
	x *= k;
	y *= k;
	z *= k;
}

Vector3 Vector3::operator%(const Vector3 & v) const
{
	return Vector3(y*v.z - z * v.y,
		z*v.x - x * v.z,
		x*v.y - y * v.x);
}

void Vector3::operator%=(const Vector3 &v)
{
	real _x = x;
	real _y = y;
	real _z = z;

	x = _y * v.z - _z * v.y;
	y = _z * v.x - _x * v.z;
	z = _x * v.y - _y * v.x;
}

Vector3 Vector3::operator/(const real k) const
{
	if (k == 0) throw "dividing by zero";

	else return Vector3(x / k, y / k, z / k);
}

void Vector3::operator/=(const real k)
{
	if (k == 0) throw "dividing by zero";

	else
	{
		x /= k;
		y /= k;
		z /= k;
	}

}

real Vector3::Magnitude() const
{
	return real_sqrt(x*x + y * y + z * z);
}

real Vector3::SquareMagnitude() const
{
	return x * x + y * y + z * z;
}

void Vector3::Normalise()
{
	real l = this->Magnitude();

	if (l > 0)
	{
		*this /= l;
	}
}

Vector3 Vector3::Unit() const
{
	Vector3 v = *this;
	v.Normalise();
	return v;
}

bool Vector3::operator==(const Vector3& v) const
{
	return x == v.x &&
		y == v.y &&
		z == v.z;
}

bool Vector3::operator!=(const Vector3& v) const
{
	return !(*this == v);
}

void Vector3::Invert()
{
	x *= -1;
	y *= -1;
	z *= -1;
}

void Vector3::AddScaledVector(const Vector3& v, real k)
{
	*this += v * k;
}

real Vector3::DotProduct(const Vector3& v)
{
	return (*this) * v;
}

Vector3 Vector3::CrossProduct(const Vector3& v)
{
	return (*this) % v;
}

Vector3 Vector3::ComponentProduct(const Vector3& v)
{
	return Vector3(x*v.x, y*v.y, z*v.z);
}

void Vector3::Clear()
{
	x = y = z = 0;
}

real Vector3::operator[](unsigned i) const
{
	if (i == 0) return x;
	if (i == 1) return y;
	return z;
}

real& Vector3::operator[](unsigned i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	return z;
}

void Vector3::OrthonormalBasis(Vector3 & u, Vector3 & v) const
{
	real abs_x = real_abs(x);
	real abs_y = real_abs(y);
	real abs_z = real_abs(z);

	if (abs_x <= abs_y && abs_x <= abs_z)
	{
		u = Vector3(1, y, z);
	}
	else if (abs_y <= abs_x && abs_y <= abs_z)
	{
		u = Vector3(x, 1, z);
	}
	else
	{
		u = Vector3(x, y, 1);
	}

	u = *this % u;
	v = *this % u;

	u.Normalise();
	v.Normalise();
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << v.x << '/' << v.y << '/' << v.z;
	return os;
}
