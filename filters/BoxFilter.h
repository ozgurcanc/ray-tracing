#include "Filter.h"

class BoxFilter : public Filter
{
public:
	real GetWeight1D(const real pos);
};