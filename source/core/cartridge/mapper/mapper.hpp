#pragma once

#include "common\inttypes.hpp"

namespace core
{
struct MemBlock
{
	uint8* data = nullptr;
	uint size = 0;
};

class Mapper
{
public:	
	void connect(MemBlock* rom, MemBlock* ram);

	virtual uint8 read(uint16 addr) = 0;
	virtual void write(uint16 addr, uint8 val) = 0;

protected:
	uint8 readRom(uint addr);
	uint8 readRam(uint addr);
	void writeRam(uint addr, uint8 val);

private:
	MemBlock* rom;
	MemBlock* ram;
};
}
