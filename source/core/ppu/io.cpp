#include <cstring>

#include "ppu.hpp"

namespace core
{
uint8 PPU::readIO(uint16 addr) {
	if (addr >= 0x8000 && addr < 0xA000) {
		return vram[addr & 0x1FFF];
	}
	if (addr >= 0xFE00 && addr < 0xFEA0) {
		if (status.dmaActive && status.dmaClock >= 8) return 0xFF;
		return oam[addr & 0xFF];
	}

	//LCD Control register
	if (addr == 0xFF40) {
		return status.lcdEnable << 7
		     | status.wndTilemapSelect << 6
			   | status.wndEnable << 5
			   | status.bgDataSelect << 4
			   | status.bgTilemapSelect << 3
			   | status.obSizeSelect << 2
			   | status.obEnable << 1
			   | status.bgEnable << 0;
	}

	//LCD Status register
	if (addr == 0xFF41) {
		return status.interruptLyc << 6
		     | status.interruptOam << 5
			   | status.interruptVblank << 4
			   | status.interruptHblank << 3
			   | (status.ly == status.lyc) << 2
			   | status.lcdMode;
	}

	//Background scroll register Y
	if (addr == 0xFF42) {
		return status.scy;
	}

	//Background scroll register X
	if (addr == 0xFF43) {
		return status.scx;
	}

	//Current scanline position
	if (addr == 0xFF44) {
		return status.ly;
	}

	//Scanline Compare
	if (addr == 0xFF45) {
		return status.lyc;
	}

	//DMA start address
	if (addr == 0xFF46) {
		return status.dmaAddress;
	}

	//Background palette data
	if (addr == 0xFF47) {
		return bgp[3] << 6
		     | bgp[2] << 4
			   | bgp[1] << 2
			   | bgp[0] << 0;
	}

	//Object palette data 0
	if (addr == 0xFF48) {
		return obp[0][3] << 6
		     | obp[0][2] << 4
			   | obp[0][1] << 2
			   | obp[0][0] << 0;
	}

	//Object palette data 1
	if (addr == 0xFF49) {
		return obp[1][3] << 6
		     | obp[1][2] << 4
			   | obp[1][1] << 2
			   | obp[1][0] << 0;
	}

	//Window sc register Y
	if (addr == 0xFF4A) {
		return status.wy;
	}

	//Window sc register X
	if (addr == 0xFF4B) {
		return status.wx;
	}

	return 0xFF;
}

void PPU::writeIO(uint16 addr, uint8 val) {
	//VRAM
	if (addr >= 0x8000 && addr < 0xA000) {
		vram[addr & 0x1FFF] = val;
		return;
	}

	//OAM
	if (addr >= 0xFE00 && addr < 0xFEA0) {
		if (status.dmaActive && status.dmaClock >= 8) return;
		oam[addr & 0xFF] = val;
		return;
	}

	//LCD Control register
	if (addr == 0xFF40) {
		if(!status.lcdEnable && val.bit(7)) status.ly = 0;

		status.lcdEnable		= val.bit(7);
		status.wndTilemapSelect = val.bit(6);
		status.wndEnable		= val.bit(5);
		status.bgDataSelect		= val.bit(4);
		status.bgTilemapSelect	= val.bit(3);
		status.obSizeSelect		= val.bit(2);
		status.obEnable			= val.bit(1);
		status.bgEnable			= val.bit(0);
		return;
	}

	//LCD Status register
	if (addr == 0xFF41) {
		status.interruptLyc	   = val.bit(6);
		status.interruptOam	   = val.bit(5);
		status.interruptVblank = val.bit(4);
		status.interruptHblank = val.bit(3);
		return;
	}

	//sc register Y
	if (addr == 0xFF42) {
		status.scy = val;
		return;
	}

	//Background sc register X
	if (addr == 0xFF43) {
		status.scx = val;
		return;
	}

	if (addr == 0xFF44) {
		status.ly = 0;
		return;
	}

	//Background scanline compare
	if (addr == 0xFF45) {
		status.lyc = val;
		return;
	}

	//DMA start address
	if (addr == 0xFF46) {
		status.dmaAddress = val;
		status.dmaActive = true;
		status.dmaClock = 0;
		return;
	}

	//Background palette
	if (addr == 0xFF47) {
		bgp[3] = val >> 6;
		bgp[2] = val >> 4;
		bgp[1] = val >> 2;
		bgp[0] = val >> 0;
		return;
	}

	//Sprite palette 0
	if (addr == 0xFF48) {
		obp[0][3] = val >> 6;
		obp[0][2] = val >> 4;
		obp[0][1] = val >> 2;
		obp[0][0] = val >> 0;
		return;
	}

	//Sprite palette 1
	if (addr == 0xFF49) {
		obp[1][3] = val >> 6;
		obp[1][2] = val >> 4;
		obp[1][1] = val >> 2;
		obp[1][0] = val >> 0;
		return;
	}

	//Window sc register y
	if (addr == 0xFF4A) {
		status.wy = val;
		return;
	}

	//Window sc register x
	if (addr == 0xFF4B) {
		status.wx = val;
		return;
	}
}
}
