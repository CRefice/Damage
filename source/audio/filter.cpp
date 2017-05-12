#include "filter.hpp"

#include <cmath>

double pi() { return std::atan(1) * 4; }

namespace audio
{
void Filter::reset(double cutoff_, double quality_) {
	cutoff = cutoff_;
	quality = quality_;

	z1 = 0.0;
	z2 = 0.0;

	double v = pow(10, fabs(0) / 20.0);
	double k = tan(pi() * cutoff);
	double q = quality;
	double n = 0.0;

	n = 1 / (1 + k / q + k * k);
	a0 = k * k * n;
	a1 = 2 * a0;
	a2 = a0;
	b1 = 2 * (k * k - 1) * n;
	b2 = (1 - k / q + k * k) * n;
}

double Filter::process(double in) {
	double out = in * a0 + z1;
	z1 = in * a1 + z2 - b1 * out;
	z2 = in * a2 - b2 * out;
	return out;
}

double Filter::butterworth(unsigned int order, unsigned int phase) {
	return -0.5 / cos(pi() * (phase + order + 0.5) / order);
}
}