#include "common\inttypes.hpp"

#include "sound-stream.hpp"
#include "mixer.hpp"

namespace audio
{
const double inputFrequency = 4 * 1024 * 1024;
const double outputFrequency = 48000;

void Mixer::reset(SoundStream* stream) {
	soundStream = stream;
	leftChannel.resampler.reset(inputFrequency, outputFrequency);
	rightChannel.resampler.reset(inputFrequency, outputFrequency);

	for (uint n = 0; n < 3; ++n) {
		const double q = Filter::butterworth(6, n);
		leftChannel.iir[n].reset(20000.0 / inputFrequency, q);
		rightChannel.iir[n].reset(20000.0 / inputFrequency, q);
	}
}

void Mixer::sample(double left, double right) {
	left += 1e-25;
	right += 1e-25;

	for (auto& iir : leftChannel.iir) left = iir.process(left);
	leftChannel.resampler.write(left);

	for (auto& iir : rightChannel.iir) right = iir.process(right);
	rightChannel.resampler.write(right);

	process();
}

void Mixer::process() {
	while (pending()) {
		double leftSample = leftChannel.resampler.read();
		double rightSample = rightChannel.resampler.read();

		int16_t left = sclamp<16>(leftSample * 32768.0);
		int16_t right = sclamp<16>(rightSample * 32768.0);
		soundStream->sample(left, right);
	}
}
}