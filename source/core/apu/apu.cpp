#include <cstring>

#include "audio\sound-stream.hpp"

#include "apu.hpp"

namespace core
{
void APU::advance(uint cycles) {
	while(cycles--) {
		square1.run();
		square2.run();
		wave.run();
		noise.run();

		mixOutput();
		hipass(leftOutput, leftBias);
		hipass(rightOutput, rightBias);
		hipass(centerOutput, centerBias);

		audio::sample(leftOutput, rightOutput);

		//512hz
		if (cycle == 0) {
			if (phase == 0 || phase == 2 || phase == 4 || phase == 6) {
				square1.stepLength();
				square2.stepLength();
				wave.stepLength();
				noise.stepLength();
			}
			if (phase == 2 || phase == 6) {
				square1.stepSweep();
			}
			if (phase == 7) {
				square1.stepEnvelope();
				square2.stepEnvelope();
				noise.stepEnvelope();
			}
			phase++;
		}
		cycle++;
	}
}

void APU::mixOutput() {
	if (!status.enable) {
		centerOutput = 0;
		leftOutput = 0;
		rightOutput = 0;

		centerBias = leftBias = rightBias = 0;
		return;
	}

	int sample = 0;
	sample += square1.output();
	sample += square2.output();
	sample += wave.output();
	sample += noise.output();
	centerOutput = (sample * 512) - 16384;

	sample = 0;
	if (status.square1.leftEnable) sample += square1.output();
	if (status.square2.leftEnable) sample += square2.output();
	if (status.wave.leftEnable) sample += wave.output();
	if (status.noise.leftEnable) sample += noise.output();
	sample = (sample * 512) - 16384;
	sample = (sample * (status.leftVolume + 1)) / 8;
	leftOutput = sample;

	sample = 0;
	if (status.square1.rightEnable) sample += square1.output();
	if (status.square2.rightEnable) sample += square2.output();
	if (status.wave.rightEnable) sample += wave.output();
	if (status.noise.rightEnable) sample += noise.output();
	sample = (sample * 512) - 16384;
	sample = (sample * (status.rightVolume + 1)) / 8;
	rightOutput = sample;

	//Decrease volume
	centerOutput >>= 1;
	rightOutput >>= 1;
	leftOutput >>= 1;
}

void APU::hipass(int16 & sample, int64 & bias) {
	bias += ((((int64)sample << 16) - (bias >> 16)) * 57593) >> 16;
	sample = sclamp<16>(sample - (bias >> 32));
}

void APU::power() {
	for (uint n = 0xFF10; n < 0xFF40; ++n) bus->map(n, this);

	centerOutput = 0;
	leftOutput = 0;
	rightOutput = 0;
	centerBias = 0;
	leftBias = 0;
	rightBias = 0;

	cycle = 0;
	phase = 0;

	std::memset(&status, 0, sizeof(status));

	square1.setApuPhase(phase);
	square2.setApuPhase(phase);
	wave.setApuPhase(phase);
	noise.setApuPhase(phase);

	square1.power();
	square2.power();
	wave.power();
	wave.resetWave();
	noise.power();
}
}
