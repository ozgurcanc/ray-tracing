#include "MitchellFilter.h"

real MitchellFilter::GetWeight1D(const real pos)
{
	real x = real_abs(pos);

	if (x > 0.25)
	{
		return ((-B - 6 * C) * x * x * x + (6 * B + 30 * C) * x * x +
			(-12 * B - 48 * C) * x + (8 * B + 24 * C)) *
			(1 / (real)6);
	}

	return ((12 - 9 * B - 6 * C) * x * x * x +
		(-18 + 12 * B + 6 * C) * x * x + (6 - 2 * B)) *
		(1 / (real)6);
}