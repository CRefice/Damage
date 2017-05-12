#pragma once

#include "common\util\ring.hpp"

class Resampler
{
public:
	void reset(double inputFrequency, double outputFrequency);
	void write(double sample);

	double read() { return samples.read(); }
	bool pending() { return samples.pending(); }

private:
	double inputFreq, outputFreq;
	double ratio;
	double fraction;
	double history[4];
	ring<double> samples;
};
