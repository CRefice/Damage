#pragma once

#include <cstdint>
#include <memory>

#include "mixer.hpp"

namespace audio
{
void init();
void term();
void sample(int16_t left, int16_t right);

class SoundStream
{
public:
	virtual ~SoundStream() {}

	virtual void init() = 0;
	virtual void power() = 0;
	virtual void term() = 0;

	Mixer& mixer() { return mixer_; }

	virtual void play() = 0;
	virtual void stop() = 0;

	virtual void sample(int16_t left, int16_t right) = 0;

private:
	Mixer mixer_{this};
};

extern std::unique_ptr<SoundStream> stream;
}
