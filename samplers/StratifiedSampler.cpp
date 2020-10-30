#include "StratifiedSampler.h"

StratifiedSampler::StratifiedSampler(int x, int y)
	:Sampler(x, y)
{
}

void StratifiedSampler::CreateSamples(std::vector<Sample> &samples)
{

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0.0, 1.0);

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			real s1 = rng(gen);
			real s2 = rng(gen);

			Sample sample;
			sample.pos.x = (i + s1) * dx;
			sample.pos.y = (j + s2) * dy;

			samples.push_back(sample);
		}
	}
}