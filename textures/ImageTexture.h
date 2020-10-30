#include "Texture.h"

class ImageTexture : public Texture
{
protected:
	Vector3 ** data;
	Interpolation interpolation;
	unsigned width;
	unsigned height;
	real bumpFactor;

public:
	ImageTexture(const Image * image, Interpolation interpolation, TextureUsage textureUsage, real bumpFactor);
	Vector3 Sample(const RayHit * rayHit);
};