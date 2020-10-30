#include "Sampler.h"

class StratifiedSampler : public Sampler
{
public:
	StratifiedSampler(int x, int y);
	void CreateSamples(std::vector<Sample> &samples);
};