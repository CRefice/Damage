#pragma once

#include "common\inttypes.hpp"

namespace core
{
class SoundChannel
{
public:
	void setApuPhase(const uint3& ref) { apuPhase = &ref; }

	int16 output() { return _output; }
	bool enabled() const { return _enable; }

protected:
	void enable() { _enable = true; }
	void disable() { _enable = false; }
	void setEnable(bool enable) { _enable = enable; }
	void sample(int16 bite) { _output = bite; }

	const uint3* apuPhase;

private:
	int16 _output;
	bool _enable;
};

//Sound modulators
struct Length
{
	int counter;
	bool enable;
};

struct Envelope
{
	uint4 volume;
	bool direction;
	uint3 frequency;
	int counter;
};

struct Sweep
{
	bool enable;
	bool direction;
	bool negate;
	uint3 shift;
	int shadow;
	uint3 frequency;
	int counter;
};
}
