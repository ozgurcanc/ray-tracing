#include "AreaLight.h"
#include <random>

AreaLight::AreaLight(const Vector3& p, const Vector3& d, const Vector3& i, real s)
{
	position = p;
	direction = d.Unit();
	intensity = i;
	size = s;

	direction.OrthonormalBasis(u, v);
}

Vector3 AreaLight::ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices)
{
	Vector3 contribution;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(-size / 2, size / 2);

	Vector3 toLight = position + u * rng(gen) + v * rng(gen) - rayHit->pos;
	real distanceToLight = toLight.Magnitude();
	toLight.Normalise();

	Ray shadowRay(rayHit->pos + rayHit->normal * 0.001, toLight, ray_time);

	RayHit *shadowHit = new RayHit();

	for (ObjectInfo* object : objects)
	{
		object->Intersect(shadowRay, primitives, vertices, shadowHit);
	}

	if (shadowHit->t < distanceToLight)
	{
		delete shadowHit;
		return contribution;
	}

	delete shadowHit;

	contribution = material.BRDF(rayHit->toEye, rayHit->normal, toLight);
	contribution *= intensity / (distanceToLight * distanceToLight);

	contribution *= size * size * real_abs(direction * -toLight);

	return contribution;
}