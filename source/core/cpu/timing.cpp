#include "cpu.hpp"

namespace core
{
void CPU::advanceTimer(uint cycles) {
	while (cycles--) {
		status.div++;

		if (status.timerEnable) {
			bool update = false;
			update |= (status.div & 15) == 0 && status.timerClock == 1;
			update |= (status.div & 63) == 0 && status.timerClock == 2;
			update |= (status.div & 255) == 0 && status.timerClock == 3;
			update |= (status.div & 511) == 0 && status.timerClock == 0;
			if (update) timerUpdate();
		}
		if (status.serialEnable) {
			if ((status.div & 1023) == 0 && status.serialClock)
				serialUpdate();
		}
	}
}

void CPU::timerUpdate() {
	if(++status.tima == 0) {
		status.tima = status.tma;
		irq.raise(Interrupt::Timer);
	}
}

void CPU::serialUpdate() {
	if (--status.serialBits == 0) {
		status.serialEnable = false;
		irq.raise(Interrupt::Serial);
	}
}
}
