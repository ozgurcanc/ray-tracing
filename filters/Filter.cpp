#include "Filter.h"

real Filter::GetWeight2D(const Vector2 & pos)
{
	return GetWeight1D(pos.x) * GetWeight1D(pos.y);
}

Vector3 Filter::FilteredColor(const std::vector<Sample> &samples)
{
	Vector3 pixel_color(0, 0, 0);
	real total_weight = 0;

	for (Sample sample : samples)
	{
		real weight = GetWeight2D(sample.pos);
		pixel_color += sample.color * weight;
		total_weight += weight;
	}

	return pixel_color * (real(1) / total_weight);
}


