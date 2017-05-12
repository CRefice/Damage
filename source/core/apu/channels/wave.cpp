#include <cstring>

#include "wave.hpp"

namespace core
{
void Wave::run() {
	if (patternHold) patternHold = false;
	if (--counter == 0) {
		counter = (2048 - frequency) * 2;
		patternSample = getPattern(++patternOffset);
		patternHold = true;
	}

	static const uint shift[] = { 4, 0, 1, 2 };  //0%, 100%, 50%, 25%
	uint4 out = patternSample >> shift[volume];
	sample(enabled() ? out : 0);
}

uint4 Wave::getPattern(uint5 offset) const {
	return pattern[offset >> 1] >> (offset & 1 ? 0 : 4);
}

void Wave::stepLength() {
	if (length.enable) {
		if (--length.counter == 0) disable();
	}
}

void Wave::power(bool initializeLength)
{
	disable();

	dacEnable = false;

	if (initializeLength) length.counter = 256;
	length.enable = false;

	frequency = 0;
	counter = 0;

	patternOffset = 0;
	patternSample = 0;
	patternHold = false;

	volume = 0;

	sample(0);
}

uint8 Wave::read(uint16 addr) {
	if (addr == 0xFF1A) {
		return dacEnable << 7
			   | 0x7F;
	}

	if (addr == 0xFF1C) {
		return 0x80
		       | volume << 5
		       | 0x1F;
	}

	if (addr == 0xFF1E) {
		return 0x80
		     | length.enable << 6
		     | 0x3F;
	}

	if (addr >= 0xFF30 && addr <= 0xFF3F) {
		if (enabled()) {
			if (!patternHold) return 0xFF;
			return pattern[patternOffset >> 1];
		}
		return pattern[addr & 15];
	}
	return 0xFF;
}

void Wave::write(uint16 addr, uint8 val) {
	if (addr == 0xFF1A) {
		dacEnable = val.bit(7);
		if (!dacEnable) disable();
		return;
	}

	if (addr == 0xFF1B) {
		length.counter = 256 - val;
		return;
	}

	if (addr == 0xFF1C) {
		volume = val.bits(6,5);
		return;
	}

	if (addr == 0xFF1D) {
		frequency.bits(7,0) = val;
		return;
	}

	if (addr == 0xFF1E) {
		if(apuPhase->bit(0)) {
			if (!length.enable && val.bit(6)) {
				if (--length.counter == 0) disable();
			}
		}

		length.enable = val.bit(6);
		frequency.bits(10,8) = val.bits(2,0);

		if (val.bit(7)) {
			if (patternHold) { //Corrupt wave ram
				if ((patternOffset >> 1) <= 3) {
					pattern[0] = pattern[patternOffset >> 1];
				}
				else {
					pattern[0] = pattern[((patternOffset >> 1) & ~3) + 0];
					pattern[1] = pattern[((patternOffset >> 1) & ~3) + 1];
					pattern[2] = pattern[((patternOffset >> 1) & ~3) + 2];
					pattern[3] = pattern[((patternOffset >> 1) & ~3) + 3];
				}
			}

			setEnable(dacEnable);
			counter = (2048 - frequency) * 2;
			patternHold = false;
			patternOffset = 0;
			patternSample = 0;

			if (!length.counter) {
				length.counter = 256;
				if (apuPhase->bit(0)) {
					if (length.enable) length.counter--;
				}
			}
		}
		return;
	}

	if (addr >= 0xFF30 && addr <= 0xFF3F) {
		if (enabled()) {
			if (!patternHold) return;
			pattern[patternOffset >> 1] = val;
			return;
		}
		pattern[addr & 15] = val;
	}
}
}
