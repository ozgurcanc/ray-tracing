#include "SpotLight.h"

SpotLight::SpotLight(const Vector3& p, const Vector3& d, const Vector3& i, real cosa, real cosb, real s)
{
	position = p;
	direction = d.Unit();
	intensity = i;
	cos_max = real_cos((cosa / 2) * DegToRad);
	cos_min = real_cos((cosb / 2) * DegToRad);
	spot = s;
}

Vector3 SpotLight::ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices)
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

	real s = (-toLight * direction - cos_min) / (cos_max - cos_min);

	contribution *= real_pow(clamp(s, real(0), real(1)), spot);

	return contribution;
}