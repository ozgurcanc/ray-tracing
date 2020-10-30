#include "GaussianFilter.h"

GaussianFilter::GaussianFilter()
{
	exp = real_exp(-alpha * radius * radius);
}

real GaussianFilter::GetWeight1D(const real pos)
{
	return std::max(real(0), real_exp(-alpha * pos * pos) - exp);
}