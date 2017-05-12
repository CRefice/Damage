#include <cstring>

#include "ppu.hpp"

namespace core
{
void PPU::advance(uint cycles) {
	advanceStatus(cycles);
	advanceDma(cycles);
}

void PPU::advanceStatus(uint cycles) {
	while (cycles--) {
		interruptTest();
		doneDrawing = false;
		if (status.ly < 144) {
			//Find out a more accurate way
			if (scanlineCounter <= 92) {
				//OAM Access mode
				status.lcdMode = 2;
				if (scanlineCounter == 0) {
					scanSprites();
				}
			}
			else if (scanlineCounter <= 252) {
				//VRAM Access mode
				status.lcdMode = 3;
				drawPixel(lastPx++);
			}
			else if (scanlineCounter <= 456) {
				//Enter HBlank
				status.lcdMode = 0;
				lastPx = 0;
			}
			else {
				//Exit HBlank, start new line
				scanlineCounter = 0;
				status.ly++;
				//Don't increment counter
				continue;
			}
		}
		else if (status.ly < 154) {
			//Enter VBlank
			status.lcdMode = 1;

			if (scanlineCounter > 456) {
				//Increase VBlank "line".
				scanlineCounter = 0;
				if (status.ly++ == 144) {
					if(status.lcdEnable) irq.raise(Interrupt::VBlank);
					doneDrawing = true;
					return;
				}
				continue;
			}
		}
		else {
			//Exit VBlank
			status.ly = 0;
		}
		scanlineCounter++;
	}
}

void PPU::interruptTest() {
	bool curIrq = status.lastIrq;

	status.lastIrq  = status.interruptHblank && status.lcdMode == 0;
	status.lastIrq |= status.interruptVblank && status.lcdMode == 1;
	status.lastIrq |= status.interruptOam    && status.lcdMode == 2;
	status.lastIrq |= status.interruptLyc && status.ly == status.lyc;

	if (!curIrq && status.lastIrq) {
		irq.raise(Interrupt::LCD);
	}
}

uint16 PPU::tileRow(bool select, uint x, uint y) {
	uint16 mapAddr, dataAddr;

	mapAddr = 0x1800 + (select << 10);
	mapAddr += (((y >> 3) << 5) + (x >> 3)) & 0x03FF;

	if (status.bgDataSelect == 0) {
		dataAddr = 0x1000 + ((int8)vram[mapAddr] << 4);
	}
	else {
		dataAddr = 0x0000 + (vram[mapAddr] << 4);
	}

	dataAddr += ((y & 7) << 1);

	return (vram[dataAddr]) | (vram[dataAddr + 1] << 8);
}

uint16 PPU::hflip(uint16 data) {
	return (data & 0x8080) >> 7 | (data & 0x4040) >> 5
	     | (data & 0x2020) >> 3 | (data & 0x1010) >> 1
	     | (data & 0x0808) << 1 | (data & 0x0404) << 3
	     | (data & 0x0202) << 5 | (data & 0x0101) << 7;
}

void PPU::power() {
	for (uint n = 0x8000; n < 0xA000; ++n) bus->map(n, this); //Vram
	for (uint n = 0xFE00; n < 0xFEA0; ++n) bus->map(n, this); //Oam

	bus->map(0xFF40, this);
	bus->map(0xFF41, this);
	bus->map(0xFF42, this);
	bus->map(0xFF43, this);
	bus->map(0xFF44, this);
	bus->map(0xFF45, this);
	bus->map(0xFF46, this); //DMA start address
	bus->map(0xFF47, this);
	bus->map(0xFF48, this);
	bus->map(0xFF49, this);
	bus->map(0xFF4A, this);
	bus->map(0xFF4B, this);

	for (auto& n : vram) n = 0;
	for (auto& n : oam) n = 0;
	for (auto& n : bgp) n = 0;
	for (auto& n : obp[0]) n = 3;
	for (auto& n : obp[1]) n = 3;

	memset(&status, 0, sizeof(status));

	doneDrawing = false;
	scanlineCounter = 0;
	lastPx = 0;

	for (auto& s : sprites) s = {};
	spriteCount = 0;

	bg.color = 0;
	ob.color = 0;
	bg.palette = 0;
	ob.palette = 0;
	bg.priority = false;
	ob.priority = false;

	background.data = 0;
	window.data = 0;

	for (auto& p : screen) p = 0;
}
}
