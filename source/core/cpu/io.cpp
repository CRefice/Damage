#include "cpu.hpp"
#include "input\control-emu.hpp"

namespace core
{
void CPU::joypadPoll() {
	if (!emucontrols) return;

	uint button = 0, dpad = 0;

	button |= (emucontrols->buttonDown(Buttons::Start) << 3);
	button |= (emucontrols->buttonDown(Buttons::Select) << 2);
	button |= (emucontrols->buttonDown(Buttons::B) << 1);
	button |= (emucontrols->buttonDown(Buttons::A) << 0);

	dpad |= (emucontrols->buttonDown(Buttons::Down) << 3);
	dpad |= (emucontrols->buttonDown(Buttons::Up) << 2);
	dpad |= (emucontrols->buttonDown(Buttons::Left) << 1);
	dpad |= (emucontrols->buttonDown(Buttons::Right) << 0);

	if (dpad & 4) dpad &= ~8;  //disallow up+down
	if (dpad & 2) dpad &= ~1;  //disallow left+right

	status.joyp = 0x0F;
	if (!status.buttonEnable) status.joyp &= button ^ 0x0F;
	if (!status.dpadEnable) status.joyp &= dpad ^ 0x0F;
	if (status.joyp != 0x0F) irq.raise(Interrupt::Joypad);
}

uint8 CPU::readIO(uint16 addr) {
	if (addr >= 0xC000 && addr < 0xFE00) return wram[addr & 0x1FFF];
	if (addr >= 0xFF80 && addr < 0xFFFF) return hram[addr & 0x7F];

	//Joypad
	if (addr == 0xFF00) {
		joypadPoll();
		return 0xc0
			| (status.buttonEnable << 5)
			| (status.dpadEnable << 4)
			| (status.joyp);
	}

	//Serial transfer
	if (addr == 0xFF01) {
		return 0x00;
	}

	//Serial Control
	if (addr == 0xFF02) {
		return (status.serialEnable << 7)
		| 0x7E
		| (status.serialClock << 0);
	}

	//Divider register
	if (addr == 0xFF04) {
		return status.div >> 8;
	}

	//Timer register
	if (addr == 0xFF05) {
		return status.tima;
	}

	//Timer modulator
	if (addr == 0xFF06) {
		return status.tma;
	}

	//Timer Control
	if (addr == 0xFF07) {
		return 0xF8
		| (status.timerEnable << 2)
		| (status.timerClock << 0);
	}

	//Interrupt Flag
	if (addr == 0xFF0F) {
		return 0xE0
		| (status.interruptRequestJoypad << 4)
		| (status.interruptRequestSerial << 3)
		| (status.interruptRequestTimer << 2)
		| (status.interruptRequestLcd << 1)
		| (status.interruptRequestVblank << 0);
	}

	//Interrupt enable
	if (addr == 0xFFFF) {
		return 0xE0
		| (status.interruptEnableJoypad << 4)
		| (status.interruptEnableSerial << 3)
		| (status.interruptEnableTimer << 2)
		| (status.interruptEnableLcd << 1)
		| (status.interruptEnableVblank << 0);
	}

	return 0xFF;
}

void CPU::writeIO(uint16 addr, uint8 val) {
	if (addr >= 0xC000 && addr < 0xFE00) { wram[addr & 0x1FFF] = val; return; }
	if (addr >= 0xFF80 && addr < 0xFFFF) { hram[addr & 0x7F] = val; return; }

	//Joypad
	if (addr == 0xFF00) {
		status.buttonEnable = val.bit(5);
		status.dpadEnable = val.bit(4);
		return;
	}

	//Serial transfer
	if (addr == 0xFF01) {
		status.serialData = val;
		return;
	}

	//Serial Control
	if (addr == 0xFF02) {
		status.serialEnable = val.bit(7);
		status.serialClock = val.bit(0);
		if (status.serialEnable) status.serialBits = 8;
		return;
	}

	//Divider register
	if (addr == 0xFF04) {
		status.div = 0;
		return;
	}

	//Timer register
	if (addr == 0xFF05) {
		status.tima = val;
		return;
	}

	//Timer modulator
	if (addr == 0xFF06) {
		status.tma = val;
		return;
	}

	//Timer Control
	if (addr == 0xFF07) {
		status.timerEnable = val.bit(2);
		status.timerClock = (val & 0x03);
		return;
	}

	//Interrupt Flag
	if (addr == 0xFF0F) {
		status.interruptRequestJoypad = val.bit(4);
		status.interruptRequestSerial = val.bit(3);
		status.interruptRequestTimer  = val.bit(2);
		status.interruptRequestLcd    = val.bit(1);
		status.interruptRequestVblank = val.bit(0);
		return;
	}

	//Interrupt enable
	if (addr == 0xFFFF) {
		status.interruptEnableJoypad = val.bit(4);
		status.interruptEnableSerial = val.bit(3);
		status.interruptEnableTimer  = val.bit(2);
		status.interruptEnableLcd    = val.bit(1);
		status.interruptEnableVblank = val.bit(0);
		return;
	}
}
}
