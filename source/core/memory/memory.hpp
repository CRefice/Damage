#pragma once

#include "common\inttypes.hpp"

namespace core
{
class Bus;

class MMIO
{
public:
	inline void connect(Bus* bus_) { bus = bus_; }

	virtual uint8 readIO(uint16 addr) = 0;
	virtual void writeIO(uint16 addr, uint8 val) = 0;
	
protected:
	Bus* bus;
};

class Bus
{
public:
	void power();
	void map(uint16 addr, MMIO* device);

	uint8 read(uint16 addr);
	void write(uint16 addr, uint8 val);

private:
	MMIO* mmio[0x10000];
};
}
