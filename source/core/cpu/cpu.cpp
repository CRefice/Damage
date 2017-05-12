#include <cstring>

#include "cpu.hpp"

namespace core
{
void CPU::run() {
	cycles = 0;
	interruptTest();
	if(r.halt || r.stop) {
		cycle();
		return;
	}
	instruction();
}

void CPU::cycle() {
	if(r.ei) {
		r.ime = true;
		r.ei = false;
	}
	cycles += 4;
	advanceTimer(4);
}

uint8 CPU::read(uint16 addr) {
	cycle();
	return bus->read(addr);
}

void CPU::write(uint16 addr, uint8 val) {
	cycle();
	bus->write(addr, val);
}

void CPU::stackPush(uint16 val) {
	write(--r[SP], val >> 8);
	write(--r[SP], val);
}

uint16 CPU::stackPop() {
	const uint8 lo = read(r[SP]++);
	const uint8 hi = read(r[SP]++);
	return (hi << 8) | (lo << 0);
}

void CPU::power() {
	r[AF] = 0x0000;
	r[BC] = 0x0000;
	r[DE] = 0x0000;
	r[HL] = 0x0000;
	r[SP] = 0x0000;
	r[PC] = 0x0000;

	for(auto& n : wram) n = 0;
	for(auto& n : hram) n = 0;

	for(uint n = 0xC000; n < 0xE000; ++n) bus->map(n, this); //Wram
	for(uint n = 0xE000; n < 0xFE00; ++n) bus->map(n, this); //Wram mirror
	for(uint n = 0xFF80; n < 0xFFFF; ++n) bus->map(n, this); //Hram

	bus->map(0xFF00, this); //Joypad
	bus->map(0xFF01, this); //Serial transfer
	bus->map(0xFF02, this); //Serial Control
	bus->map(0xFF04, this); //Divider register
	bus->map(0xFF05, this); //Timer register
	bus->map(0xFF06, this); //Timer modulator
	bus->map(0xFF07, this); //Timer Control
	bus->map(0xFF0F, this); //Interrupt Flag
	bus->map(0xFFFF, this); //Interrupt enable

	std::memset(&status, 0, sizeof(status));

	r.halt = false;
	r.stop = false;
	r.ime = false;
	r.ei = false;
}
}
