#pragma once

#include "memory\memory.hpp"

#include "channels\square1.hpp"
#include "channels\square2.hpp"
#include "channels\wave.hpp"
#include "channels\noise.hpp"

namespace core
{
class APU : public MMIO
{
public:
	void power();
	void advance(uint cycles);

private:
	uint8 readIO(uint16 addr) final;
	void writeIO(uint16 addr, uint8 val) final;

	void mixOutput();
	void hipass(int16& sample, int64& bias);

	int16 centerOutput;
	int16 leftOutput;
	int16 rightOutput;
	int64 centerBias;
	int64 leftBias;
	int64 rightBias;

	uint12 cycle;
	uint3 phase;

	struct Status
	{
		bool enable;

		bool leftEnable;
		uint3 leftVolume;
		bool rightEnable;
		uint3 rightVolume;

		struct Channel
		{
			bool leftEnable;
			bool rightEnable;
		} square1, square2, wave, noise;
	} status;

	Square1 square1;
	Square2 square2;
	Wave wave;
	Noise noise;
};
}
