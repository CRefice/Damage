#pragma once

#include "mapper.hpp"

namespace core
{
class MBC1 : public Mapper
{
public:
	void power();

	uint8 read(uint16 addr) final;
	void write(uint16 addr, uint8 val) final;

private:
	uint romBank;
	uint ramBank;
	bool ramEnable;
	bool mode;
};
}
