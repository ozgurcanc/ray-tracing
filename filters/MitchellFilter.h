#include "Filter.h"

class MitchellFilter : public Filter
{
public:
	real GetWeight1D(const real pos);

public:
	real radius = 2;
	real B = (real)1 / 3;
	real C = (real)1 / 3;
};