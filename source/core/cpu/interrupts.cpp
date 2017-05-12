#include "cpu.hpp"

namespace core
{
void CPU::handle(Interrupt i) {
	switch (i) {
	case Interrupt::VBlank:
		status.interruptRequestVblank = true;
		if (status.interruptEnableVblank) r.halt = false;
		break;
	case Interrupt::LCD:
		status.interruptRequestLcd = true;
		if (status.interruptEnableLcd) r.halt = false;
		break;
	case Interrupt::Timer:
		status.interruptRequestTimer = true;
		if (status.interruptEnableTimer) r.halt = false;
		break;
	case Interrupt::Serial:
		status.interruptRequestSerial = true;
		if (status.interruptEnableSerial) r.halt = false;
		break;
	case Interrupt::Joypad:
		status.interruptRequestJoypad = true;
		if (status.interruptEnableJoypad) {
			r.halt = false;
			r.stop = false;
		}
		break;
	}
}

void CPU::interruptTest() {
	if (!r.ime) return;

	if (status.interruptRequestVblank && status.interruptEnableVblank) {
		status.interruptRequestVblank = false;
		return interrupt(0x0040);
	}
	if (status.interruptRequestLcd && status.interruptEnableLcd) {
		status.interruptRequestLcd = false;
		return interrupt(0x0048);
	}
	if (status.interruptRequestTimer && status.interruptEnableTimer) {
		status.interruptRequestTimer = false;
		return interrupt(0x0050);
	}
	if (status.interruptRequestSerial && status.interruptEnableSerial) {
		status.interruptRequestSerial = false;
		return interrupt(0x0058);
	}
	if (status.interruptRequestJoypad && status.interruptEnableJoypad) {
		status.interruptRequestJoypad = false;
		return interrupt(0x0060);
	}
}

void CPU::interrupt(uint16 addr) {
	cycle();
	cycle();
	cycle();
	r.ime = false;
	stackPush(r[PC]);
	r[PC] = addr;
}
}
