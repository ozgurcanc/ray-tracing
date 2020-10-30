#include "Filter.h"

class TriangleFilter : public Filter
{
public:
	real GetWeight1D(const real pos);

public:
	real radius = 0.5;
};