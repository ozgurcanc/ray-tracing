#include "PerlinTexture.h"

PerlinTexture::PerlinTexture(TextureUsage textureUsage, NoiseConversion noiseConversion, real noiseScale, real bumpFactor)
	:Texture(textureUsage), noiseConversion(noiseConversion), noiseScale(noiseScale), bumpFactor(bumpFactor)
{
	gradient.push_back(Vector3(1, 1, 0));
	gradient.push_back(Vector3(-1, 1, 0));
	gradient.push_back(Vector3(1, -1, 0));
	gradient.push_back(Vector3(-1, -1, 0));

	gradient.push_back(Vector3(1, 0, 1));
	gradient.push_back(Vector3(-1, 0, 1));
	gradient.push_back(Vector3(1, 0, -1));
	gradient.push_back(Vector3(-1, 0, -1));

	gradient.push_back(Vector3(0, 1, 1));
	gradient.push_back(Vector3(0, -1, 1));
	gradient.push_back(Vector3(0, 1, -1));
	gradient.push_back(Vector3(0, -1, -1));

	gradient.push_back(Vector3(1, 1, 0));
	gradient.push_back(Vector3(-1, 1, 0));
	gradient.push_back(Vector3(0, -1, 1));
	gradient.push_back(Vector3(0, -1, -1));

	for (int i = 0; i < 16; i++) phi.push_back(i);

	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(phi.begin(), phi.end(), std::default_random_engine(seed));

}

Vector3 PerlinTexture::Sample(const RayHit * rayHit)
{
	Vector3 perlinPos = rayHit->pos;

	if (textureUsage == REPLACE_KD || textureUsage == BLEND_KD)
	{
		real noise = Noise(perlinPos.x, perlinPos.y, perlinPos.z);
		return Vector3(noise, noise, noise);
	}
	else if (textureUsage == BUMP_NORMAL)
	{
		const real epsilon = 0.001;
		real noise = Noise(perlinPos.x, perlinPos.y, perlinPos.z);
		real x = Noise(perlinPos.x + epsilon, perlinPos.y, perlinPos.z) - noise;
		real y = Noise(perlinPos.x, perlinPos.y + epsilon, perlinPos.z) - noise;
		real z = Noise(perlinPos.x, perlinPos.y, perlinPos.z + epsilon) - noise;

		Vector3 g = Vector3(x / epsilon, y / epsilon, z / epsilon) * bumpFactor;
		Vector3 gParallel = rayHit->normal * (g * rayHit->normal);
		Vector3 gPerpendicular = (g - gParallel);

		return rayHit->normal - gPerpendicular;
	}
	else
	{
		std::cout << "perlin different texture usage" << std::endl;
		return Vector3(0, 0, 255);
	}
}

inline real PerlinTexture::Weight(real t)
{
	t = real_abs(t);
	return 1 - (t * t*t*(t*(t*6.0 - 15.0) + 10.0));
}

real PerlinTexture::Noise(real x, real y, real z)
{
	Vector3 p = Vector3(x, y, z) * noiseScale;

	Vector3 min_corner = Vector3(real_floor(p.x), real_floor(p.y), real_floor(p.z));

	real noise = 0;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				Vector3 curr_corner = min_corner + Vector3(i, j, k);

				int index = phi[(int)real_abs(curr_corner.z) % 16];
				index = phi[(int)real_abs(curr_corner.y + index) % 16];
				index = phi[(int)real_abs(curr_corner.x + index) % 16];

				Vector3 g = gradient[index];
				Vector3 v = p - curr_corner;

				noise += g * v * Weight(v.x) * Weight(v.y) * Weight(v.z);
			}
		}
	}

	assert(noise <= 1);
	assert(noise >= -1);

	if (noiseConversion == ABSOLUTE) noise = real_abs(noise);
	else noise = (noise + 1.0) * 0.5;

	return noise;
}
