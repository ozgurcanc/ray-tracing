#include "Sphere.h"
#include <random>

Sphere::Sphere()
{
}

void Sphere::Intersect(const Ray& ray, const std::vector<Vertex> &vertices, RayHit * rayHit, bool smoothShading, int texOffset, bool texturing)
{
	// locally, centered at (0,0,0) and radius 1

	Vector3 centerToOrigin = ray.origin;

	real a = ray.direction * ray.direction;
	real b = ray.direction * centerToOrigin;
	real c = centerToOrigin * centerToOrigin - 1;

	real delta = b * b - a * c;

	if (delta > 0)
	{
		real sqrtDelta = real_sqrt(delta);
		real t1 = (-b - sqrtDelta) / a;
		real t2 = (-b + sqrtDelta) / a;

		real tmin, tmax;

		if (t1 < t2)
		{
			tmin = t1;
			tmax = t2;
		}
		else
		{
			tmin = t2;
			tmax = t1;
		}

		if (tmin > 0)
		{
			if (tmin < rayHit->t)
			{
				rayHit->t = tmin;
				rayHit->pos = ray.GetPoint(tmin);
				rayHit->normal = (rayHit->pos).Unit();
				rayHit->toEye = ray.direction * -1;

				if (!texturing) return;
				rayHit->texCoord.x = (R_PI - real_atan2(rayHit->pos.z, rayHit->pos.x)) / (2 * R_PI);
				rayHit->texCoord.y = real_acos(rayHit->pos.y) / R_PI;
				rayHit->tangent = Vector3(rayHit->pos.z * 2 * R_PI, 0, -rayHit->pos.x * 2 * R_PI);
			}
			return;
		}

		if (tmax > 0)
		{
			if (tmax < rayHit->t)
			{
				rayHit->t = tmax;
				rayHit->pos = ray.GetPoint(tmax);
				rayHit->normal = (rayHit->pos).Unit();
				rayHit->toEye = ray.direction * -1;

				if (!texturing) return;
				rayHit->texCoord.x = (R_PI - real_atan2(rayHit->pos.z, rayHit->pos.x)) / (2 * R_PI);
				rayHit->texCoord.y = real_acos(rayHit->pos.y) / R_PI;
				rayHit->tangent = Vector3(rayHit->pos.z * 2 * R_PI, 0, -rayHit->pos.x * 2 * R_PI);
			}
			return;
		}

		return;
	}
	else if (delta == 0)
	{
		real t = -b / a;

		if (t > 0 && t < rayHit->t)
		{
			rayHit->t = t;
			rayHit->pos = ray.GetPoint(t);
			rayHit->normal = (rayHit->pos).Unit();
			rayHit->toEye = ray.direction * -1;

			if (!texturing) return;
			rayHit->texCoord.x = (R_PI - real_atan2(rayHit->pos.z, rayHit->pos.x)) / (2 * R_PI);
			rayHit->texCoord.y = real_acos(rayHit->pos.y) / R_PI;
			rayHit->tangent = Vector3(rayHit->pos.z * 2 * R_PI, 0, -rayHit->pos.x * 2 * R_PI);
		}
	}
	else return;

}


real Sphere::Area(const std::vector<Vertex> & vertices)
{
	printf("Sphere Area is called\n");
	return 0;
}


Vector3 Sphere::RandomPoint(const std::vector<Vertex> &vertices)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0, 1);

	real theta = rng(gen) * R_PI;
	real phi = rng(gen) * R_PI * 2;

	Vector3 p;
	p.x = real_sin(theta) * real_cos(phi);
	p.y = real_sin(theta) * real_sin(phi);
	p.z = real_cos(theta);

	return p;
}