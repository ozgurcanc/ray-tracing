#include "Filter.h"

class GaussianFilter : public Filter
{
public:
	GaussianFilter();
	real GetWeight1D(const real pos);

public:
	real radius = 2;
	real alpha = 2;
	real exp;
};