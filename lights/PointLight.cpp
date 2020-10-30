#include "PointLight.h"

PointLight::PointLight(const Vector3 & p, const Vector3& i)
{
	position = p;
	intensity = i;
}

Vector3 PointLight::ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices)
{
	Vector3 contribution;

	Vector3 toLight = position - rayHit->pos;
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

	return contribution;
}