#pragma once

#include "input\control-emu.hpp"

#include "core\apu\apu.hpp"
#include "core\cpu\cpu.hpp"
#include "core\ppu\ppu.hpp"
#include "core\cartridge\cartridge.hpp"

namespace core
{
class System
{
public:
	void power();
	void runStep();
	void runFrame();

	void load(const std::string& path) { cartridge.load(path); }
	void save(const std::string& path) { cartridge.save(path); }
	void unload() { cartridge.unload(); }

	void setControls(input::EmulatedControls* controls) {
		cpu.setControls(controls);
	}

	const uint16* screenBuffer() const { return ppu.buffer(); }

private:
	Bus bus;
	Cartridge cartridge;
	APU apu;
	PPU ppu;
	CPU cpu;
};
}
