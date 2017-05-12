#include "mbc2.hpp"

namespace core
{
void MBC2::power() {
	romBank = 1;
	ramEnable = true;
}

uint8 MBC2::read(uint16 addr) {
	if (addr < 0x4000) return readRom(addr);
	if (addr >= 0x4000 && addr < 0x8000) {
		return readRom((romBank << 14) | (addr & 0x3FFF));
	}
	if (addr >= 0xA000 && addr < 0xC000) {
		if (!ramEnable) return 0xFF;
		return readRam(addr & 0x1FF);
	}
	return 0xFF;
}

void MBC2::write(uint16 addr, uint8 val) {
	if (addr < 0x2000) {
		if (addr.bit(8) == 0) {
			ramEnable = (val & 0x0F) == 0x0A;
		}
		return;
	}
	if (addr >= 0x2000 && addr < 0x4000) {
		if (addr.bit(8)) {
			romBank = (val & 0x0F) + uint8((val & 0x0F) == 0);
		}
		return;
	}
	if (addr >= 0xA000 && addr < 0xA1FF) {
		if (!ramEnable) return;
		return writeRam(addr & 0x1FFF, val);
	}
}
}
