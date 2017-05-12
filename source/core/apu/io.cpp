#include <cstring>

#include "apu.hpp"

namespace core
{
uint8 APU::readIO(uint16 addr)
{
	if (addr >= 0xFF10 && addr <= 0xFF14) return square1.read(addr);
	if (addr >= 0xFF15 && addr <= 0xFF19) return square2.read(addr);
	if (addr >= 0xFF1A && addr <= 0xFF1E) return wave.read(addr);
	if (addr >= 0xFF1F && addr <= 0xFF23) return noise.read(addr);
	if (addr >= 0xFF30 && addr <= 0xFF3F) return wave.read(addr);

	if (addr == 0xFF24) {
		return status.leftEnable << 7
		     | status.leftVolume << 4
		     | status.rightEnable << 3
		     | status.rightVolume;
	}
	
	if (addr == 0xFF25) {
		return status.noise.leftEnable << 7
		     | status.wave.leftEnable << 6
			   | status.square2.leftEnable << 5
			   | status.square1.leftEnable << 4
			   | status.noise.rightEnable << 3
			   | status.wave.rightEnable << 2
			   | status.square2.rightEnable << 1
			   | status.square1.rightEnable;
	}
		
	if (addr == 0xFF26) {
		return status.enable << 7 | 0x70
		     | noise.enabled() << 3
			   | wave.enabled() << 2
			   | square2.enabled() << 1
			   | square1.enabled() << 0;
	}

	return 0xFF;
}

void APU::writeIO(uint16 addr, uint8 val)
{
	if (!status.enable) {
		bool valid = addr == 0xFF26;
		if (addr == 0xFF11) { valid = true; val &= 0x3F; }
		if (addr == 0xFF16) { valid = true; val &= 0x3F; }
		if (addr == 0xFF1B) { valid = true; }
		if (addr == 0xFF20) { valid = true; }
		if (!valid) return;
	}

	if (addr >= 0xFF10 && addr <= 0xFF14) return square1.write(addr, val);
	if (addr >= 0xFF15 && addr <= 0xFF19) return square2.write(addr, val);
	if (addr >= 0xFF1A && addr <= 0xFF1E) return wave.write(addr, val);
	if (addr >= 0xFF1F && addr <= 0xFF23) return noise.write(addr, val);
	if (addr >= 0xFF30 && addr <= 0xFF3F) return wave.write(addr, val);

	if (addr == 0xFF24) {
		status.leftEnable  = val.bit(7);
		status.leftVolume  = val.bits(6,4);
		status.rightEnable = val.bit(3);
		status.rightVolume = val.bits(2,0);
		return;
	}

	if (addr == 0xFF25) {
		status.noise.leftEnable    = val.bit(7);
		status.wave.leftEnable     = val.bit(6);
		status.square2.leftEnable  = val.bit(5);
		status.square1.leftEnable  = val.bit(4);
		status.noise.rightEnable   = val.bit(3);
		status.wave.rightEnable    = val.bit(2);
		status.square2.rightEnable = val.bit(1);
		status.square1.rightEnable = val.bit(0);
		return;
	}

	if (addr == 0xFF26) {
		if (status.enable != val.bit(7)) {
			status.enable = val.bit(7);
			if (!status.enable) {
				square1.power(false);
				square2.power(false);
				wave.power(false);
				noise.power(false);
				std::memset(&status, 0, sizeof(status));
			}
			else {
				phase = 0;
			}
		}
		return;
	}

}
}
