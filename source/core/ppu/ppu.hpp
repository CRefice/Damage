#pragma once

#include "core\memory\memory.hpp"
#include "core\interrupts.hpp"

namespace core
{
class PPU : public MMIO
{
public:
	void power();
	void advance(uint cycles);

	bool frameCompleted() const { return doneDrawing && scanlineCounter == 0; }
	const uint16* buffer() const { return screen; }

	InterruptRequester irq;

private:
	void advanceStatus(uint cycles);
	void interruptTest();
	uint16 tileRow(bool select, uint x, uint y);
	uint16 hflip(uint16 data);

	//dmg.cpp
	void drawPixel(uint px);
	void drawBg(uint px);
	void drawWnd(uint px);
	void drawObj(uint px);
	void scanSprites();

	//io.cpp
	uint8 readIO(uint16 addr) final;
	void writeIO(uint16 addr, uint8 val) final;

	//dma.cpp
	void advanceDma(uint cycles);

	uint8 vram[0x2000];
	uint8 oam[0xA0];
	uint2 bgp[4];
	uint2 obp[2][4];

	uint scanlineCounter;
	uint lastPx;
	bool doneDrawing;

	struct Status
	{
		bool lastIrq;

		//FF40 - LCD Control Register
		bool lcdEnable;
		bool wndTilemapSelect;
		bool wndEnable;
		bool bgDataSelect;
		bool bgTilemapSelect;
		bool obSizeSelect;
		bool obEnable;
		bool bgEnable;

		//FF41 - LCD status register
		bool interruptLyc;
		bool interruptOam;
		bool interruptVblank;
		bool interruptHblank;
		uint2 lcdMode;

		//FF42 - Background scroll register y
		uint8 scy;
		//FF43 - Background scroll register x
		uint8 scx;

		//FF44 - LCD scanline position
		uint8 ly;

		//FF45 - LCD scanline compare
		uint8 lyc;
		
		//FF46 - DMA start address
		bool dmaActive;
		uint dmaClock;
		uint8 dmaAddress;

		//FF4A - Window Y position
		uint8 wy;
		//FF4B - Window X position
		uint8 wx;
	} status;

	struct Sprite
	{
		uint8 x = 0;
		uint8 y = 0;
		uint8 attr = 0;
		uint8 tile = 0;
		uint16 data = 0;
	};

	Sprite sprites[10];
	uint spriteCount;

	struct Pixel
	{
		uint2 color;
		uint8 palette;
		bool priority;
	} bg, ob;

	struct Background
	{
		uint data;
	} background, window;

	uint16 screen[160 * 144];
};
}
