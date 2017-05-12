#pragma once

#include "mapper.hpp"

namespace core
{
class MBC3 : public Mapper
{
public:
	void power();

	uint8 read(uint16 addr) final;
	void write(uint16 addr, uint8 val) final;

	void second();

private:
	uint8 romBank;
	uint8 ramBank;
	bool ramEnable;

	struct RTC
	{
		bool halt;
		bool latch;

		uint seconds;
		uint minutes;
		uint hours;
		uint days;
		bool dayCarry;

		uint latchSeconds;
		uint latchMinutes;
		uint latchHours;
		uint latchDays;
		bool latchDayCarry;
	} rtc;
};
}
