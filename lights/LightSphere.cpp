#include "LightSphere.h"

LightSphere::LightSphere(BVH * object, Matrix4& transform, int matIndex, Vector3 motion, bool smoothShading, int texIndex[2], int texOffset)
	:ObjectInfo(object, transform, matIndex, motion, smoothShading, texIndex, texOffset)
{
}

Vector3 LightSphere::ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices)
{
	Vector3 contribution;

	if (rayHit->object_id == object_id) return contribution;

	Vector3 light_pos = transformation.TransformPoint(primitives[0].primitive->RandomPoint(vertices));

	Vector3 toLight = light_pos - rayHit->pos;
	real distanceToLight = toLight.Magnitude();
	toLight.Normalise();

	Ray shadowRay(rayHit->pos + rayHit->normal * 0.001, toLight, ray_time);

	RayHit *shadowHit = new RayHit();

	for (ObjectInfo* object : objects)
	{
		object->Intersect(shadowRay, primitives, vertices, shadowHit);
	}

	if (shadowHit->t < distanceToLight && shadowHit->object_id != object_id)
	{
		delete shadowHit;
		return contribution;
	}

	delete shadowHit;

	contribution = material.BRDF(rayHit->toEye, rayHit->normal, toLight);
	contribution *= radiance;

	return contribution;
}