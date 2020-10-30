#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Vector3 & d, const Vector3& i)
{
	direction = d.Unit();
	intensity = i;
}

Vector3 DirectionalLight::ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices)
{
	Vector3 contribution;

	Vector3 toLight = -direction;

	Ray shadowRay(rayHit->pos + rayHit->normal * 0.001, toLight, ray_time);

	RayHit *shadowHit = new RayHit();

	for (ObjectInfo* object : objects)
	{
		object->Intersect(shadowRay, primitives, vertices, shadowHit);
	}

	if (shadowHit->t < REAL_MAX)
	{
		delete shadowHit;
		return contribution;
	}

	delete shadowHit;

	contribution = material.BRDF(rayHit->toEye, rayHit->normal, toLight);
	contribution *= intensity;

	return contribution;
}

