#pragma once
#include "../renderer/Image.h"
#include "../shapes/Shape.h"
#include "../core/Matrix4.h"
#include <iostream>     
#include <algorithm>   
#include <array>        
#include <random>      
#include <chrono>

enum Interpolation
{
	NEAREST_NEIGHBOR, BILINEAR
};

enum TextureUsage
{
	REPLACE_KD, BLEND_KD, REPLACE_ALL, REPLACE_NORMAL, BUMP_NORMAL, REPLACE_BACKGROUND
};

enum NoiseConversion
{
	LINEAR, ABSOLUTE
};

class Texture
{
public:
	TextureUsage textureUsage;

public:
	Texture(TextureUsage textureUsage);
	virtual Vector3 Sample(const RayHit * rayHit) = 0;
};






