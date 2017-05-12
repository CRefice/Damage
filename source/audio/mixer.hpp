#pragma once

#include "filter.hpp"
#include "resampler.hpp"

namespace audio
{
class SoundStream;

class Mixer
{
public:
	Mixer(SoundStream* stream) { reset(stream); }

	void reset(SoundStream* stream);
	void sample(double left, double right);

	bool pending() { return leftChannel.resampler.pending(); }
	void process();

private:
	SoundStream* soundStream;
	struct Channel
	{
		Filter iir[3];
		Resampler resampler;
	} leftChannel, rightChannel;
};
}
