#include <cstring>

#include "noise.hpp"

namespace core
{
uint Noise::getCounter() const {
	static const uint table[] = { 4, 8, 16, 24, 32, 40, 48, 56 };
	return table[ratio] << frequency;
}

void Noise::run() {
	if (--counter == 0) {
		counter = getCounter();
		if (frequency < 14) {
			bool bit = (lfsr ^ (lfsr >> 1)) & 1;
			lfsr = (lfsr >> 1) ^ (bit << (widthSelect ? 6 : 14));
		}
	}

	uint4 out = lfsr & 1 ? 0 : (uint)volume;
	sample(enabled() ? out : 0);
}

void Noise::stepLength() {
	if (length.enable) {
		if (--length.counter == 0) disable();
	}
}

void Noise::stepEnvelope() {
	if (enabled() && envelope.frequency && --envelope.counter == 0) {
		envelope.counter = envelope.frequency ? (uint)envelope.frequency : 8;
		if (envelope.direction == 0 && volume >  0) volume--;
		if (envelope.direction == 1 && volume < 15) volume++;
	}
}

void Noise::power(bool initializeLength) {
	disable();

	std::memset(&envelope, 0, sizeof(envelope));

	frequency = 0;
	counter = 0;

	widthSelect = false;
	ratio = 0;

	if (initializeLength) length.counter = 64;
	length.enable = false;

	volume = 0;
	lfsr = 0;

	sample(0);
}

uint8 Noise::read(uint16 addr) {
	if (addr == 0xFF21) {
		return envelope.volume << 4
			   | envelope.direction << 3
				 | envelope.frequency;
	}

	if (addr == 0xFF22) {
		return frequency << 4
			   | widthSelect << 3
				 | ratio;
	}

	if (addr == 0xFF23) {
		return 0x80
			   | length.enable << 6
				 | 0x3F;
	}

	return 0xFF;
}

void Noise::write(uint16 addr, uint8 val) {
	if (addr == 0xFF20) {
		length.counter = 64 - (val & 0x3F);
		return;
	}

	if (addr == 0xFF21) {
		envelope.volume = val.bits(7,4);
		envelope.direction = val.bit(3);
		envelope.frequency = val.bits(2,0);
		if (!(envelope.volume || envelope.direction)) disable();
		return;
	}

	if (addr == 0xFF22) {
		frequency = val.bits(7,4);
		widthSelect = val.bit(3);
		ratio = val.bits(2,0);
		counter = getCounter();
		return;
	}

	if (addr == 0xFF23) {
		if (apuPhase->bit(0)) {
			if (!length.enable && val.bit(6)) {
				if (--length.counter == 0) disable();
			}
		}
		length.enable = val.bit(6);

		if (val.bit(7)) {
			SoundChannel::setEnable(envelope.volume || envelope.direction);
			lfsr = -1;
			envelope.counter = envelope.frequency ? (uint)envelope.frequency : 8;
			volume = envelope.volume;

			if (!length.counter) {
				length.counter = 64;
				if (apuPhase->bit(0)) {
					if (length.enable) length.counter--;
				}
			}
		}
		return;
	}
}
}
