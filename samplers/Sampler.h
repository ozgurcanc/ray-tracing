#pragma once
#include "../core/Vector3.h"
#include "../core/Vector2.h"
#include "../core/Ray.h"
#include <random>

struct Sample
{
	Vector2 pos;
	Ray ray;
	Vector3 color;
};

class Sampler
{
public:
	Sampler(int x, int y);
	virtual void CreateSamples(std::vector<Sample> &samples) = 0;

public:
	int x, y;
	real dx,dy;
};
