#pragma once

#include "input\control-emu.hpp"

#include "core\memory\memory.hpp"
#include "core\interrupts.hpp"
#include "registers.hpp"

namespace input
{
class EmulatedControls;
}
namespace core
{
class CPU : public MMIO, public InterruptHandler
{
public:
	void power();
	void run();

	void setControls(input::EmulatedControls* controls) {
		emucontrols = controls;
	}

	uint clock() { return cycles; }

	InterruptRequester irq;

private:
	void cycle();

	uint8 read(uint16 addr);
	void write(uint16 addr, uint8 val);

	void stackPush(uint16 val);
	uint16 stackPop();

	//interrupts.cpp
	void handle(Interrupt i) final;
	void interrupt(uint16 addr);
	void interruptTest();

	//opcodes.cpp
	void instruction();
	void instructionCB();

	//io.cpp
	uint8 readIO(uint16 addr) final;
	void writeIO(uint16 addr, uint8 val) final;
	void joypadPoll();

	//timings.cpp
	void advanceTimer(uint cycles);
	void timerUpdate();
	void serialUpdate();

	struct Status
	{
		//FF00 - Joypad
		bool buttonEnable;
		bool dpadEnable;
		uint4 joyp;

		//FF01 - Serial transfer
		uint8 serialData;
		uint serialBits;

		//FF02 - Serial Control
		bool serialEnable;
		bool serialClock;

		//FF04 - Divider register
		uint16 div;

		//FF05 - Timer register
		uint8 tima;

		//FF06 - Timer modulator
		uint8 tma;

		//FF07 - Timer Control
		bool timerEnable;
		uint timerClock;

		//FF0F - Interrupt Flag
		bool interruptRequestJoypad;
		bool interruptRequestSerial;
		bool interruptRequestTimer;
		bool interruptRequestLcd;
		bool interruptRequestVblank;

		//FFFF - Interrupt enable
		bool interruptEnableJoypad;
		bool interruptEnableSerial;
		bool interruptEnableTimer;
		bool interruptEnableLcd;
		bool interruptEnableVblank;
	} status;

	uint cycles;

	input::EmulatedControls* emucontrols;

	uint8 wram[0x2000];
	uint8 hram[0x80];

	Registers r;

	#include "instructions.inl"
};
}
