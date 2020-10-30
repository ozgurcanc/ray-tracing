#include "LanczosFilter.h"

real LanczosFilter::GetWeight1D(const real pos)
{
	real x = pos;
	x = real_abs(x);

	if (x > radius) return 0;

	real lanczos = Sinc(x / tau);
	return Sinc(x) * lanczos;
}

real LanczosFilter::Sinc(real x)
{
	x = real_abs(x);
	if (x < 1e-5) return 1;

	real pi_x = R_PI * x;
	return real_sin(pi_x) / pi_x;
}