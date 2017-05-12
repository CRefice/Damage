#include <cstring>

#include "square1.hpp"

namespace core
{
void Square1::run() {
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

void Square1::updateSweep(bool save) {
	if (!sweep.enable) return;

	sweep.negate = sweep.direction;
	uint delta = sweep.shadow >> sweep.shift;
	int freq = sweep.shadow + (sweep.negate ? -delta : delta);

	if (freq > 2047) {
		disable();
	}
	else if (sweep.shift && save) {
		sweep.shadow = freq;
		frequency = freq & 2047;
		counter = (2048 - frequency) * 4;
	}
}

void Square1::stepLength() {
	if (length.enable) {
		if (--length.counter == 0)
			disable();
	}
}

void Square1::stepSweep() {
	if (--sweep.counter == 0) {
		sweep.counter = sweep.frequency ? (uint)sweep.frequency : 8;
		if (sweep.enable && sweep.frequency) {
			updateSweep(true);
			updateSweep(false);
		}
	}
}

void Square1::stepEnvelope() {
	if (enabled() && envelope.frequency && --envelope.counter == 0) {
		envelope.counter = envelope.frequency ? (uint)envelope.frequency : 8;
		if (envelope.direction == 0 && volume > 0)  volume--;
		if (envelope.direction == 1 && volume < 15) volume++;
	}
}

void Square1::power(bool initializeLength) {
	disable();

	std::memset(&envelope, 0, sizeof(envelope));
	std::memset(&sweep, 0, sizeof(sweep));

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

uint8 Square1::read(uint16 addr) {
	if (addr == 0xFF10) {
		return 0x80
			   | sweep.frequency << 4
				 | sweep.direction << 3
				 | sweep.shift;
	}

	if (addr == 0xFF11) {
		return duty << 6
			   | 0x3F;
	}

	if (addr == 0xFF12) {
		return envelope.volume    << 4
			   | envelope.direction << 3
				 | envelope.frequency;
	}

	if (addr == 0xFF14) {
		return 0x80
			   | length.enable << 6
				 | 0x3F;
	}

	return 0xFF;
}

void Square1::write(uint16 addr, uint8 val) {
	if (addr == 0xFF10) {
		if (sweep.enable && sweep.negate && !val.bit(3)) disable();
		sweep.frequency = val.bits(6, 4);
		sweep.direction = val.bit(3);
		sweep.shift     = val.bits(2, 0);
		return;
	}

	if (addr == 0xFF11) {
		duty = val.bits(7, 6);
		length.counter = 64 - val.bits(5, 0);
		return;
	}

	if (addr == 0xFF12) {
		envelope.volume    = val.bits(7, 4);
		envelope.direction = val.bit(3);
		envelope.frequency = val.bits(2, 0);
		if (!(envelope.volume || envelope.direction)) disable();
		return;
	}

	if (addr == 0xFF13) {
		frequency.bits(7, 0) = val;
		return;
	}

	if (addr == 0xFF14) {
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

			sweep.shadow = frequency;
			sweep.negate = false;
			sweep.counter = sweep.frequency ? (uint)sweep.frequency : 8;
			sweep.enable = sweep.counter || sweep.shift;
			if (sweep.shift) updateSweep(false);
		}
		return;
	}
}
}
