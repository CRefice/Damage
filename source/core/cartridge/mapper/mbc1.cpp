#include "mbc1.hpp"

namespace core
{
void MBC1::power() {
	romBank = 1;
	ramBank = 0;
	ramEnable = false;
	mode = 0;
}

uint8 MBC1::read(uint16 addr) {
	if (addr < 0x4000) return readRom(addr);
	if (addr >= 0x4000 && addr < 0x8000) {
		if (mode == 0) {
			return readRom((ramBank << 19) | (romBank << 14) | (addr & 0x3FFF));
		}
		else {
			return readRom((romBank << 14) | (addr & 0x3FFF));
		}
	}
	if (addr >= 0xA000 && addr < 0xC000) {
		if (!ramEnable) return 0xFF;
		if (mode == 0) return readRam(addr & 0x1FFF);
		else return readRam((ramBank << 13) | (addr & 0x1FFF));
	}
	return 0xFF;
}

void MBC1::write(uint16 addr, uint8 val) {
	if (addr < 0x2000) {
		ramEnable = (val & 0x0F) == 0x0A;
		return;
	}
	if (addr >= 0x2000 && addr < 0x4000) {
		romBank = (val & 0x1F) + uint8((val & 0x1F) == 0);
		return;
	}
	if (addr >= 0x4000 && addr < 0x6000) {
		ramBank = (val & 0x03);
		return;
	}
	if (addr >= 0x6000 && addr < 0x8000) {
		mode = (val & 0x01);
		return;
	}
	if (addr >= 0xA000 && addr < 0xC000) {
		if (!ramEnable) return;
		if (mode == 0) return writeRam(addr & 0x1FFF, val);
		else return writeRam((ramBank << 13) | (addr & 0x1FFF), val);
	}
}
}
