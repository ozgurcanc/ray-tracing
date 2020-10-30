#include "ImageTexture.h"

ImageTexture::ImageTexture(const Image *image, Interpolation interpolation, TextureUsage textureUsage, real bumpFactor)
	: Texture(textureUsage), interpolation(interpolation), bumpFactor(bumpFactor)
{
	width = image->width;
	height = image->height;

	data = new Vector3*[height];

	for (int i = 0; i < height; i++)
	{
		data[i] = new Vector3[width];

		for (int j = 0; j < width; j++)
		{
			Vector3 pixelColor(real(image->data[i][j].red), real(image->data[i][j].grn), real(image->data[i][j].blu));

			if (textureUsage == BLEND_KD || textureUsage == REPLACE_KD)
				pixelColor *= 1 / real(255);
			else if (textureUsage == REPLACE_NORMAL)
				pixelColor = (pixelColor - Vector3(127.5, 127.5, 127.5)).Unit();

			data[i][j] = pixelColor;
		}
	}
}

Vector3 ImageTexture::Sample(const RayHit * rayHit)
{
	Vector2 texCoord;
	texCoord.x = (rayHit->texCoord.x - (int)rayHit->texCoord.x);
	texCoord.y = (rayHit->texCoord.y - (int)rayHit->texCoord.y);

	if (texCoord.x < 0) texCoord.x += 1;
	if (texCoord.y < 0) texCoord.y += 1;

	texCoord.x *= width;
	texCoord.y *= height;

	if (textureUsage == BUMP_NORMAL)
	{
		int x = (int)texCoord.x;
		int y = (int)texCoord.y;

		Vector3 value = data[y][x];
		Vector3 value_x = x < (width - 1) ? data[y][x + 1] - value : Vector3();
		Vector3 value_y = y < (height - 1) ? data[y + 1][x] - value : Vector3();

		Vector3 binormal = rayHit->tangent % rayHit->normal;

		real dx = (value_x.x + value_x.y + value_x.z) / 3.0;
		real dy = (value_y.x + value_y.y + value_y.z) / 3.0;

		dx *= bumpFactor;
		dy *= bumpFactor;

		Vector3 tangentPrime = rayHit->tangent + rayHit->normal * dx;
		Vector3 binormalPrime = binormal + rayHit->normal * dy;

		return (binormalPrime % tangentPrime);
	}
	else
	{
		Vector3 texValue;

		if (interpolation == NEAREST_NEIGHBOR)
		{
			texValue = data[(int)texCoord.y][(int)texCoord.x];
		}
		else
		{
			real max_u = real_round(texCoord.x);
			real max_v = real_round(texCoord.y);

			real dx = texCoord.x + 0.5 - max_u;
			real dy = texCoord.y + 0.5 - max_v;

			real total_weight = 0;

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					int u = max_u - i;
					int v = max_v - j;

					if (u < 0 || u >= width || v < 0 || v >= height) continue;

					real weight = real_abs(i - dx) * real_abs(j - dy);
					texValue += data[v][u] * weight;
					total_weight += weight;
				}
			}

			assert(total_weight != 0);

			texValue *= (1 / total_weight);
		}

		if (textureUsage == REPLACE_NORMAL)
		{
			Vector3 tangent = rayHit->tangent.Unit();
			Vector3 binormal = tangent % rayHit->normal;

			Matrix4 tbn(tangent.x, binormal.x, rayHit->normal.x, 0,
				tangent.y, binormal.y, rayHit->normal.y, 0,
				tangent.z, binormal.z, rayHit->normal.z, 1);

			texValue = tbn.TransformDirection(texValue);
		}

		return texValue;
	}
}