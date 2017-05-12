#pragma once

namespace audio
{
struct Filter
{
public:
	void reset(double cutoff_, double quality_);
	double process(double in);

	static double butterworth(unsigned int order, unsigned int phase);

private:
	double cutoff;
	double quality;
	double a0, a1, a2, b1, b2;
	double z1, z2;
};
}