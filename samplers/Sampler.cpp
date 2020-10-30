#include "Sampler.h"

Sampler::Sampler(int x_, int y_)
	:x(x_), y(y_)
{
	dx = real(1) / x_;
	dy = real(1) / y_;
}