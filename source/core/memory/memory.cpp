#include "memory.hpp"

namespace core
{
static class Unmapped : public MMIO
{
public:
	uint8 readIO(uint16 addr) final { return 0xFF; }
	void writeIO(uint16 addr, uint8 val) final {}
} unmapped;

void Bus::power() {
	for (uint n = 0; n < 0x10000; ++n) map(n, &unmapped);
}

void Bus::map(uint16 addr, MMIO* device) {
	mmio[addr] = device;
}

uint8 Bus::read(uint16 addr) {
	return mmio[addr]->readIO(addr);
}

void Bus::write(uint16 addr, uint8 val) {
	mmio[addr]->writeIO(addr, val);
}
}
