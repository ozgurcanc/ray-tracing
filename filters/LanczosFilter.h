#include "Filter.h"

class LanczosFilter : public Filter
{
public:
	real GetWeight1D(const real pos);
	real Sinc(real x);

public:
	real radius = 0.5;
	real tau = 3;

};