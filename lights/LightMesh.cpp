#include "LightMesh.h"
#include <random>

Vector3 LightMesh::ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices)
{
	Vector3 contribution;

	if (rayHit->object_id == object_id) return contribution;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0, 1);

	int i = 0;
	real r1 = rng(gen);
	for (; i < weight_cdf.size(); i++)
	{
		if (r1 <= weight_cdf[i]) break;
	}

	Vector3 light_pos = transformation.TransformPoint(primitives.at(primitive_start + i).primitive->RandomPoint(vertices));

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

LightMesh::LightMesh(BVH * object, Matrix4& transform, int matIndex, Vector3 motion, bool smoothShading, int texIndex[2], int texOffset)
	:ObjectInfo(object, transform, matIndex, motion, smoothShading, texIndex, texOffset)
{
}