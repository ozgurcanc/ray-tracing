#include "Triangle.h"
#include <random>

Triangle::Triangle(int v1Index, int v2Index, int v3Index)
	:v1Index(v1Index), v2Index(v2Index), v3Index(v3Index)
{
}

void Triangle::Intersect(const Ray& ray, const std::vector<Vertex> &vertices, RayHit * rayHit, bool smoothShading, int texOffset, bool texturing)
{
	Vector3 e1 = vertices.at(v2Index).position - vertices.at(v1Index).position;
	Vector3 e2 = vertices.at(v3Index).position - vertices.at(v1Index).position;
	Vector3 m = ray.origin - vertices.at(v1Index).position;

	Vector3 d_e2 = (ray.direction % e2);
	real divider = e1 * d_e2;

	if (divider == 0) return;

	Vector3 m_e1 = m % e1;

	real t = (e2 * m_e1) / divider;

	if (t <= 0 || t >= rayHit->t) return;

	real v = (ray.direction * m_e1) / divider;

	if (v < 0 || v>1) return;

	real u = (m * d_e2) / divider;

	if (u < 0 || u>1 || u + v > 1) return;

	rayHit->t = t;
	rayHit->pos = ray.GetPoint(t);

	if (smoothShading) rayHit->normal = vertices.at(v1Index).normal * (1 - u - v) + vertices.at(v2Index).normal * u + vertices.at(v3Index).normal * v;
	else rayHit->normal = (e1 % e2).Unit();

	rayHit->toEye = ray.direction * -1;

	if (!texturing) return;

	Vector2 v1_texCoord = vertices.at(v1Index + texOffset).texCoord;
	Vector2 v2_texCoord = vertices.at(v2Index + texOffset).texCoord;
	Vector2 v3_texCoord = vertices.at(v3Index + texOffset).texCoord;

	rayHit->texCoord = v1_texCoord * (1 - u - v) + v2_texCoord * u + v3_texCoord * v;

	Vector2 delta0 = v2_texCoord - v1_texCoord;
	Vector2 delta1 = v3_texCoord - v1_texCoord;

	rayHit->tangent = Vector3(delta1.y * e1.x - delta0.y * e2.x, delta1.y * e1.y - delta0.y * e2.y, delta1.y * e1.z - delta0.y * e2.z);
	rayHit->tangent *= 1 / (delta0.x * delta1.y - delta0.y * delta1.x);

}

real Triangle::Area(const std::vector<Vertex> & vertices)
{
	Vector3 e1 = vertices.at(v2Index).position - vertices.at(v1Index).position;
	Vector3 e2 = vertices.at(v3Index).position - vertices.at(v1Index).position;

	return (e1 % e2).Magnitude();
}


Vector3 Triangle::RandomPoint(const std::vector<Vertex> &vertices)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0, 1);

	real r1 = rng(gen);
	real r2 = real_sqrt(rng(gen));

	Vector3 random_pos = vertices.at(v2Index).position * r1 + vertices.at(v3Index).position * (1 - r1);
	return random_pos * r2 + vertices.at(v1Index).position * (1 - r2);

}