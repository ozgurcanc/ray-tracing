#include "EnvironmentLight.h"
#include <random>

EnvironmentLight::EnvironmentLight(const Image * image)
{
	width = image->width;
	height = image->height;

	hdrMAP = new Vector3*[height];

	for (int i = 0; i < height; i++)
	{
		hdrMAP[i] = new Vector3[width];

		for (int j = 0; j < width; j++)
		{
			hdrMAP[i][j] = Vector3(image->data[i][j].red, image->data[i][j].grn, image->data[i][j].blu);
		}
	}
}

Vector3 EnvironmentLight::ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices)
{
	Vector3 contribution;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0, 1);

	Vector3 toLight;
	Vector3 u, v;

	rayHit->normal.OrthonormalBasis(u, v);

	real s2 = rng(gen);
	real sqr_s2 = real_sqrt(1 - s2 * s2);
	real s1 = rng(gen) * 2 * R_PI;

	toLight = u * sqr_s2 * real_cos(s1) + rayHit->normal * s2 + v * sqr_s2 * real_sin(s1);

	toLight.Normalise();

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
	contribution *= EnviromentColor(toLight) * 2 * R_PI;

	return contribution;
}

Vector3 EnvironmentLight::EnviromentColor(const Vector3& direction)
{
	real phi = real_acos(direction.y);
	real theta = real_atan2(direction.z, direction.x);

	int x = ((R_PI - theta) / (2 * R_PI)) * width;
	int y = (phi / R_PI) * height;

	if (x == width) x--;
	if (y == height) y--;

	return hdrMAP[y][x];
}