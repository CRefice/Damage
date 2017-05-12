#include <cstring>

#include "square2.hpp"

namespace core
{
void Square2::run() {
	if (--counter == 0) {
		counter = (2048 - frequency) * 4;
		phase++;
		switch (duty) {
		case 0: dutyOutput = (phase == 6); break;
		case 1: dutyOutput = (phase >= 6); break;
		case 2: dutyOutput = (phase >= 4); break;
		case 3: dutyOutput = (phase <= 5); break;
		}
	}

	uint4 out = dutyOutput ? (uint)volume : 0;
	sample(enabled() ? out : 0);
}

void Square2::stepLength() {
	if (length.enable) {
		if (--length.counter == 0)
			disable();
	}
}

void Square2::stepEnvelope() {
	if (enabled() && envelope.frequency && --envelope.counter == 0) {
		envelope.counter = envelope.frequency ? (uint)envelope.frequency : 8;
		if (envelope.direction == 0 && volume > 0)  volume--;
		if (envelope.direction == 1 && volume < 15) volume++;
	}
}

void Square2::power(bool initializeLength) {
	disable();

	std::memset(&envelope, 0, sizeof(envelope));

	length.enable = false;
	if (initializeLength) length.counter = 64;	

	frequency = 0;
	counter = 0;
	phase = 0;

	duty = 0;
	dutyOutput = false;

	volume = 0;
	sample(0);
}

uint8 Square2::read(uint16 addr) {
	if (addr == 0xFF16) {
		return duty << 6
			   | 0x3F;
	}

	if (addr == 0xFF17) {
		return envelope.volume    << 4
			   | envelope.direction << 3
				 | envelope.frequency;
	}

	if (addr == 0xFF19) {
		return 0x80
			   | length.enable << 6
				 | 0x3F;
	}

	return 0xFF;
}

void Square2::write(uint16 addr, uint8 val) {
	if (addr == 0xFF16) {
		duty = val.bits(7, 6);
		length.counter = 64 - val.bits(5, 0);
		return;
	}

	if (addr == 0xFF17) {
		envelope.volume    = val.bits(7, 4);
		envelope.direction = val.bit(3);
		envelope.frequency = val.bits(2, 0);
		if (!(envelope.volume || envelope.direction)) disable();
		return;
	}

	if (addr == 0xFF18) {
		frequency.bits(7, 0) = val;
		return;
	}

	if (addr == 0xFF19) {
		//Need a way to access APU phase
		if (apuPhase->bit(0)) {
			if (!length.enable && val.bit(6)) {
				if (--length.counter == 0) disable();
			}
		}
		length.enable = val.bit(6);
		frequency.bits(10, 8) = val.bits(2, 0);

		if (val.bit(7)) {
			setEnable(envelope.volume || envelope.direction);
			counter = (2048 - frequency) * 4;
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
