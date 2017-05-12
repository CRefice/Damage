#pragma once

#include "system.hpp"

namespace core
{
void System::power() {
	bus.power();
	cartridge.connect(&bus);
	apu.connect(&bus);
	cpu.connect(&bus);
	ppu.connect(&bus);

	cpu.irq.connect(&cpu);
	ppu.irq.connect(&cpu);

	cartridge.power();
	apu.power();
	cpu.power();
	ppu.power();
}

void System::runStep() {
	cpu.run();
	cartridge.advance(cpu.clock());
	ppu.advance(cpu.clock());
	apu.advance(cpu.clock());
}

void System::runFrame() {
	//Cycles per frame = Frequency / Framerate
	//static const uint frameCycles = (1024 * 1024 * 4) / 60;

	//uint cycles = 0;
	do {
		//CPU sets the pace for the other components
		cpu.run();
		cartridge.advance(cpu.clock());
		ppu.advance(cpu.clock());
		apu.advance(cpu.clock());
		//cycles += cpu.clock();
	} while (!ppu.frameCompleted());
}
}
