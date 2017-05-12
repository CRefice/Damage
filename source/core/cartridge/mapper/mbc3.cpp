#include "mbc3.hpp"

namespace core
{
void MBC3::power() {
	romBank = 1;
	ramBank = 0;

	ramEnable = false;

	rtc.halt = true;
	rtc.latch = false;

	rtc.seconds = 0;
	rtc.minutes = 0;
	rtc.hours = 0;
	rtc.days = 0;
	rtc.dayCarry = false;

	rtc.latchSeconds = 0;
	rtc.latchMinutes = 0;
	rtc.latchHours = 0;
	rtc.latchDays = 0;
	rtc.latchDayCarry = false;
}

uint8 MBC3::read(uint16 addr) {
	if (addr < 0x4000) return readRom(addr);

	if (addr >= 0x4000 && addr < 0x8000) {
		return readRom((romBank << 14) | (addr & 0x3FFF));
	}

	if (addr >= 0xA000 && addr < 0xC000) {
		if (!ramEnable) return 0xFF;
		if (ramBank < 0x08) {
			return readRam((ramBank << 13) | (addr & 0x1FFF));
		}
		if (ramBank == 0x08) return rtc.latchSeconds;
		if (ramBank == 0x09) return rtc.latchMinutes;
		if (ramBank == 0x0A) return rtc.latchHours;
		if (ramBank == 0x0B) return rtc.latchDays;
		if (ramBank == 0x0C) return (rtc.latchDayCarry << 7) | (rtc.halt << 6) | (rtc.latchDayCarry >> 8);
	}

	return 0xFF;
}

void MBC3::write(uint16 addr, uint8 val) {
	if (addr < 0x2000) {
		ramEnable = (val & 0x0F) == 0x0A;
		return;
	}
	if (addr >= 0x2000 && addr < 0x4000) {
		romBank = (val & 0x7F) + uint8((val & 0x7F) == 0);
		return;
	}
	if (addr >= 0x4000 && addr < 0x6000) {
		ramBank = val;
		return;
	}
	if (addr >= 0x6000 && addr < 0x8000) {
		if (rtc.latch == 0 && val == 1) {
			rtc.latchSeconds = rtc.seconds;
			rtc.latchMinutes = rtc.minutes;
			rtc.latchHours = rtc.hours;
			rtc.latchDays = rtc.days;
			rtc.latchDayCarry = rtc.dayCarry;
		}
		rtc.latch = val != 0;
		return;
	}

	if (addr >= 0xA000 && addr < 0xC000) {
		if (!ramEnable) return;
		if (ramBank < 0x08) {
			writeRam((ramBank << 13) | (addr & 0x1FFF), val);
			return;
		}
		if (ramBank == 0x08) {
			rtc.seconds = val < 60 ? val : 0;
			return;
		}
		if (ramBank == 0x09) {
			rtc.minutes = val < 60 ? val : 0;
			return;
		}
		if (ramBank == 0x0A) {
			rtc.hours = val < 24 ? val : 0;
			return;
		}
		if (ramBank == 0x0B) {
			rtc.days = (rtc.days & 0x100) | val;
			return;
		}
		if (ramBank == 0x0C) {
			rtc.dayCarry = (val & 0x80) != 0;
			rtc.halt = (val & 0x40) != 0;
			rtc.days = ((val & 1) << 8) | (rtc.days & 0xff);
			return;
		}
	}
}

void MBC3::second() {
	if (rtc.halt) return;
	if (++rtc.seconds >= 60) {
		if (++rtc.minutes >= 60) {
			if (++rtc.hours >= 24) {
				if (++rtc.days >= 512) {
					rtc.days = 0;
					rtc.dayCarry = true;
				}
			}
		}
	}
}
}
