#include "ppu.hpp"

namespace core
{
void PPU::drawPixel(uint px) {
	uint2 color = 0;
	bg.color = 0;
	bg.palette = 0;

	ob.color = 0;
	ob.palette = 0;

	if (status.lcdEnable) {
		if (status.bgEnable) drawBg(px);
		if (status.wndEnable) drawWnd(px);
		if (status.obEnable) drawObj(px);

		if (ob.palette == 0) color = bg.color;
		else if (bg.palette == 0) color = ob.color;
		else if (ob.priority) color = ob.color;
		else color = bg.color;
	}

	screen[status.ly * 160 + px] = color;
}

void PPU::drawBg(uint px) {
	uint scrollx = (px + status.scx) & 0xFF;
	uint scrolly = (status.ly + status.scy) & 0xFF;
	uint tilex = scrollx & 7;

	if(tilex == 0 || px == 0) {
		background.data = tileRow(status.bgTilemapSelect, scrollx, scrolly);
	}

	uint index = (background.data & (0x0080 >> tilex)) ? 1 : 0;
	index |= (background.data & (0x8000 >> tilex)) ? 2 : 0;

	bg.color = bgp[index];
	bg.palette = index;
}

void PPU::drawWnd(uint px) {
	uint scrollx = px + 7 - status.wx;
	uint scrolly = status.ly - status.wy;

	if (scrolly >= 144u || scrollx >= 160u) return;

	uint tilex = scrollx & 7;
	if (tilex == 0 || px == 0)
		window.data = tileRow(status.wndTilemapSelect, scrollx, scrolly);

	uint index = (window.data & (0x0080 >> tilex)) ? 1 : 0;
	index |= (window.data & (0x8000 >> tilex)) ? 2 : 0;

	bg.color = bgp[index];
	bg.palette = index;
}

void PPU::drawObj(uint px) {
	for (int n = spriteCount - 1; n >= 0; n--) {
		Sprite& s = sprites[n];
		int tilex = px - s.x;

		if (tilex < 0 || tilex > 7) continue;

		uint index = (s.data & (0x0080 >> tilex)) ? 1 : 0;
		index |= (s.data & (0x8000 >> tilex)) ? 2 : 0;
		if (index == 0) continue;

		ob.color = obp[s.attr.bit(4)][index];
		ob.palette = index;
		ob.priority = !(s.attr.bit(7));
	}
}

void PPU::scanSprites() {
	if (!status.lcdEnable) return;

	spriteCount = 0;
	const uint height = (status.obSizeSelect ? 16 : 8);

	for (int n = 0; n < 40 * 4; n += 4) {
		Sprite& s = sprites[spriteCount];
		s.y = oam[n + 0] - 16;
		s.x = oam[n + 1] - 8;
		s.tile = oam[n + 2] & ~(uint)status.obSizeSelect;
		s.attr = oam[n + 3];

		s.y = status.ly - s.y;
		if (s.y >= height) continue;
		if (s.attr.bit(6)) s.y ^= (height - 1);

		uint dataAddr = (s.tile << 4) + (s.y << 1);
		s.data = (vram[dataAddr]);
		s.data |= (vram[dataAddr + 1] << 8);

		if (s.attr.bit(5)) s.data = hflip(s.data);

		if (++spriteCount == 10) break;
	}

	for (uint lo = 0; lo < spriteCount; ++lo) {
		for (uint hi = lo + 1; hi < spriteCount; ++hi) {
			if (sprites[hi].x < sprites[lo].x) {
				Sprite temp = sprites[lo];
				sprites[lo] = sprites[hi];
				sprites[hi] = temp;
			}
		}
	}
}
}
