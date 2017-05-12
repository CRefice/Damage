#pragma once

#include "ppu.hpp"

namespace core
{
void PPU::advanceDma(uint cycles) {
	if (status.dmaActive) {
		const uint lastPos = status.dmaClock >> 2;
		uint curPos = (status.dmaClock += cycles) >> 2;
		
		if (curPos >= 161) {
			status.dmaActive = false;
			curPos = 161;
		}

		if (lastPos == 0) {
			//DMA warm-up, no transfer is completed
			status.dmaClock = 4;
			return;
		}
		if (lastPos != 0) {
			for (uint n = lastPos; n < curPos; ++n) {
				oam[n - 1] = bus->read((status.dmaAddress << 8) | n - 1);
			}
		}
	}
}
}
