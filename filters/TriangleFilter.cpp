#include "TriangleFilter.h"

real TriangleFilter::GetWeight1D(const real pos)
{
	return std::max(real(0), radius - real_abs(pos));
}