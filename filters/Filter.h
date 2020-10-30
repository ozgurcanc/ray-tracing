#pragma once
#include "../samplers/Sampler.h"

class Filter
{
public:
	Vector3 FilteredColor(const std::vector<Sample> &samples);
	virtual real GetWeight1D(const real pos) = 0;
	real GetWeight2D(const Vector2 & pos);
};


