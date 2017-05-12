#include "mapper.hpp"

namespace core
{
void Mapper::connect(MemBlock* rom_, MemBlock* ram_) {
	rom = rom_;
	ram = ram_;
}

uint8 Mapper::readRom(uint addr) {
	if (addr >= rom->size) addr %= rom->size;
	return rom->data[addr];
}

uint8 Mapper::readRam(uint addr) {
	if (ram->size == 0) return 0xFF;
	if (addr >= ram->size) addr %= ram->size;
	return ram->data[addr & 0x1FFF];
}

void Mapper::writeRam(uint addr, uint8 val) {
	if (ram->size == 0) return;
	if (addr >= ram->size) addr %= ram->size;
	ram->data[addr & 0x1FFF] = val;
}
}
