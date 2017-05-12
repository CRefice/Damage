#include "mbc0.hpp"

namespace core
{
void MBC0::power() {}

uint8 MBC0::read(uint16 addr) {
	if (addr < 0x8000) return readRom(addr);
	if (addr >= 0xA000 && addr < 0xC000) return readRam(addr & 0x1FFF);
	return 0xFF;
}

void MBC0::write(uint16 addr, uint8 val) {
	if (addr >= 0xA000 && addr < 0xC000) return writeRam(addr & 0x1FFF, val);
}
}
