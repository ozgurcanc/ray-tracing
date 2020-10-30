#include "Texture.h"

class PerlinTexture : public Texture
{
protected:
	real noiseScale = 1;
	NoiseConversion noiseConversion;
	real bumpFactor;

	std::vector<Vector3> gradient;
	std::vector<int> phi;

public:
	PerlinTexture(TextureUsage textureUsage, NoiseConversion noiseConversion, real noiseScale, real bumpFactor);
	Vector3 Sample(const RayHit * rayHit);

private:
	real Weight(real x);
	real Noise(real x, real y, real z);
};